#define _TCSTYPEINTERFACE_
#include "tcstype.h"
//#include <shared/lib_util.h>
#include "lib_util.h"
//#include "shared/lib_weatherfile.h"
#include "lib_weatherfile.h"
#include <algorithm>

#include "interpolation_routines.h"
#include "solarpilot/AutoPilot_API.h"
#include "solarpilot/IOUtil.h"
#include "solarpilot/sort_method.h"

#include "csp_solver_pt_heliostatfield.h"
#include "csp_solver_util.h"

using namespace std;

static bool solarpilot_callback( simulation_info *siminfo, void *data );

/* 
A self-contained heliostat field type that directly calls SolarPilot through the AutoPilot API. The user
may optionally specify the heliostat field positions or may use this type to generate positions based on
input parameters.
 
This type can be run in three different modes. 

----------------------------------------------------------------------------------------------------------------------
					Parameters
----------------------------------------------------------------------------------------------------------------------

--- RUN_TYPE::AUTO --- SolarPILOT automatic mode
	** Generate field layout and characterize performance with user-specified macro-geometry **
	#######################################################################################################
	No.	|	Item						|	Variable				|	Units	|	Note
	----- INPUTS ------------------------------------------------------------------------------------------
	0	|	Run type					|	run_type				|	-		|	enum RUN_TYPE
	1	|	Heliostat width				|	helio_width				|	m		|
	2	|	Heliostat height			|	helio_height			|	m		|
	3	|	Heliostat optical error		|	helio_optical_error		|	rad		|
	4	|	Heliostat active frac.		|	helio_active_fraction	|	-		|
	5	|	Heliostat reflectance		|	helio_reflectance		|	-		|
	6	|	Receiver absorptance		|	rec_absorptance			|	-		|
	7	|	Receiver height				|	rec_height				|	m		|
	8	|	Receiver aspect ratio		|	rec_aspect				|	- 		|	(H/W)
	9	|	Receiver design heatloss	|	rec_hl_perm2			|	kW/m2	|
	10	|	Field thermal power rating	|	q_design				|	kW		|
	11	|	Tower height				|	h_tower					|	m		|
	12	|	Weather file name			|	weather_file			|	-		|	String
	13	|	Land boundary type			|	land_bound_type			|	- 		|	(sp_layout::LAND_BOUND_TYPE)
	14	|	Land max boundary			|	land_max				|	- OR m	|	X tower heights OR fixed radius
	15	|	Land min boundary			|	land_min				|	- OR m	|	X tower heights OR fixed radius
	16	|	<>Land boundary table		|	land_bound_table		|	m		|	Polygon {{x1,y1},{x2,y2},...}
	17	|	<>Boundary table listing	|	land_bound_list			|	-		|	Poly. sizes (- if exclusion) {L1,-L2,...}
	18	|	Heliostat startup energy	|	p_start					|	kWe-hr	|	
	19	|	Heliostat tracking energy	|	p_track					|	kWe		|	
	20	|	Stow/deploy elevation		|	hel_stow_deploy			|	deg		|
	21	|	Max. wind velocity			|	v_wind_max				|	m/s		|
	22	|	<>Interpolation nugget		|	interp_nug				|	-		|
	23	|	<>Interpolation beta coef.	|	interp_beta				|	-		|
	24	|	Flux map X resolution		|	n_flux_x				|	-		|
	25	|	Flux map Y resolution		|	n_flux_y				|	-		|
	----- Parameters set on Init() ------------------------------------------------------------------------
	26	|	Heliostat position table	|	helio_positions			|	m		|	{{x1,y1,z1},...}
	27	|	<>Heliostat aim point table	|	helio_aim_points		|	m		|	{{x1,y1,z1},...} receiver coordinates
	28	|	Number of heliostats		|	N_hel					|	-		|
	29	|	Field efficiency array		|	eta_map					| deg,deg,-	|	{{az1,el1,eff1},{az2,...}}
	30	|	Flux map sun positions		|	flux_positions			| deg,deg	|	{{az1,el1},{az2,...}}
	31	|	Flux map intensities		|	flux_maps				|	-		|	{{f11,f12,f13...},{f21,f22...}..}
	########################################################################################################
	<> = Optional


--- RUN_TYPE::USER_FIELD --- SolarPILOT user-field mode
	** User specifies heliostat positions, macro geometry, annual performance characterized internally **
	#################  Required inputs ####################################################################
	No.	|	Item						|	Variable				|	Units	|	Note
	-------------------------------------------------------------------------------------------------------
	0	|	Run type					|	run_type				|	-		|	enum RUN_TYPE
	1	|	Heliostat width				|	helio_width				|	m		|
	2	|	Heliostat height			|	helio_height			|	m		|
	3	|	Heliostat optical error		|	helio_optical_error		|	rad		|
	4	|	Heliostat active frac.		|	helio_active_fraction	|	-		|
	5	|	Heliostat reflectance		|	helio_reflectance		|	-		|
	6	|	Receiver absorptance		|	rec_absorptance			|	-		|
	7	|	Receiver height				|	rec_height				|	m		|
	8	|	Receiver aspect ratio		|	rec_aspect				|	- 		|	(H/W)
	9	|	Receiver design heatloss	|	rec_hl_perm2			|	kW/m2	|
	10	|	Field thermal power rating	|	q_design				|	kW		|
	11	|	Tower height				|	h_tower					|	m		|
	12	|	Weather file name			|	weather_file			|	-		|	String
	13	|	Land boundary type			|	land_bound_type			|	- 		|	(sp_layout::LAND_BOUND_TYPE)
	14	|	Land max boundary			|	land_max				|	- OR m	|	X tower heights OR fixed radius
	15	|	Land min boundary			|	land_min				|	- OR m	|	X tower heights OR fixed radius
	16	|	<>Land boundary table		|	land_bound_table		|	m		|	Polygon {{x1,y1},{x2,y2},...}
	17	|	<>Boundary table listing	|	land_bound_list			|	-		|	Poly. sizes (- if exclusion) {L1,-L2,...}
	18	|	Heliostat startup energy	|	p_start					|	kWe-hr	|	
	19	|	Heliostat tracking energy	|	p_track					|	kWe		|	
	20	|	Stow/deploy elevation		|	hel_stow_deploy			|	deg		|
	21	|	Max. wind velocity			|	v_wind_max				|	m/s		|
	22	|	<>Interpolation nugget		|	interp_nug				|	-		|
	23	|	<>Interpolation beta coef.	|	interp_beta				|	-		|
	24	|	Flux map X resolution		|	n_flux_x				|	-		|
	25	|	Flux map Y resolution		|	n_flux_y				|	-		|
	26  |   Atm. atten coef 0           |   c_atm_0                 |   -       |
	27  |   Atm. atten coef 1           |   c_atm_1                 |   1/km    |
	28  |   Atm. atten coef 2           |   c_atm_2                 |   1/km^2  |
	29  |   Atm. atten coef 3           |   c_atm_3                 |   1/km^3  |
	30  |   Number of helio. facets X   |   n_facet_x               |   -       |
	31  |   Number of helio. facets Y   |   n_facet_y               |   -       |
	32  |   Helio. canting type         |   cant_type               |   -       |   0=Flat, 1=Ideal, 2=Equinox, 3=Summer sol., 4=Winter sol
	33  |   Helio. focus type           |   focus_type              |   -       |   0=Flat, 1=Ideal
	34	|	Heliostat position table	|	helio_positions			|	m		|	{{x1,y1,z1},...}
	35	|	<>Heliostat aim point table	|	helio_aim_points		|	m		|	{{x1,y1,z1},...} receiver coordinates
	----- Parameters set on Init() ------------------------------------------------------------------------
	36	|	<>Heliostat aim point table	|	helio_aim_points		|	m		|	{{x1,y1,z1},...} receiver coordinates
	37	|	Number of heliostats		|	N_hel					|	-		|
	38	|	Field efficiency array		|	eta_map					| deg,deg,-	|	{{az1,el1,eff1},{az2,...}}
	39	|	Flux map sun positions		|	flux_positions			| deg,deg	|	{{az1,el1},{az2,...}}
	40	|	Flux map intensities		|	flux_maps				|	-		|	{{f11,f12,f13...},{f21,f22...}..}
	########################################################################################################
	<> = Optional

--- RUN_TYPE::USER_DATA --- User-data mode
	** User specifies field efficiency and flux intensity on the receiver vs. sun position. No SolarPILOT runs **
	#################  Required inputs ####################################################################
	No.	|	Item						|	Variable				|	Units	|	Note
	-------------------------------------------------------------------------------------------------------
	0	|	Run type					|	run_type				|	-		|	enum RUN_TYPE
	11	|	Tower height				|	h_tower					|	m		|
	12	|	Weather file name			|	weather_file			|	-		|	String
	13	|	Land boundary type			|	land_bound_type			|	- 		|	(sp_layout::LAND_BOUND_TYPE)
	14	|	Land max boundary			|	land_max				|	- OR m	|	X tower heights OR fixed radius
	15	|	Land min boundary			|	land_min				|	- OR m	|	X tower heights OR fixed radius
	16	|	<>Land boundary table		|	land_bound_table		|	m		|	Polygon {{x1,y1},{x2,y2},...}
	17	|	<>Boundary table listing	|	land_bound_list			|	-		|	Poly. sizes (- if exclusion) {L1,-L2,...}
	18	|	Heliostat startup energy	|	p_start					|	kWe-hr	|	
	19	|	Heliostat tracking energy	|	p_track					|	kWe		|	
	20	|	Stow/deploy elevation		|	hel_stow_deploy			|	deg		|
	21	|	Max. wind velocity			|	v_wind_max				|	m/s		|
	22	|	<>Interpolation nugget		|	interp_nug				|	-		|
	23	|	<>Interpolation beta coef.	|	interp_beta				|	-		|
	24	|	Flux map X resolution		|	n_flux_x				|	-		|
	25	|	Flux map Y resolution		|	n_flux_y				|	-		|
	28	|	Number of heliostats		|	N_hel					|	-		|
	29	|	Field efficiency array		|	eta_map					| deg,deg,-	|	{{az1,el1,eff1},{az2,...}}
	30	|	Flux map sun positions		|	flux_positions			| deg,deg	|	{{az1,el1},{az2,...}}
	31	|	Flux map intensities		|	flux_maps				|	-		|	{{f11,f12,f13...},{f21,f22...}..}
	32  |   Atm. atten coef 0           |   c_atm_0                 |   -       |
	33  |   Atm. atten coef 1           |   c_atm_1                 |   1/km    |
	34  |   Atm. atten coef 2           |   c_atm_2                 |   1/km^2  |
	35  |   Atm. atten coef 3           |   c_atm_3                 |   1/km^3  |
	36  |   Number of helio. facets X   |   n_facet_x               |   -       |
	37  |   Number of helio. facets Y   |   n_facet_y               |   -       |
	38  |   Helio. canting type         |   cant_type               |   -       |   0=Flat, 1=Ideal, 2=Equinox, 3=Summer sol., 4=Winter sol
	39  |   Helio. focus type           |   focus_type              |   -       |   0=Flat, 1=Ideal
	----- Parameters set on Init() ------------------------------------------------------------------------
	None
	########################################################################################################
	<> = Optional
	

Note: 
Annual efficiency is generated using non-uniform sun position spacing. The method implemented to handle
interpolation of non-uniform data is Gauss-Markov estimation (Kriging), with parameters set to induce
nearly linear interpolation that maintains fit fidelity with the original data.

*/


