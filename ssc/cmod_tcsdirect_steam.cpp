#include "core.h"
#include "tckernel.h"
// for adjustment factors
#include "common.h"

static var_info _cm_vtab_tcsdirect_steam[] = {
/*	EXAMPLE LINES FOR INPUTS
    { SSC_INPUT,        SSC_NUMBER,      "XXXXXXXXXXXXXX",      "Label",                                                          "",             "",            "sca",            "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "INTINTINTINT",        "Label",                                                          "",             "",            "parasitic",      "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_ARRAY,       "XXXXXXXXXXX",         "Number indicating the receiver type",                            "",             "",            "hce",            "*",                       "",                      "" },
    { SSC_INPUT,        SSC_MATRIX,      "XXXXXXXXXXX",         "Label",                                                          "",             "",            "tes",            "*",                       "",                      "" },
*/														     
														     
//    VARTYPE           DATATYPE          NAME                   LABEL                                                            UNITS           META            GROUP            REQUIRED_IF                 CONSTRAINTS             UI_HINTS
	{ SSC_INPUT, SSC_STRING, "solar_resource_file", "local weather file path", "", "", "Weather", "*", "LOCAL_FILE", "" },
	
	{ SSC_INPUT, SSC_NUMBER, "system_capacity", "Nameplate capacity", "kW", "", "direct steam tower", "*", "", "" },

	/*
	{ SSC_INPUT, SSC_STRING, "file_name", "local weather file path", "", "", "Weather", "*", "LOCAL_FILE", "" },
	{ SSC_INPUT,        SSC_NUMBER,      "track_mode",          "Tracking mode",                                                  "",             "",            "Weather",        "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "tilt",                "Tilt angle of surface/axis",                                     "",             "",            "Weather",        "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "azimuth",             "Azimuth angle of surface/axis",                                  "",             "",            "Weather",        "*",                       "",                      "" },
	*/													     
    // TOU												     
    { SSC_INPUT,        SSC_MATRIX,      "weekday_schedule",    "12x24 Time of Use Values for week days",                         "",             "",            "tou_translator", "*",                       "",                      "" }, 
    { SSC_INPUT,        SSC_MATRIX,      "weekend_schedule",    "12x24 Time of Use Values for week end days",                     "",             "",            "tou_translator", "*",                       "",                      "" }, 
	/*
	// Heliostat field (type 221) parameters			     
    { SSC_INPUT,        SSC_MATRIX,      "eta_map",             "Field efficiency matrix",                                          "-",            "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "n_zen",               "Number of zenith angle data points in file",                       "-",            "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "n_azi",               "Number of azimuth angle data points in file",                      "-",            "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "n_hel",               "Number of heliostats in the field",                                "-",            "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "q_start",             "Electric work for starting up one heliostat",                      "kWe-hr",       "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "p_run",               "Electric power for tracking one heliostat",                        "kWe",          "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "v_wind_max",          "Maximum tolerable wind speed",                                     "m/s",          "",            "heliostat",      "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "hel_stow_deploy",     "Heliostat field stow/deploy solar elevation angle",                "deg",          "",            "heliostat",      "*",                       "",                      "" },
	// Heliostat field (type 221) inputs				       
    { SSC_INPUT,        SSC_NUMBER,      "field_control",       "Field defocus control",                                            "",             "",            "heliostat",      "*",                       "",                      "" },
	*/	

	// Heliostat field  parameters				     										
	{ SSC_INPUT, SSC_NUMBER, "run_type", "Run type", "-", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "helio_width", "Heliostat width", "m", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "helio_height", "Heliostat height", "m", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "helio_optical_error", "Heliostat optical error", "rad", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "helio_active_fraction", "Heliostat active frac.", "-", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "helio_reflectance", "Heliostat reflectance", "-", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "rec_absorptance", "Receiver absorptance", "-", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "rec_aspect", "Receiver aspect ratio", "-", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "rec_hl_perm2", "Receiver design heatloss", "kW/m2", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "land_bound_type", "Land boundary type", "-", "", "heliostat", "?=0", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "land_max", "Land max boundary", "-ORm", "", "heliostat", "?=7.5", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "land_min", "Land min boundary", "-ORm", "", "heliostat", "?=0.75", "", "" },
	{ SSC_INPUT, SSC_MATRIX, "land_bound_table", "Land boundary table", "m", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_ARRAY, "land_bound_list", "Boundary table listing", "-", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "dni_des", "Design-point DNI", "W/m2", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "p_start", "Heliostat startup energy", "kWe-hr", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "p_track", "Heliostat tracking energy", "kWe", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "hel_stow_deploy", "Stow/deploy elevation", "deg", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "v_wind_max", "Max. wind velocity", "m/s", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "interp_nug", "Interpolation nugget", "-", "", "heliostat", "?=0", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "interp_beta", "Interpolation beta coef.", "-", "", "heliostat", "?=1.99", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "n_flux_x", "Flux map X resolution", "-", "", "heliostat", "?=12", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "n_flux_y", "Flux map Y resolution", "-", "", "heliostat", "?=1", "", "" },
    { SSC_INPUT, SSC_NUMBER, "dens_mirror", "Ratio of reflective area to profile", "-", "", "heliostat", "*", "", ""},
	{ SSC_INPUT, SSC_MATRIX, "helio_positions", "Heliostat position table", "m", "", "heliostat", "run_type=1", "", "" },
	{ SSC_INPUT, SSC_MATRIX, "helio_aim_points", "Heliostat aim point table", "m", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "N_hel", "Number of heliostats", "-", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_MATRIX, "eta_map", "Field efficiency array", "-", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_MATRIX, "flux_positions", "Flux map sun positions", "deg", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_MATRIX, "flux_maps", "Flux map intensities", "-", "", "heliostat", "?", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "c_atm_0", "Attenuation coefficient 0", "", "", "heliostat", "?=0.006789", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "c_atm_1", "Attenuation coefficient 1", "", "", "heliostat", "?=0.1046", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "c_atm_2", "Attenuation coefficient 2", "", "", "heliostat", "?=-0.0107", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "c_atm_3", "Attenuation coefficient 3", "", "", "heliostat", "?=0.002845", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "n_facet_x", "Number of heliostat facets - X", "", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "n_facet_y", "Number of heliostat facets - Y", "", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "focus_type", "Heliostat focus method", "", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "cant_type", "Heliostat cant method", "", "", "heliostat", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "n_flux_days", "No. days in flux map lookup", "", "", "heliostat", "?=8", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "delta_flux_hrs", "Hourly frequency in flux map lookup", "", "", "heliostat", "?=1", "", "" },

	//


	{ SSC_INPUT, SSC_NUMBER, "H_rec", "The height of the receiver", "m", "", "receiver", "*", "", "" },
	{ SSC_INPUT, SSC_NUMBER, "THT", "The height of the tower (hel. pivot to rec equator)", "m", "", "receiver", "*", "", "" },

	//// Heliostat field inputs					     																	  
	//   { SSC_INPUT,        SSC_NUMBER,      "field_control",        "Field defocus control",                                             "",             "",            "heliostat",      "*",                       "",                      "" },
	//   { SSC_INPUT,        SSC_NUMBER,      "vwind",                "Wind velocity",                                                     "m/s",          "",            "heliostat",      "*",                       "",                     "" },
	//   { SSC_INPUT,        SSC_NUMBER,      "solaz",                "Solar azimuth angle: 0 due north, clocwise to +360",                "deg",          "",            "heliostat",      "*",                       "",                     "" },
	//   { SSC_INPUT,        SSC_NUMBER,      "solzen",               "Solar zenith angle",                                                "deg",          "",            "heliostat",      "*",                       "",                     "" },

	// Direct steam controller (type 265) parameters	       
    { SSC_INPUT,        SSC_NUMBER,      "fossil_mode",         "Fossil model: 1=Normal, 2=Supplemental",                           "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "q_pb_design",         "Heat rate into powerblock at design",                              "MW",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "q_aux_max",           "Maximum heat rate of auxiliary heater",                            "MW",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "lhv_eff",             "Aux Heater lower heating value efficiency",                        "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "h_tower",             "Tower Height",                                                     "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "n_panels",            "Number of panels",                                                 "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "flowtype",            "Code for flow pattern through rec.",                               "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "d_rec",               "Diameter of Receiver",                                             "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "q_rec_des",           "Design-point thermal power",                                       "MW",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "f_rec_min",           "Minimum receiver absorbed power fraction",                         "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "rec_qf_delay",        "Receiver start-up delay fraction of thermal energy of receiver running at design for 1 hour", "-", "", "dsg_controller", "*",                  "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "rec_su_delay",        "Receiver start-up delay time",                                     "hr",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "f_pb_cutoff",         "Cycle cut-off fraction",                                           "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "f_pb_sb",             "Cycle minimum standby fraction",                                   "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "t_standby_ini",       "Power block standby time",                                         "hr",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "x_b_target",          "Target boiler outlet quality",                                     "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "eta_rec_pump",        "Feedwater pump efficiency",                                        "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_hp_in_des",         "Design HP Turbine Inlet Pressure",                                 "bar",          "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_hp_out_des",        "Design HP Turbine Outlet Pressure",                                "bar",          "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "f_mdotrh_des",        "Design reheat mass flow rate fraction",                            "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "p_cycle_design",      "Design Cycle Power",                                               "MW",           "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "ct",                  "Cooling Type",                                                     "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_amb_des",           "Design ambient temperature (power cycle)",                         "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "dT_cw_ref",           "Reference condenser water dT",                                     "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_approach",          "Approach temperature for wet cooling",                             "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_ITD_des",           "Approach temperature for dry cooling",                             "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "hl_ffact",            "Heat Loss Fudge FACTor",                                           "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "h_boiler",            "Height of boiler",                                                 "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "d_t_boiler",          "O.D. of boiler tubes",                                             "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "th_t_boiler",         "Thickness of boiler tubes",                                        "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "emis_boiler",         "Emissivity of boiler tubes",                                       "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "abs_boiler",          "Absorptance of boiler tubes",                                      "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "mat_boiler",          "Numerical code for tube material",                                 "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "th_fin",              "Thickness of fin",                                                 "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "l_fin",               "Length of fin (distance between tubes)",                           "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "emis_fin",            "Emissivity of fin",                                                "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "abs_fin",             "Absorptance of fin",                                               "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "mat_fin",             "Numerical code for fin material",                                  "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "h_sh",                "Height of superheater",                                            "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "d_sh",                "O.D. of superheater tubes",                                        "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "th_sh",               "Thickness of superheater tubes",                                   "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "emis_sh",             "Emissivity of superheater tubes",                                  "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "abs_sh",              "Absorptance of superheater tubes",                                 "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "mat_sh",              "Numerical code for superheater material",                          "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_sh_out_des",        "Target superheater outlet temperature",                            "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "h_rh",                "Height of reheater",                                               "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "d_rh",                "O.D. of reheater tubes",                                           "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "th_rh",               "Thickness of reheater tubes",                                      "m",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "emis_rh",             "Emissivity of reheater tubes",                                     "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "abs_rh",              "Absorptance of reheater tubes",                                    "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "mat_rh",              "Numerical code for reheater material",                             "-",            "",            "dsg_controller", "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_rh_out_des",        "Target reheater outlet temperature",                               "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "cycle_max_frac",      "Cycle maximum overdesign fraction",                                "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "A_sf",                "Solar field area",                                                 "m^2",          "",            "dsg_controller", "*",                       "",                      "" },
	{ SSC_INPUT,        SSC_ARRAY,       "ffrac",               "Fossil dispatch logic",                                            "-",            "",            "dsg_controller", "*",                       "",                      "" },
//    { SSC_INPUT,        SSC_MATRIX,      "fluxmap_angles",      "Matrix containing zenith and azimuth angles for flux maps",        "-",            "",            "dsg_controller", "*",                       "",                      "" },
 //   { SSC_INPUT,        SSC_MATRIX,      "fluxmap",             "Matrix containing flux map for various solar positions",           "-",            "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_ARRAY,       "TOU_schedule",        "Annual hourly time-of-use schedule",                               "-",            "",            "dsg_controller", "*",                       "",                      "" },
	// Direct steam controller (type 265) inputs		       
  //{ SSC_INPUT,        SSC_NUMBER,      "azimuth",             "Solar azimuth",                                                    "deg",          "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "zenith",              "Solar zenith",                                                     "deg",          "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "DNI",                 "Direct normal irradiance",                                         "W/m^2",        "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "T_amb",               "Ambient dry bulb temperature",                                     "C",            "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "v_wind_10",           "Wind speed at 10 m",                                               "m/s",          "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "P_atm",               "Ambient Pressure",                                                 "atm",          "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "T_dp",                "Dew point temperature",                                            "C",            "",            "dsg_controller", "*",                       "",                      "" },
  //{ SSC_INPUT,        SSC_NUMBER,      "field_eff",           "Heliostat field efficiency",                                       "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_b_in_init",         "Initial Boiler inlet pressure",                                    "bar",          "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "f_mdot_rh_init",      "Reheat mass flow rate fraction",                                   "-",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_hp_out",            "HP turbine outlet pressure",                                       "bar",          "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_hp_out",            "HP turbine outlet temperature",                                    "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_rh_target",         "Target reheater outlet temp.",                                     "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_fw_init",           "Initial Feedwater outlet temperature",                             "C",            "",            "dsg_controller", "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_cond_init",         "Condenser pressure",                                               "Pa",           "",            "dsg_controller", "*",                       "",                      "" },
														     
	// Power block (type 234) parameters				     
    { SSC_INPUT,        SSC_NUMBER,      "P_ref",               "Reference output electric power at design condition",               "MW",          "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "eta_ref",             "Reference conversion efficiency at design condition",               "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_hot_ref",           "Reference HTF inlet temperature at design",                         "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_cold_ref",          "Reference HTF outlet temperature at design",                        "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "dT_cw_ref",           "Reference condenser cooling water inlet/outlet T diff",             "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_amb_des",           "Reference ambient temperature at design point",                     "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "q_sby_frac",          "Fraction of thermal power required for standby mode",               "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_boil_des",          "Boiler operating pressure @ design",                                "bar",         "",            "powerblock",     "*",                       "",                      "" },
//    { SSC_INPUT,        SSC_NUMBER,      "is_rh",               "Flag indicating whether reheat is used 0:no, 1:yes",                "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_rh_ref",            "Reheater operating pressure at design",                             "bar",         "",            "powerblock",     "*",                       "",                      "" },
//    { SSC_INPUT,        SSC_NUMBER,      "T_rh_hot_ref",        "Reheater design outlet temperature",                                "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "rh_frac_ref",         "Reheater flow fraction at design",                                  "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "CT",                  "Flag for using dry cooling or wet cooling system",                  "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "startup_time",        "Time needed for power block startup",                               "hr",          "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "startup_frac",        "Fraction of design thermal power needed for startup",               "none",        "",            "powerblock",     "*",                       "",                      "" },
//    { SSC_INPUT,        SSC_NUMBER,      "tech_type",           "Flag indicating which coef. set to use. (1=tower,2=trough,3=user)", "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "T_approach",          "Cooling tower approach temperature",                                "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_ITD_des",           "ITD at design for dry system",                                      "C",           "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_cond_ratio",        "Condenser pressure ratio",                                          "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "pb_bd_frac",          "Power block blowdown steam fraction ",                              "none",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "P_cond_min",          "Minimum condenser pressure",                                        "inHg",        "",            "powerblock",     "*",                       "",                      "" },
    { SSC_INPUT,        SSC_NUMBER,      "n_pl_inc",            "Number of part-load increments for the heat rejection system",      "none",        "",            "powerblock",     "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_ARRAY,       "F_wc",                "Fraction indicating wet cooling use for hybrid system",             "none",        "",            "powerblock",     "*",                       "",                      "" },
	// Power block (type 234) inputs					       
    { SSC_INPUT,        SSC_NUMBER,      "mode",                "Cycle part load control, from plant controller",                    "none",        "",            "powerblock",     "*",                       "INTEGER",               "" },
    { SSC_INPUT,        SSC_NUMBER,      "T_hot",               "Hot HTF inlet temperature, from storage tank",                      "C",           "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "m_dot_st",            "HTF mass flow rate",                                                "kg/hr",       "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "T_wb",                "Ambient wet bulb temperature",                                      "C",           "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "demand_var",          "Control signal indicating operational mode",                        "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "standby_control",     "Control signal indicating standby mode",                            "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "T_db",                "Ambient dry bulb temperature",                                      "C",           "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "P_amb",               "Ambient pressure",                                                  "atm",         "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "TOU",                 "Current Time-of-use period",                                        "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "relhum",              "Relative humidity of the ambient air",                              "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "f_recSU",             "Fraction powerblock can run due to receiver startup",               "none",        "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "dp_b",                "Pressure drop in boiler",                                           "Pa",          "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "dp_sh",               "Pressure drop in superheater",                                      "Pa",          "",            "powerblock",     "*",                       "",                      "" },
//	{ SSC_INPUT,        SSC_NUMBER,      "dp_rh",               "Pressure drop in reheater",                                         "Pa",          "",            "powerblock",     "*",                       "",                      "" },
															  
	// Parasitics (type 228) parameters						  
    {SSC_INPUT,         SSC_NUMBER,      "P_storage_pump",      "Storage pump power, rated per MWt of storage use",                  "MWe/MWt",     "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "Piping_loss",         "Thermal loss per meter of piping",                                  "Wt/m",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "Piping_length",       "Total length of exposed piping",                                    "m",           "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "Design_power",        "Power production at design conditions",                             "MWe",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "recirc_htr_eff",      "Recirculation heater efficiency",                                   "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "design_eff",          "Power cycle efficiency at design",                                  "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "night_recirc",        "Flag indicating whether night recirculation is allowed",            "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "pb_fixed_par",        "Fixed parasitic load - runs at all times",                          "MWe/MWcap",   "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "aux_par",             "Aux heater, boiler parasitic",                                      "MWe/MWcap",   "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "aux_par_f",           "Aux heater, boiler parasitic - multiplying fraction",               "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "aux_par_0",           "Aux heater, boiler parasitic - constant coefficient",               "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "aux_par_1",           "Aux heater, boiler parasitic - linear coefficient",                 "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "aux_par_2",           "Aux heater, boiler parasitic - quadratic coefficient",              "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "bop_par",             "Balance of plant parasitic power fraction",                         "MWe/MWcap",   "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "bop_par_f",           "Balance of plant parasitic power fraction - mult frac",             "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "bop_par_0",           "Balance of plant parasitic power fraction - const coeff",           "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "bop_par_1",           "Balance of plant parasitic power fraction - linear coeff",          "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "bop_par_2",           "Balance of plant parasitic power fraction - quadratic coeff",       "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "storage_bypass",      "Flag indicating whether the hot salt pump always runs w/ PB",       "none",        "",            "parasitics",     "*",                       "",                      "" },
    // Parasitics (type 228) inputs							   
    {SSC_INPUT,         SSC_NUMBER,      "flow_from_storage",   "Flow rate from storage",                                           "kg/hr",       "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "P_cooling_tower",     "Cooling tower parasitic power fraction",                            "MWe",         "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "P_tower_pump",        "Reported tower pump power",                                         "MWe",         "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "P_helio_track",       "Reported heliostat tracking power",                                 "MWe",         "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "P_plant_output",      "Reported plant power output",                                       "MWe",         "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "eta_cycle",           "Power cycle efficiency",                                            "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "P_cold_tank",         "Cold tank heater parasitic power",                                  "MWe",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "P_hot_tank",          "Hot tank heater parasitic power",                                   "MWe",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "P_tower_conv",        "Reported tower convection loss",                                    "MWt",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "P_tower_rad",         "Reported tower radiation loss",                                     "MWt",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "recirc_source",       "Recirculation heater control",                                      "none",        "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "ref_htf_flow",        "HTF flow rate through the power cycle at design",                   "kg/hr",       "",            "parasitics",     "*",                       "",                      "" },
//	{SSC_INPUT,         SSC_NUMBER,      "aux_power",           "Auxiliary heater thermal power output",                             "MWt",         "",            "parasitics",     "*",                       "",                      "" },
    {SSC_INPUT,         SSC_NUMBER,      "P_htf_pump",          "HTF pumping power",                                                 "MWe",         "",            "parasitics",     "*",                       "",                      "" },


// OUTPUTS
// The names of the output variables should match the parameter names for the TCS units in order to signal to the TCS kernel to store the values by timestep

//    VARTYPE           DATATYPE         NAME                   LABEL                                                             UNITS           META           GROUP             REQUIRED_IF                CONSTRAINTS              UI_HINTS
	{ SSC_OUTPUT,       SSC_ARRAY,       "tdry",                "Dry bulb temperature",                                           "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "twet",                "Wet bulb temperature",                                           "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "wspd",                "Wind speed",                                                     "m/s",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "beam",                "Beam normal irradiance",                                         "W/m2",         "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "eta_field",           "Total field efficiency",                                         "",             "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "defocus",             "Defocus fraction",                                               "",             "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_fw",                "Feedwater outlet temp",                                          "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_b_in",              "Boiler Inlet Temperature",                                       "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_boiling",           "Boiler temp (= recirc temp, drum temp)",                         "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_b_in",              "Boiler inlet pressure",                                          "kPa",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_b_out",             "Boiler outlet pressure",                                         "kPa",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_drop_b",            "Boiler pressure drop",                                           "Pa",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "eta_b",               "Boiler thermal efficiency",                                      "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_b_conv",            "Boiler convective losses",                                       "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_b_rad",             "Boiler radiative losses",                                        "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_b_abs",             "Boiler absorbed power",                                          "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_max_b_surf",        "Max boiler surface temperature",                                 "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "m_dot_sh",            "Mass flow rate through superheater",                             "kg/hr",        "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_sh_out",            "Superheater outlet pressure",                                    "kPa",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "dP_sh",               "Superheater pressure drop",                                      "Pa",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "eta_sh",              "Superheater thermal efficiency",                                 "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_sh_conv",           "Superheater convective losses",                                  "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_sh_rad",            "Superheater radiative losses",                                   "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_sh_abs",            "Superheater absorbed power",                                     "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_max_sh_surf",       "Max superheater surface temperature",                            "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "v_sh_max",            "Superheater exit velocity",                                      "m/s",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "f_mdot_rh",           "Reheater mass flow rate fraction",                               "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_rh_in",             "Reheater inlet pressure",                                        "kPa",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_rh_in",             "Reheater inlet temperature",                                     "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_rh_out",            "Reheater outlet pressure",                                       "kPa",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_rh_out",            "Reheater outlet temperature",                                    "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "dP_rh",               "Pressure drop through reheater",                                 "Pa",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "eta_rh",              "Thermal efficiency of reheater",                                 "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "T_max_rh_surf",       "Max reheater surface temperature",                               "C",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "v_rh_max",            "Exit velocity through reheater",                                 "m/s",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_rh_conv",           "Reheater convective losses",                                     "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_rh_rad",            "Reheater radiative losses",                                      "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_rh_abs",            "Reheater absorbed power",                                        "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_inc_full",          "Incident radiation before defocus",                              "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_abs_rec",           "Total receiver absorbed power",                                  "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_rad_rec",           "Total receiver radiative losses",                                "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_abs_less_rad",      "Receiver->Comb: Power to receiver less radiation",               "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_conv_rec",          "Total receiver convective losses",                               "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_therm_in_rec",      "Rate of energy into steam/water",                                "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "eta_rec",             "Receiver thermal efficiency",                                    "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "m_dot_aux",           "Auxiliary mass flow rate",                                       "kg/hr",        "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_aux",               "Auxiliary heat rate delivered to cycle",                         "MW",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "q_aux_fuel",          "Fuel energy rate to aux heater",                                 "MMBTU",        "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_cycle",             "Cycle power output",                                             "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_out_net",           "Cycle power output",                                             "MWh",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_plant_balance_tot", "Total balance of plant parasitic power",                         "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_fixed",             "Total fixed parasitic loss",                                     "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_cooling_tower_tot", "Total cooling tower parasitic power",                            "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "W_dot_boost",         "Parasitics: Receiver pumping power",                             "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "pparasi",			    "Parasitic tracking/startup power",                               "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_piping_tot",        "Total piping loss parasitic power",                              "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_parasitics",        "Overall parasitic losses",                                       "MWe",          "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "m_dot_makeup",        "Cooling water makeup flow rate",                                 "kg/hr",        "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "f_bays",              "Fraction of operating heat rejection bays",                      "-",            "",            "Outputs",        "*",                       "LENGTH=8760",           "" },
	{ SSC_OUTPUT,       SSC_ARRAY,       "P_cond",              "Condenser pressure",                                             "Pa",           "",            "Outputs",        "*",                       "LENGTH=8760",           "" },

	{ SSC_OUTPUT, SSC_ARRAY, "hourly_energy", "Hourly Energy", "kW", "", "Net_E_Calc", "*", "LENGTH=8760", "" },

	// Annual Outputs
	{ SSC_OUTPUT, SSC_NUMBER, "annual_energy", "Annual Energy", "kW", "", "Net_E_Calc", "*", "", "" },


	{ SSC_OUTPUT, SSC_NUMBER, "capacity_factor", "Capacity factor", "", "", "", "*", "", "" },
	{ SSC_OUTPUT, SSC_NUMBER, "kwh_per_kw", "First year kWh/kW", "", "", "", "*", "", "" },
	{ SSC_OUTPUT, SSC_NUMBER, "system_heat_rate", "System heat rate", "MMBtu/MWh", "", "", "*", "", "" },
	{ SSC_OUTPUT, SSC_NUMBER, "annual_fuel_usage", "Annual fuel usage", "kWh", "", "", "*", "", "" },

	var_info_invalid };

class cm_tcsdirect_steam : public tcKernel
{
public:

	cm_tcsdirect_steam(tcstypeprovider *prov)
	:tcKernel(prov)
	{
		add_var_info( _cm_vtab_tcsdirect_steam );
		//set_store_all_parameters(true); // default is 'false' = only store TCS parameters that match the SSC_OUTPUT variables above
		// performance adjustment factors
		add_var_info(vtab_adjustment_factors);
	}

	void exec( ) throw( general_error )
	{
		//bool debug_mode = (__DEBUG__ == 1);  // When compiled in VS debug mode, this will use the trnsys weather file; otherwise, it will attempt to open the file with name that was passed in
		bool debug_mode = false;
		//Add weather file reader unit
		int weather = 0;
		if(debug_mode) weather = add_unit("trnsys_weatherreader", "TRNSYS weather reader");
		else weather = add_unit("weatherreader", "TCS weather reader");

		// Add units
		int	tou = add_unit("tou_translator", "Time of Use Translator");
		//int type221_hel_field = add_unit("sam_mw_pt_type221");
		int type_hel_field = add_unit("sam_mw_pt_heliostatfield");
		int type265_dsg_controller = add_unit("sam_dsg_controller_type265");
		int type234_powerblock = add_unit("sam_mw_type234");
		int type228_parasitics = add_unit("sam_mw_pt_type228");

		if(debug_mode)
		{
			set_unit_value( weather, "file_name", "C:/svn_NREL/main/ssc/tcsdata/typelib/TRNSYS_weather_outputs/daggett_trnsys_weather.out" );
			set_unit_value( weather, "i_hour", "TIME" );
			set_unit_value( weather, "i_month", "month" );
			set_unit_value( weather, "i_day", "day" );
			set_unit_value( weather, "i_global", "GlobalHorizontal" );
			set_unit_value( weather, "i_beam", "DNI" );
			set_unit_value( weather, "i_diff", "DiffuseHorizontal" );
			set_unit_value( weather, "i_tdry", "T_dry" );
			set_unit_value( weather, "i_twet", "T_wet" );
			set_unit_value( weather, "i_tdew", "T_dew" );
			set_unit_value( weather, "i_wspd", "WindSpeed" );
			set_unit_value( weather, "i_wdir", "WindDir" );
			set_unit_value( weather, "i_rhum", "RelHum" );
			set_unit_value( weather, "i_pres", "AtmPres" );
			set_unit_value( weather, "i_snow", "SnowCover" );
			set_unit_value( weather, "i_albedo", "GroundAlbedo" );
			set_unit_value( weather, "i_poa", "POA" );
			set_unit_value( weather, "i_solazi", "Azimuth" );
			set_unit_value( weather, "i_solzen", "Zenith" );
			set_unit_value( weather, "i_lat", "Latitude" );
			set_unit_value( weather, "i_lon", "Longitude" );
			set_unit_value( weather, "i_shift", "Shift" );
		}
		else
		{
			//Set weatherreader parameters
			//set_unit_value_ssc_string( weather, "file_name" );
			//set_unit_value_ssc_double( weather, "track_mode" );
			//set_unit_value_ssc_double( weather, "tilt" );
			//set_unit_value_ssc_double( weather, "azimuth" );

			//Set weatherreader parameters
			set_unit_value(weather, "file_name", as_string("solar_resource_file"));
			set_unit_value(weather, "track_mode", 0.0);
			set_unit_value(weather, "tilt", 0.0);
			set_unit_value(weather, "azimuth", 0.0);

		}

		set_unit_value_ssc_matrix(tou, "weekday_schedule"); // tou values from control will be between 1 and 9
		set_unit_value_ssc_matrix(tou, "weekend_schedule");

		/*
		// Set heliostat field parameters
		set_unit_value_ssc_matrix( type221_hel_field, "eta_map");//, eta_field_map );
		set_unit_value_ssc_double( type221_hel_field, "n_zen");//, num_zen );
		set_unit_value_ssc_double( type221_hel_field, "n_azi");//, num_azi );
		set_unit_value_ssc_double( type221_hel_field, "n_hel");//, num_helio );
		set_unit_value_ssc_double( type221_hel_field, "q_start");//, P_hel_start );
		set_unit_value_ssc_double( type221_hel_field, "p_run");//, P_hel_track );
		set_unit_value_ssc_double( type221_hel_field, "v_wind_max");//, wind_stow_speed );
		set_unit_value_ssc_double( type221_hel_field, "hel_stow_deploy");//, hel_stow_deploy );
        set_unit_value_ssc_double( type221_hel_field, "field_control" ); // INITIAL VALUE
		


		// Connect heliostat field inputs
		bool bConnected = connect( weather, "wspd", type221_hel_field, "vwind" );
		bConnected &= connect( weather, "solzen", type221_hel_field, "theta" );
		bConnected &= connect( weather, "solazi", type221_hel_field, "phi" );
		*/


		// Heliostat field
		set_unit_value_ssc_double(type_hel_field, "run_type");// , 0);	//0=auto, 1=user-type_hel_field, 2=user data
		set_unit_value_ssc_double(type_hel_field, "helio_width");//, 12.);
		set_unit_value_ssc_double(type_hel_field, "helio_height");//, 12.);
		set_unit_value_ssc_double(type_hel_field, "helio_optical_error");//, 0.00153);
		set_unit_value_ssc_double(type_hel_field, "helio_active_fraction");//, 0.97);
		set_unit_value_ssc_double(type_hel_field, "helio_reflectance");//, 0.90);
		set_unit_value_ssc_double(type_hel_field, "rec_absorptance");//, 0.94);
		set_unit_value_ssc_double(type_hel_field, "rec_height", as_double("H_rec"));//, 5.);
		set_unit_value_ssc_double(type_hel_field, "rec_aspect");//, 1);
		set_unit_value_ssc_double(type_hel_field, "rec_hl_perm2");//, 0.);
		set_unit_value_ssc_double(type_hel_field, "q_design", as_double("q_rec_des"));//, 25.);
		set_unit_value_ssc_double(type_hel_field, "dni_des");
		set_unit_value_ssc_double(type_hel_field, "h_tower", as_double("THT"));//, 50);
		set_unit_value(type_hel_field, "weather_file", as_string("solar_resource_file"));
		set_unit_value_ssc_double(type_hel_field, "land_bound_type");//, 0);
		set_unit_value_ssc_double(type_hel_field, "land_max");//, 7.5);
		set_unit_value_ssc_double(type_hel_field, "land_min");//, 0.75);
		set_unit_value_ssc_double(type_hel_field, "p_start");//, 0.025);
		set_unit_value_ssc_double(type_hel_field, "p_track");//, 0.055);
		set_unit_value_ssc_double(type_hel_field, "hel_stow_deploy");//, 8);
		set_unit_value_ssc_double(type_hel_field, "v_wind_max");//, 25.);
		set_unit_value_ssc_double(type_hel_field, "n_flux_x");//, 10);
		set_unit_value_ssc_double(type_hel_field, "n_flux_y");//, 1);
        set_unit_value_ssc_double(type_hel_field, "dens_mirror");
		set_unit_value_ssc_double(type_hel_field, "c_atm_0");
		set_unit_value_ssc_double(type_hel_field, "c_atm_1");
		set_unit_value_ssc_double(type_hel_field, "c_atm_2");
		set_unit_value_ssc_double(type_hel_field, "c_atm_3");
		set_unit_value_ssc_double(type_hel_field, "n_facet_x");
		set_unit_value_ssc_double(type_hel_field, "n_facet_y");
		set_unit_value_ssc_double(type_hel_field, "focus_type");
		set_unit_value_ssc_double(type_hel_field, "cant_type");
		set_unit_value_ssc_double(type_hel_field, "n_flux_days");
		set_unit_value_ssc_double(type_hel_field, "delta_flux_hrs");

		//set_unit_value_ssc_double(type_hel_field, "n_flux_days", 2.0);
		//set_unit_value_ssc_double(type_hel_field, "delta_flux_hrs", 2.0);

		int run_type = (int)get_unit_value_number(type_hel_field, "run_type");
		/*if(run_type == 0){

		}
		else */
		if (run_type == 1){
			set_unit_value_ssc_matrix(type_hel_field, "helio_positions");
		}
		else if (run_type == 2){
			set_unit_value_ssc_matrix(type_hel_field, "eta_map");
			set_unit_value_ssc_matrix(type_hel_field, "flux_positions");
			set_unit_value_ssc_matrix(type_hel_field, "flux_maps");
		}

		// for user specified x,y field

		bool bConnected = connect(weather, "wspd", type_hel_field, "vwind");
		set_unit_value_ssc_double(type_hel_field, "field_control", 1.);
		set_unit_value_ssc_double(weather, "solzen", 90.);	//initialize to be on the horizon
		bConnected &= connect(weather, "solzen", type_hel_field, "solzen");
		bConnected &= connect(weather, "solazi", type_hel_field, "solaz");





		//Set DSG Controller Parameters
		set_unit_value_ssc_double( type265_dsg_controller, "fossil_mode"); //fossil_mode);
        set_unit_value_ssc_double( type265_dsg_controller, "q_pb_design"); //, as_double("p_cycle_design")/as_double("Eff_cycle_design") );
		set_unit_value_ssc_double( type265_dsg_controller, "q_aux_max"); //, as_double("p_cycle_design")/as_double("Eff_cycle_design")); //q_pb_design);
		set_unit_value_ssc_double( type265_dsg_controller, "lhv_eff"); //LHV_eff);
		set_unit_value_ssc_double( type265_dsg_controller, "h_tower"); //h_tower);
		set_unit_value_ssc_double( type265_dsg_controller, "n_panels"); //num_panels);
		set_unit_value_ssc_double( type265_dsg_controller, "flowtype"); //flow_pattern);
		set_unit_value_ssc_double( type265_dsg_controller, "d_rec"); //d_rec);
		set_unit_value_ssc_double( type265_dsg_controller, "q_rec_des"); //Q_rec_des);
		set_unit_value_ssc_double( type265_dsg_controller, "f_rec_min"); //f_rec_min);
		set_unit_value_ssc_double( type265_dsg_controller, "rec_qf_delay"); //rec_qf_delay);
		set_unit_value_ssc_double( type265_dsg_controller, "rec_su_delay"); //rec_su_delay);
		set_unit_value_ssc_double( type265_dsg_controller, "f_pb_cutoff"); //cycle_cutoff_frac);
		set_unit_value_ssc_double( type265_dsg_controller, "f_pb_sb"); //F_standby);
		set_unit_value_ssc_double( type265_dsg_controller, "t_standby_ini"); //T_standby);
		set_unit_value_ssc_double( type265_dsg_controller, "x_b_target"); //x_b_target);
		set_unit_value_ssc_double( type265_dsg_controller, "eta_rec_pump"); //eta_rec_pump);
		set_unit_value_ssc_double( type265_dsg_controller, "P_hp_in_des"); //P_HP_in);
		set_unit_value_ssc_double( type265_dsg_controller, "P_hp_out_des"); //P_HP_out);
		set_unit_value_ssc_double( type265_dsg_controller, "f_mdotrh_des"); //rh_frac_ref);
		set_unit_value_ssc_double( type265_dsg_controller, "p_cycle_design"); //P_cycle_design);
		set_unit_value_ssc_double( type265_dsg_controller, "ct"); //Cool_type);
		set_unit_value_ssc_double( type265_dsg_controller, "T_amb_des"); //T_amb_des);
		set_unit_value_ssc_double( type265_dsg_controller, "dT_cw_ref"); //dT_cooling_ref);
		set_unit_value_ssc_double( type265_dsg_controller, "T_approach"); //T_approach);
		set_unit_value_ssc_double( type265_dsg_controller, "T_ITD_des"); //T_ITD_des);
		set_unit_value_ssc_double( type265_dsg_controller, "hl_ffact"); //hl_ffact);
		set_unit_value_ssc_double( type265_dsg_controller, "h_boiler"); //h_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "d_t_boiler"); //d_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "th_t_boiler"); //th_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "emis_boiler"); //emis_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "abs_boiler"); //abs_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "mat_boiler"); //Mat_boiler);
		set_unit_value_ssc_double( type265_dsg_controller, "th_fin"); //th_fin);
		set_unit_value_ssc_double( type265_dsg_controller, "l_fin"); //L_fin);
		set_unit_value_ssc_double( type265_dsg_controller, "emis_fin"); //emis_fin);
		set_unit_value_ssc_double( type265_dsg_controller, "abs_fin"); //abs_fin);
		set_unit_value_ssc_double( type265_dsg_controller, "mat_fin"); //Mat_fin);
		set_unit_value_ssc_double( type265_dsg_controller, "h_sh"); //h_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "d_sh"); //d_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "th_sh"); //th_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "emis_sh"); //emis_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "abs_sh"); //abs_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "mat_sh"); //Mat_SH);
		set_unit_value_ssc_double( type265_dsg_controller, "T_sh_out_des"); //T_SH_out_ref);
		set_unit_value_ssc_double( type265_dsg_controller, "h_rh"); //h_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "d_rh"); //d_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "th_rh"); //th_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "emis_rh"); //emis_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "abs_rh"); //abs_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "mat_rh"); //Mat_RH);
		set_unit_value_ssc_double( type265_dsg_controller, "T_rh_out_des"); //T_rh_out_ref);
		set_unit_value_ssc_double( type265_dsg_controller, "cycle_max_frac"); //cycle_max_fraction);
		set_unit_value_ssc_double( type265_dsg_controller, "A_sf");//, A_sf );
		set_unit_value_ssc_array( type265_dsg_controller, "ffrac");
		set_unit_value_ssc_double(type265_dsg_controller, "n_flux_x");
		set_unit_value_ssc_double(type265_dsg_controller, "n_flux_y");

//		set_unit_value_ssc_matrix( type265_dsg_controller, "fluxmap_angles"); //arr_sol_pos);
//		set_unit_value_ssc_matrix( type265_dsg_controller, "fluxmap"); //arr_flux);
		//set_unit_value_ssc_array( type265_dsg_controller, "TOU_schedule");

		// initial values for dsg controller
		set_unit_value_ssc_double(type265_dsg_controller, "P_b_in", as_double("P_b_in_init")); //P_HP_in);			// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "f_mdot_rh", as_double("f_mdot_rh_init")); //rh_frac_ref);		// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "P_hp_out"); //P_HP_out);			// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "T_hp_out"); //300);				// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "T_rh_target"); //T_rh_out_ref);	// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "T_fw", as_double("T_fw_init")); //340);					// Initial value
		set_unit_value_ssc_double(type265_dsg_controller, "P_cond", as_double("P_cond_init")); //P_HP_out);			// Initial value


		// Connect DSG Controller Inputs
		bConnected &= connect(weather, "solazi", type265_dsg_controller, "azimuth");
		bConnected &= connect(weather, "solzen", type265_dsg_controller, "zenith");
		bConnected &= connect(weather, "beam", type265_dsg_controller, "DNI");
		bConnected &= connect(weather, "tdry", type265_dsg_controller, "T_amb");
		bConnected &= connect(weather, "wspd", type265_dsg_controller, "v_wind_10");
		bConnected &= connect(weather, "pres", type265_dsg_controller, "P_atm");
		bConnected &= connect(weather, "tdew", type265_dsg_controller, "T_dp");
//		bConnected &= connect(type221_hel_field, "eta_field", type265_dsg_controller, "field_eff");
		bConnected &= connect(type_hel_field, "eta_field", type265_dsg_controller, "field_eff");
		bConnected &= connect(type234_powerblock, "P_boiler_in", type265_dsg_controller, "P_b_in");
		bConnected &= connect(type234_powerblock, "f_rh", type265_dsg_controller, "f_mdot_rh");
		bConnected &= connect(type234_powerblock, "P_rh_in", type265_dsg_controller, "P_hp_out");
		bConnected &= connect(type234_powerblock, "T_rh_in", type265_dsg_controller, "T_hp_out");
		bConnected &= connect(type234_powerblock, "T_rh_out", type265_dsg_controller, "T_rh_target");
		bConnected &= connect(type234_powerblock, "T_cold", type265_dsg_controller, "T_fw");
		bConnected &= connect(type234_powerblock, "P_cond", type265_dsg_controller, "P_cond");
		bConnected &= connect(tou, "tou_value", type265_dsg_controller, "TOUPeriod");
		bConnected &= connect(type_hel_field, "flux_map", type265_dsg_controller, "flux_map");
//		bConnected &= connect(type_hel_field, "flux_positions", type265_dsg_controller, "fluxmap_angles");


		// Set Powerblock Parameters
		set_unit_value_ssc_double(type234_powerblock, "P_ref"); //P_cycle_design);
		set_unit_value_ssc_double(type234_powerblock, "eta_ref"); //Eff_cycle_design);
		set_unit_value_ssc_double(type234_powerblock, "T_hot_ref"); //T_SH_out_ref);
		set_unit_value_ssc_double(type234_powerblock, "T_cold_ref"); //-1.23);				// This value isn't used in DSG
		set_unit_value_ssc_double(type234_powerblock, "dT_cw_ref"); //dT_cooling_ref);
		set_unit_value_ssc_double(type234_powerblock, "T_amb_des"); //T_amb_des);
		set_unit_value_ssc_double(type234_powerblock, "q_sby_frac"); //F_standby);
		set_unit_value_ssc_double(type234_powerblock, "P_boil_des"); //P_HP_in);
		
		
		//set_unit_value_ssc_double(type234_powerblock, "is_rh"); //is_rh);
		set_unit_value_ssc_double(type234_powerblock, "is_rh", 1);
		
		
		set_unit_value_ssc_double(type234_powerblock, "P_rh_ref"); //P_HP_out);
		
		
		//set_unit_value_ssc_double(type234_powerblock, "T_rh_hot_ref"); //T_rh_out_ref);
		set_unit_value_ssc_double(type234_powerblock, "T_rh_hot_ref", as_double("T_rh_out_des"));
		
		
		set_unit_value_ssc_double(type234_powerblock, "rh_frac_ref"); //rh_frac_ref);
		set_unit_value_ssc_double(type234_powerblock, "CT"); //Cool_type);
		set_unit_value_ssc_double(type234_powerblock, "startup_time"); //startup_time);
		set_unit_value_ssc_double(type234_powerblock, "startup_frac"); //startup_frac);
		
		
		//set_unit_value_ssc_double(type234_powerblock, "tech_type"); //tech_type);
		set_unit_value_ssc_double(type234_powerblock, "tech_type", 5);
		
		
		set_unit_value_ssc_double(type234_powerblock, "T_approach"); //T_approach);
		set_unit_value_ssc_double(type234_powerblock, "T_ITD_des"); //T_ITD_des);
		set_unit_value_ssc_double(type234_powerblock, "P_cond_ratio"); //P_cond_ratio);
		set_unit_value_ssc_double(type234_powerblock, "pb_bd_frac"); //pb_bd_frac);
		set_unit_value_ssc_double(type234_powerblock, "P_cond_min"); //min_cond_pres);
		set_unit_value_ssc_double(type234_powerblock, "n_pl_inc"); //hr_pl_nlev);
		set_unit_value_ssc_array(type234_powerblock, "F_wc"); //[HC_LOGIC0, HC_LOGIC1, HC_LOGIC2, HC_LOGIC3, HC_LOGIC4, HC_LOGIC5, HC_LOGIC6, HC_LOGIC7, HC_LOGIC8]);

		// Set Powerblock initial values
		set_unit_value_ssc_double(type234_powerblock, "mode");//, 2);
		set_unit_value_ssc_double(type234_powerblock, "T_hot"); //T_SH_out_ref);
		//set_unit_value_ssc_double(type234_powerblock, "TOU");//, 1);

		// Connect Powerblock Inputs (and initial values?)
		bConnected &= connect(type265_dsg_controller, "m_dot_toPB", type234_powerblock, "m_dot_st");
		bConnected &= connect(weather, "twet", type234_powerblock, "T_wb");
		bConnected &= connect(type265_dsg_controller, "m_dot_toPB", type234_powerblock, "m_dot_st");
		bConnected &= connect(type265_dsg_controller, "standby_control", type234_powerblock, "standby_control");
		bConnected &= connect(weather, "tdry", type234_powerblock, "T_db");
		bConnected &= connect(weather, "pres", type234_powerblock, "P_amb");
		bConnected &= connect(weather, "rhum", type234_powerblock, "relhum");
		bConnected &= connect(type265_dsg_controller, "f_timestep", type234_powerblock, "f_recSU");
		bConnected &= connect(type265_dsg_controller, "P_drop_b", type234_powerblock, "dp_b");
		bConnected &= connect(type265_dsg_controller, "dP_sh", type234_powerblock, "dp_sh");
		bConnected &= connect(type265_dsg_controller, "dP_rh", type234_powerblock, "dp_rh");
		bConnected &= connect(tou, "tou_value", type234_powerblock, "TOU");

		// Set Parasitics Parameters
		set_unit_value_ssc_double(type228_parasitics, "P_storage_pump"); //P_storage_pump);
		set_unit_value_ssc_double(type228_parasitics, "Piping_loss"); //piping_loss );
		set_unit_value_ssc_double(type228_parasitics, "Piping_length"); //piping_length );
		set_unit_value_ssc_double(type228_parasitics, "Design_power"); //P_cycle_design );
		set_unit_value_ssc_double(type228_parasitics, "recirc_htr_eff"); //recirc_htr_eff );
		set_unit_value_ssc_double(type228_parasitics, "design_eff"); //Eff_cycle_design );
		set_unit_value_ssc_double(type228_parasitics, "night_recirc"); //night_recirc );
		set_unit_value_ssc_double(type228_parasitics, "pb_fixed_par"); //pb_fixed_par );
		set_unit_value_ssc_double(type228_parasitics, "aux_par"); //aux_par );
		set_unit_value_ssc_double(type228_parasitics, "aux_par_f"); //aux_par_f );
		set_unit_value_ssc_double(type228_parasitics, "aux_par_0"); //aux_par_0 );
		set_unit_value_ssc_double(type228_parasitics, "aux_par_1"); //aux_par_1 );
		set_unit_value_ssc_double(type228_parasitics, "aux_par_2"); //aux_par_2 );
		set_unit_value_ssc_double(type228_parasitics, "bop_par"); //bop_par );
		set_unit_value_ssc_double(type228_parasitics, "bop_par_f"); //bop_par_f );
		set_unit_value_ssc_double(type228_parasitics, "bop_par_0"); //bop_par_0 );
		set_unit_value_ssc_double(type228_parasitics, "bop_par_1"); //bop_par_1 );
		set_unit_value_ssc_double(type228_parasitics, "bop_par_2"); //bop_par_2 );
		set_unit_value_ssc_double(type228_parasitics, "storage_bypass"); //storage_bypass );

		// Set Parasitics Inputs (Initial values?)
		set_unit_value_ssc_double(type228_parasitics, "flow_from_storage", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "P_cold_tank", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "P_hot_tank", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "P_tower_conv", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "P_tower_rad", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "recirc_source", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "ref_htf_flow", 0.0);
		set_unit_value_ssc_double(type228_parasitics, "P_htf_pump", 0.0);

		bConnected &= connect(type234_powerblock, "W_cool_par", type228_parasitics, "P_cooling_tower");
		bConnected &= connect(type265_dsg_controller, "W_dot_boost", type228_parasitics, "P_tower_pump");
//		bConnected &= connect(type221_hel_field, "pparasi", type228_parasitics, "P_helio_track");
		bConnected &= connect(type_hel_field, "pparasi", type228_parasitics, "P_helio_track");
		bConnected &= connect(type234_powerblock, "P_cycle", type228_parasitics, "P_plant_output");
		bConnected &= connect(type234_powerblock, "eta", type228_parasitics, "eta_cycle");
		bConnected &= connect(type265_dsg_controller, "q_aux", type228_parasitics, "aux_power");

		// check if all connections worked
		if ( !bConnected )
			throw exec_error( "tcsdirect_steam", util::format("there was a problem connecting outputs of one unit to inputs of another for the simulation.") );


		// Run simulation
		size_t hours = 8760;
		int error = simulate(3600, hours * 3600, 3600, 30);
//		if (0 > simulate(3600, hours*3600, 3600) )
		if (0>error)
			throw exec_error( "tcsdirect_steam", util::format("there was a problem simulating in the TCS direct steam power tower model.") );

		// get the outputs
		if (!set_all_output_arrays() )
			throw exec_error( "tcsdirect_steam", util::format("there was a problem returning the results from the simulation.") );

		set_output_array("hourly_energy", "P_out_net", 8760, 1000.0); // MWh to kWh

		//calculated field parameters
		int nr, nc;
		if (double *fm = get_unit_value(type_hel_field, "flux_maps", &nr, &nc))
		{
			ssc_number_t *ssc_fm = allocate("flux_lookup", nr, nc);
			for (size_t i = 0; i<nr*nc; i++)
				ssc_fm[i] = (ssc_number_t)fm[i];
		}

		if (double *etam = get_unit_value(type_hel_field, "eta_map", &nr, &nc))
		{
			ssc_number_t *ssc_etam = allocate("eff_lookup", nr, nc);
			for (size_t i = 0; i<nr*nc; i++)
				ssc_etam[i] = (ssc_number_t)etam[i];
		}

		if (double *fpm = get_unit_value(type_hel_field, "flux_positions", &nr, &nc))
		{
			ssc_number_t *ssc_fpm = allocate("sunpos_eval", nr, nc);
			for (size_t i = 0; i<nr*nc; i++)
				ssc_fpm[i] = (ssc_number_t)fpm[i];
		}
		assign("land_area", var_data((ssc_number_t)get_unit_value_number(type_hel_field, "land_area")));
		//-----------

		accumulate_annual("hourly_energy", "annual_energy"); // already in kWh

		// performance adjustement factors
		adjustment_factors haf(this);
		if (!haf.setup())
			throw exec_error("tcsmolten_salt", "failed to setup adjustment factors: " + haf.error());
		// hourly_energy output
		ssc_number_t *p_hourly_energy = allocate("hourly_energy", 8760);
		// set hourly energy = tcs output Enet
		size_t count;
		ssc_number_t *hourly_energy = as_array("P_out_net", &count);//MWh
		if (count != 8760)
			throw exec_error("tcsmolten_salt", "hourly_energy count incorrect (should be 8760): " + count);
		// apply performance adjustments and convert from MWh to kWh
		for (size_t i = 0; i < count; i++)
			p_hourly_energy[i] = hourly_energy[i] * (ssc_number_t)(haf(i) * 1000.0);

		accumulate_annual("hourly_energy", "annual_energy"); // already in kWh

		// metric outputs moved to technology
		double kWhperkW = 0.0;
		double nameplate = as_double("system_capacity");
		double annual_energy = 0.0;
		for (int i = 0; i < 8760; i++)
			annual_energy += p_hourly_energy[i];
		if (nameplate > 0) kWhperkW = annual_energy / nameplate;
		assign("capacity_factor", var_data((ssc_number_t)(kWhperkW / 87.6)));
		assign("kwh_per_kw", var_data((ssc_number_t)kWhperkW));

		double fuel_usage_mmbtu = 0;
		ssc_number_t *hourly_fuel = as_array("q_aux_fuel", &count);//MWh
		if (count != 8760)
			throw exec_error("tcsmolten_salt", "q_aux_fuel count incorrect (should be 8760): " + count);
		for (size_t i = 0; i < count; i++)
			fuel_usage_mmbtu += hourly_fuel[i];
		assign("system_heat_rate", 3.413); // samsim tcstrough_physical
		// www.unitjuggler.com/convert-energy-from-MMBtu-to-kWh.html
		assign("annual_fuel_usage", var_data((ssc_number_t)(fuel_usage_mmbtu * 293.297)));


	}

};

DEFINE_TCS_MODULE_ENTRY( tcsdirect_steam, "CSP model using the direct steam power tower TCS types.", 4 )