enum{	//Parameters
		P_run_type, 
		P_helio_width, 
		P_helio_height, 
		P_helio_optical_error, 
		P_helio_active_fraction, 
        P_dens_mirror,
		P_helio_reflectance, 
		P_rec_absorptance, 
		P_rec_height, 
		P_rec_aspect, 
		P_rec_hl_perm2, 
		P_q_design, 
		P_h_tower, 
		P_weather_file,
		P_land_bound_type, 
		P_land_max, 
		P_land_min, 
		P_land_bound_table, 
		P_land_bound_list, 
		P_p_start, 
		P_p_track, 
		P_hel_stow_deploy, 
		P_v_wind_max, 
		P_interp_nug, 
		P_interp_beta, 
		P_n_flux_x, 
		P_n_flux_y, 
		P_helio_positions, 
		P_helio_aim_points, 
		P_N_hel, 
		P_eta_map, 
		P_flux_positions, 
		P_flux_maps,
		P_c_atm_0,
		P_c_atm_1,
		P_c_atm_2,
		P_c_atm_3,
		P_n_facet_x,
		P_n_facet_y,
		P_cant_type,
		P_focus_type,
		P_n_flux_days,
		P_delta_flux_hrs,
		P_dni_des,
		P_land_area,

		//Inputs
		I_v_wind,
		I_field_control,
		I_solaz,
		I_solzen,

		//Outputs
		O_pparasi,
		O_eta_field,
		O_flux_map,

		//N_MAX
		N_MAX};

tcsvarinfo sam_mw_pt_heliostatfield_variables[] = {
    { TCS_PARAM,    TCS_NUMBER,   P_run_type,                "run_type",              "Run type",                                             "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_helio_width,             "helio_width",           "Heliostat width",                                      "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_helio_height,            "helio_height",          "Heliostat height",                                     "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_helio_optical_error,     "helio_optical_error",   "Heliostat optical error",                              "rad",    "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_helio_active_fraction,   "helio_active_fraction", "Heliostat active frac.",                               "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_dens_mirror,             "dens_mirror",           "Ratio of reflective area to profile",                  "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_helio_reflectance,       "helio_reflectance",     "Heliostat reflectance",                                "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_rec_absorptance,         "rec_absorptance",       "Receiver absorptance",                                 "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_rec_height,              "rec_height",            "Receiver height",                                      "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_rec_aspect,              "rec_aspect",            "Receiver aspect ratio",                                "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_rec_hl_perm2,            "rec_hl_perm2",          "Receiver design heatloss",                             "kW/m2",  "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_q_design,                "q_design",              "Field thermal power rating",                           "kW",     "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_h_tower,                 "h_tower",               "Tower height",                                         "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_STRING,   P_weather_file,            "weather_file",          "Weather file location",                                "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_land_bound_type,         "land_bound_type",       "Land boundary type",                                   "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_land_max,                "land_max",              "Land max boundary",                                    "- OR m", "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_land_min,                "land_min",              "Land min boundary",                                    "- OR m", "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_land_bound_table,        "land_bound_table",      "Land boundary table",                                  "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_ARRAY,    P_land_bound_list,         "land_bound_list",       "Boundary table listing",                               "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_p_start,                 "p_start",               "Heliostat startup energy",                             "kWe-hr", "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_p_track,                 "p_track",               "Heliostat tracking energy",                            "kWe",    "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_hel_stow_deploy,         "hel_stow_deploy",       "Stow/deploy elevation",                                "deg",    "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_v_wind_max,              "v_wind_max",            "Max. wind velocity",                                   "m/s",    "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_interp_nug,              "interp_nug",            "Interpolation nugget",                                 "-",      "",                              "", "0.0"       },
    { TCS_PARAM,    TCS_NUMBER,   P_interp_beta,             "interp_beta",           "Interpolation beta coef.",                             "-",      "",                              "", "1.99"      },
    { TCS_PARAM,    TCS_NUMBER,   P_n_flux_x,                "n_flux_x",              "Flux map X resolution",                                "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_n_flux_y,                "n_flux_y",              "Flux map Y resolution",                                "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_helio_positions,         "helio_positions",       "Heliostat position table",                             "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_helio_aim_points,        "helio_aim_points",      "Heliostat aim point table",                            "m",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_N_hel,                   "N_hel",                 "Number of heliostats",                                 "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_eta_map,                 "eta_map",               "Field efficiency array",                               "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_flux_positions,          "flux_positions",        "Flux map sun positions",                               "deg",    "",                              "", ""          },
    { TCS_PARAM,    TCS_MATRIX,   P_flux_maps,               "flux_maps",             "Flux map intensities",                                 "-",      "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_c_atm_0,                 "c_atm_0",               "Attenuation coefficient 0",                            "",       "",                              "", "0.006789"  },
    { TCS_PARAM,    TCS_NUMBER,   P_c_atm_0,                 "c_atm_1",               "Attenuation coefficient 1",                            "",       "",                              "", "0.1046"    },
    { TCS_PARAM,    TCS_NUMBER,   P_c_atm_0,                 "c_atm_2",               "Attenuation coefficient 2",                            "",       "",                              "", "-0.0107"   },
    { TCS_PARAM,    TCS_NUMBER,   P_c_atm_0,                 "c_atm_3",               "Attenuation coefficient 3",                            "",       "",                              "", "0.002845"  },
    { TCS_PARAM,    TCS_NUMBER,   P_n_facet_x,               "n_facet_x",             "Number of heliostat facets - X",                       "",       "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_n_facet_y,               "n_facet_y",             "Number of heliostat facets - Y",                       "",       "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_cant_type,               "cant_type",             "Heliostat cant method",                                "",       "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_focus_type,              "focus_type",            "Heliostat focus method",                               "",       "",                              "", ""          },
    { TCS_PARAM,    TCS_NUMBER,   P_n_flux_days,             "n_flux_days",           "No. days in flux map lookup",                          "",       "",                              "", "8"         },
    { TCS_PARAM,    TCS_NUMBER,   P_delta_flux_hrs,          "delta_flux_hrs",        "Hourly frequency in flux map lookup",                  "hrs",    "",                              "", "1"         },
    { TCS_PARAM,    TCS_NUMBER,   P_dni_des,                 "dni_des",               "Design-point DNI",                                     "W/m2",   "",                              "", ""          },
	{ TCS_PARAM,    TCS_NUMBER,   P_land_area,               "land_area",             "CALCULATED land area",                                 "acre",   "",                              "", ""          },
    
	{ TCS_INPUT,    TCS_NUMBER,   I_v_wind,                  "vwind",                 "Wind velocity",                                        "m/s",    "",                              "", ""          },
    { TCS_INPUT,    TCS_NUMBER,   I_field_control,           "field_control",         "Field defocus control",                                "",       "",                              "", ""          },
    { TCS_INPUT,    TCS_NUMBER,   I_solaz,                   "solaz",                 "Solar azimuth angle: 0 due north - clockwise to +360", "deg",    "",                              "", ""          },
    { TCS_INPUT,    TCS_NUMBER,   I_solzen,                  "solzen",                "Solar zenith angle",                                   "deg",    "",                              "", ""          },
    
	{ TCS_OUTPUT,   TCS_NUMBER,   O_pparasi,                 "pparasi",               "Parasitic tracking/startup power",                     "MWe",    "",                              "", ""          },
    { TCS_OUTPUT,   TCS_NUMBER,   O_eta_field,               "eta_field",             "Total field efficiency",                               "",       "",                              "", ""          },
    { TCS_OUTPUT,   TCS_MATRIX,   O_flux_map,                "flux_map",              "Receiver flux map",                                    "",       "n_flux_x cols x n_flux_y rows", "", ""          },


	//N_MAX
	{TCS_INVALID, TCS_INVALID, N_MAX,			0,					0, 0, 0, 0, 0	}
};

#ifdef _MSC_VER
#define mysnprintf _snprintf
#else
#define mysnprintf snprintf
#endif


#define pi 3.141592654
#define az_scale 6.283125908 
#define zen_scale 1.570781477 
#define eff_scale 0.7

class sam_mw_pt_heliostatfield : public tcstypeinterface
{
private:
	C_pt_heliostatfield mc_heliostatfield;
	
	// Class Instances
	//GaussMarkov *field_efficiency_table;
	// Flux table
	//sp_flux_table fluxtab;

	//Parameters
	//string weather_file;
	//int run_type;
	//double helio_width;
	//double helio_height;
	//double helio_optical_error;
	//double helio_active_fraction;
    //double dens_mirror;
	//double helio_reflectance;
	//double rec_absorptance;
	//double rec_height;
	//double rec_aspect;
	//double rec_hl_perm2;
	//double q_design;
	//double h_tower;
	//int land_bound_type;
	//double land_max;
	//double land_min;
	//double* land_bound_table;
	//double* land_bound_list;
	//double p_start;
	//double p_track;
	//double hel_stow_deploy;
	//double v_wind_max;
	//double interp_nug;
	//double interp_beta;
	//double* helio_positions;
	//double* helio_aim_points;
	//int N_hel; 
	//int pos_dim;
	//double* eta_map;
	//int n_flux_x;
	//int n_flux_y;
	//double* flux_positions;
    //MatDoub m_flux_positions;
	//double* flux_maps;
	//double* flux_map;
	//double c_atm_0, c_atm_1, c_atm_2, c_atm_3;
	//int n_facet_x, n_facet_y;
	//int cant_type, focus_type;
	//int n_flux_days, delta_flux_hrs;
	//double dni_des;
	
	//Stored Variables
	//double eta_prev;
	//double v_wind_prev;

public:

	//struct RUN_TYPE { enum A {AUTO, USER_FIELD, USER_DATA}; };

	sam_mw_pt_heliostatfield( tcscontext *cst, tcstypeinfo *ti)
		: tcstypeinterface( cst, ti)
	{
		//run_type	=0;
		//helio_width	=std::numeric_limits<double>::quiet_NaN();
		//helio_height	=std::numeric_limits<double>::quiet_NaN();
		//helio_optical_error	=std::numeric_limits<double>::quiet_NaN();
		//helio_active_fraction	=std::numeric_limits<double>::quiet_NaN();
		//dens_mirror = std::numeric_limits<double>::quiet_NaN();
        //helio_reflectance	=std::numeric_limits<double>::quiet_NaN();
		//rec_absorptance	=std::numeric_limits<double>::quiet_NaN();
		//rec_height	=std::numeric_limits<double>::quiet_NaN();
		//rec_aspect	=std::numeric_limits<double>::quiet_NaN();
		//rec_hl_perm2	=std::numeric_limits<double>::quiet_NaN();
		//q_design	=std::numeric_limits<double>::quiet_NaN();
		//h_tower	=std::numeric_limits<double>::quiet_NaN();
		//land_bound_type	=0;
		//land_max	=std::numeric_limits<double>::quiet_NaN();
		//land_min	=std::numeric_limits<double>::quiet_NaN();
		//land_bound_table	= NULL;
		//land_bound_list	= NULL;
		//p_start	=std::numeric_limits<double>::quiet_NaN();
		//p_track	=std::numeric_limits<double>::quiet_NaN();
		//hel_stow_deploy	=std::numeric_limits<double>::quiet_NaN();
		//v_wind_max	=std::numeric_limits<double>::quiet_NaN();
		//interp_nug	=std::numeric_limits<double>::quiet_NaN();
		//interp_beta	=std::numeric_limits<double>::quiet_NaN();
		//helio_positions	= NULL;
		//helio_aim_points	= NULL;
		//N_hel	=0;
		//pos_dim = 0;
		//eta_map	= NULL;
		//n_flux_x	=0;
		//n_flux_y	=0;
		//flux_positions	= NULL;
		//flux_maps	= NULL;
		//flux_map = NULL;
		//n_facet_x = 0;
		//n_facet_y = 0;
		//cant_type = 0;
		//focus_type = 0;
		//n_flux_days = 0;
		//delta_flux_hrs = 0; 
		
		//eta_prev = std::numeric_limits<double>::quiet_NaN();
		//v_wind_prev = std::numeric_limits<double>::quiet_NaN();
		//c_atm_0 = std::numeric_limits<double>::quiet_NaN();
		//c_atm_1 = std::numeric_limits<double>::quiet_NaN();
		//c_atm_2 = std::numeric_limits<double>::quiet_NaN();
		//c_atm_3 = std::numeric_limits<double>::quiet_NaN();
		//dni_des = std::numeric_limits<double>::quiet_NaN();

        //field_efficiency_table = 0;
	}

	virtual ~sam_mw_pt_heliostatfield()
	{
        //if(field_efficiency_table != 0)
		//    delete field_efficiency_table;
	}

	virtual int init()
	{
		mc_heliostatfield.ms_params.m_run_type = (int)value(P_run_type);
		mc_heliostatfield.ms_params.m_helio_width = value(P_helio_width);
		mc_heliostatfield.ms_params.m_helio_height = value(P_helio_height);
		mc_heliostatfield.ms_params.m_helio_optical_error = value(P_helio_optical_error);
		mc_heliostatfield.ms_params.m_helio_active_fraction = value(P_helio_active_fraction);
		mc_heliostatfield.ms_params.m_dens_mirror = value(P_dens_mirror);
		mc_heliostatfield.ms_params.m_helio_reflectance = value(P_helio_reflectance);
		mc_heliostatfield.ms_params.m_rec_absorptance = value(P_rec_absorptance);
		mc_heliostatfield.ms_params.m_rec_height = value(P_rec_height);
		mc_heliostatfield.ms_params.m_rec_aspect = value(P_rec_aspect);
		mc_heliostatfield.ms_params.m_rec_hl_perm2 = value(P_rec_hl_perm2);
		mc_heliostatfield.ms_params.m_q_design = value(P_q_design);
		mc_heliostatfield.ms_params.m_h_tower = value(P_h_tower);
		mc_heliostatfield.ms_params.m_weather_file = value_str(P_weather_file);
		mc_heliostatfield.ms_params.m_land_bound_type = (int)value(P_land_bound_type);
		mc_heliostatfield.ms_params.m_land_max = value(P_land_max);
		mc_heliostatfield.ms_params.m_land_min = value(P_land_min);

		int nrows_in = -1;
		int ncols_in = -1;
		double *input_matrix = value(P_land_bound_table, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_land_bound_table.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_land_bound_table(i, j) = TCS_MATRIX_INDEX(var(P_land_bound_table), i, j);
			}
		}

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_land_bound_list, &nrows_in);
		mc_heliostatfield.ms_params.m_land_bound_list.resize_fill(nrows_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			mc_heliostatfield.ms_params.m_land_bound_list(i, 0) = input_matrix[i];
		}

		mc_heliostatfield.ms_params.m_p_start = value(P_p_start);
		mc_heliostatfield.ms_params.m_p_track = value(P_p_track);
		mc_heliostatfield.ms_params.m_hel_stow_deploy = value(P_hel_stow_deploy);
		mc_heliostatfield.ms_params.m_v_wind_max = value(P_v_wind_max);
		mc_heliostatfield.ms_params.m_interp_nug = value(P_interp_nug);
		mc_heliostatfield.ms_params.m_interp_beta = value(P_interp_beta);
		mc_heliostatfield.ms_params.m_n_flux_x = (int)value(P_n_flux_x);
		mc_heliostatfield.ms_params.m_n_flux_y = (int)value(P_n_flux_y);

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_helio_positions, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_helio_positions.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_helio_positions(i, j) = TCS_MATRIX_INDEX(var(P_helio_positions), i, j);
			}
		}

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_helio_aim_points, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_helio_aim_points.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_helio_aim_points(i, j) = TCS_MATRIX_INDEX(var(P_helio_aim_points), i, j);
			}
		}

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_eta_map, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_eta_map.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_eta_map(i, j) = TCS_MATRIX_INDEX(var(P_eta_map), i, j);
			}
		}

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_flux_positions, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_flux_positions.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_flux_positions(i, j) = TCS_MATRIX_INDEX(var(P_flux_positions), i, j);
			}
		}

		nrows_in = -1;
		ncols_in = -1;
		input_matrix = value(P_flux_maps, &nrows_in, &ncols_in);
		mc_heliostatfield.ms_params.m_flux_maps.resize_fill(nrows_in, ncols_in, 0.0);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				mc_heliostatfield.ms_params.m_flux_maps(i, j) = TCS_MATRIX_INDEX(var(P_flux_maps), i, j);
			}
		}

		mc_heliostatfield.ms_params.m_c_atm_0 = value(P_c_atm_0);
		mc_heliostatfield.ms_params.m_c_atm_1 = value(P_c_atm_1);
		mc_heliostatfield.ms_params.m_c_atm_2 = value(P_c_atm_2);
		mc_heliostatfield.ms_params.m_c_atm_3 = value(P_c_atm_3);
		mc_heliostatfield.ms_params.m_n_facet_x = (int)value(P_n_facet_x);
		mc_heliostatfield.ms_params.m_n_facet_y = (int)value(P_n_facet_y);
		mc_heliostatfield.ms_params.m_cant_type = (int)value(P_cant_type);
		mc_heliostatfield.ms_params.m_focus_type = (int)value(P_focus_type);
		mc_heliostatfield.ms_params.m_n_flux_days = (int)value(P_n_flux_days);
		mc_heliostatfield.ms_params.m_delta_flux_hrs = (int)value(P_delta_flux_hrs);
		mc_heliostatfield.ms_params.m_dni_des = value(P_dni_des);

		mc_heliostatfield.ms_params.m_land_area = value(P_land_area);

		mc_heliostatfield.mf_callback = solarpilot_callback;
		mc_heliostatfield.m_cdata = (void*)this;

		int out_type = -1;
		std::string out_msg = "";

		try
		{
			//mc_heliostatfield.init(solarpilot_callback, (void*)this);
			mc_heliostatfield.init();
		}

		catch( C_csp_exception &csp_exception )
		{
			// Report warning before exiting with error
			while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}

		// If no exception, then report messages and move on
		while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}


		// Read back in parameters that have been updated
		// P_helio_positions: should be correct matrix_t - to - TCS_MATRIX syntax
		nrows_in = -1;
		ncols_in = -1;
		nrows_in = mc_heliostatfield.ms_params.m_helio_positions.nrows();
		ncols_in = mc_heliostatfield.ms_params.m_helio_positions.ncols();
		double *p_param = allocate(P_helio_positions, nrows_in, ncols_in);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				TCS_MATRIX_INDEX(var(P_helio_positions), i, j) = mc_heliostatfield.ms_params.m_helio_positions(i, j);
			}
		}

		// P_land_area
		value(P_land_area, mc_heliostatfield.ms_params.m_land_area);

		// P_eta_map: should be correct matrix_t - to - TCS_MATRIX syntax
		nrows_in = -1;
		ncols_in = -1;
		nrows_in = mc_heliostatfield.ms_params.m_eta_map.nrows();
		ncols_in = mc_heliostatfield.ms_params.m_eta_map.ncols();
		p_param = allocate(P_eta_map, nrows_in, ncols_in);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				TCS_MATRIX_INDEX(var(P_eta_map), i, j) = mc_heliostatfield.ms_params.m_eta_map(i, j);
			}
		}

		// P_flux_maps: should be correct matrix_t - to - TCS_MATRIX syntax
		nrows_in = -1;
		ncols_in = -1;
		nrows_in = mc_heliostatfield.ms_params.m_flux_maps.nrows();
		ncols_in = mc_heliostatfield.ms_params.m_flux_maps.ncols();
		p_param = allocate(P_flux_maps, nrows_in, ncols_in);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				TCS_MATRIX_INDEX(var(P_flux_maps), i, j) = mc_heliostatfield.ms_params.m_flux_maps(i, j);
			}
		}

		// P_flux_positions: same issue as P_helio_positions
		nrows_in = -1;
		ncols_in = -1;
		nrows_in = mc_heliostatfield.ms_params.m_flux_positions.nrows();
		ncols_in = mc_heliostatfield.ms_params.m_flux_positions.ncols();
		p_param = allocate(P_flux_positions, nrows_in, ncols_in);
		for( int i = 0; i < nrows_in; i++ )
		{
			for( int j = 0; j < ncols_in; j++ )
			{
				TCS_MATRIX_INDEX(var(P_flux_positions), i, j) = mc_heliostatfield.ms_params.m_flux_positions(i, j);
			}
		}

		//size the output
		p_param = allocate(O_flux_map, mc_heliostatfield.ms_params.m_n_flux_y, mc_heliostatfield.ms_params.m_n_flux_x);

		return 0;
	}

	virtual int call( double time, double step, int ncall )
	{						
		double v_wind_csp = value(I_v_wind);
		double field_control_csp = value(I_field_control);
		double solzen_csp = value(I_solzen);
		double solaz_csp = value(I_solaz);

		int out_type = -1;
		std::string out_msg = "";

		try
		{
			mc_heliostatfield.call(v_wind_csp, field_control_csp, solaz_csp, solzen_csp,
				time, ncall, step);
		}

		catch( C_csp_exception &csp_exception )
		{
			// Report warning before exiting with error
			while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}
		
		// If no exception, then report messages and move on
		while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}

		// Get outputs from heliostatfield class and set to TCS OUTPUT values
			// First, clear out the existing flux map
		for( int j = 0; j<mc_heliostatfield.ms_params.m_n_flux_y; j++ )
			for( int i = 0; i<mc_heliostatfield.ms_params.m_n_flux_x; i++ )
				TCS_MATRIX_INDEX(var(O_flux_map), j, i) = 0.;

			// Then, set values
		for( int j = 0; j<mc_heliostatfield.ms_params.m_n_flux_y; j++ )
			for( int i = 0; i<mc_heliostatfield.ms_params.m_n_flux_x; i++ )
				TCS_MATRIX_INDEX(var(O_flux_map), j, i) = mc_heliostatfield.ms_outputs.m_flux_map_out(j,i);
		
			// Set remaining outputs
		value( O_pparasi, mc_heliostatfield.ms_outputs.m_pparasi );
		value( O_eta_field, mc_heliostatfield.ms_outputs.m_eta_field );

		return 0;
	}

	virtual int converged( double time )
	{
		int out_type = -1;
		std::string out_msg = "";

		try
		{
			mc_heliostatfield.converged();
		}

		catch( C_csp_exception &csp_exception )
		{
			// Report warning before exiting with error
			while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
			{
				if( out_type == C_csp_messages::NOTICE )
					message(TCS_NOTICE, out_msg.c_str());
				else if( out_type == C_csp_messages::WARNING )
					message(TCS_WARNING, out_msg.c_str());
			}

			message(TCS_ERROR, csp_exception.m_error_message.c_str());
			return -1;
		}

		// If no exception, then report messages and move on
		while( mc_heliostatfield.mc_csp_messages.get_message(&out_type, &out_msg) )
		{
			if( out_type == C_csp_messages::NOTICE )
				message(TCS_NOTICE, out_msg.c_str());
			else if( out_type == C_csp_messages::WARNING )
				message(TCS_WARNING, out_msg.c_str());
		}

		return 0;
	}

	int relay_message( string &msg, double percent ){
		return progress( percent, msg.c_str() ) ? 0 : -1;
	}

	double rdist(VectDoub *p1, VectDoub *p2, int dim=2){
		double d=0;
		for(int i=0; i<dim; i++){
			double rd = p1->at(i) - p2->at(i);
			d += rd * rd;
		}
		return sqrt(d);
	}

};

static bool solarpilot_callback( simulation_info *siminfo, void *data )
{
	sam_mw_pt_heliostatfield *cm = static_cast<sam_mw_pt_heliostatfield*>( data );
	if ( !cm ) return false;
	float simprogress = (float)siminfo->getCurrentSimulation()/(float)(max(siminfo->getTotalSimulationCount(),1));
	
	return cm->relay_message( *siminfo->getSimulationNotices(), simprogress*100.0f ) == 0;
}

TCS_IMPLEMENT_TYPE( sam_mw_pt_heliostatfield, "Heliostat field with SolarPILOT", "Mike Wagner", 1, sam_mw_pt_heliostatfield_variables, NULL, 1 )

