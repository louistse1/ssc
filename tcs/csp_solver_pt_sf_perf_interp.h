#ifndef __csp_solver_pt_heliostatfield_
#define __csp_solver_pt_heliostatfield_

#include "csp_solver_util.h"
#include "csp_solver_core.h"

#include "sort_method.h"
#include "interpolation_routines.h"
#include "AutoPilot_API.h" 
#include "IOUtil.h"


class C_pt_sf_perf_interp
{
private:
	// Class Instances
	GaussMarkov *field_efficiency_table;
	MatDoub m_flux_positions;
	//sp_flux_table fluxtab;
	
	double m_p_start;				//[kWe-hr] Heliostat startup energy
	double m_p_track;				//[kWe] Heliostat tracking power
	double m_hel_stow_deploy;		//[rad] converted from [deg] in ms_params
	double m_v_wind_max;			//[m/s]

	int m_n_flux_x;		//[-]
	int m_n_flux_y;		//[-]
	int m_N_hel;		//[-]

	//Stored Variables
	double m_eta_prev;			//[-]
	double m_v_wind_prev;		//[m/s]
	double m_v_wind_current;	//[m/s]

	// member string for exception messages
	std::string error_msg;

	double rdist(VectDoub *p1, VectDoub *p2, int dim = 2);

	// track number of calls per timestep, reset = -1 in converged() call
	int m_ncall;

public:
	// Class to save messages for up stream classes
	C_csp_messages mc_csp_messages;

	C_pt_sf_perf_interp();

	~C_pt_sf_perf_interp();

	struct RUN_TYPE { enum A {AUTO, USER_FIELD, USER_DATA}; };

	// Callback funtion
	bool(*mf_callback)(simulation_info* siminfo, void *data);
	void *m_cdata;

	struct S_params
	{
        bool m_eta_map_aod_format;
		int m_run_type;
		double m_helio_width;				//[m]
		double m_helio_height;				//[m]
		double m_dens_mirror;

		double m_p_start;			//[kWe-hr] Heliostat startup energy
		double m_p_track;			//[kWe] Heliostat tracking power
		double m_hel_stow_deploy;	//[deg] convert to [rad] in init()
		double m_v_wind_max;		//[m/s] max wind speed
		double m_interp_nug;
		double m_interp_beta;

		int m_n_flux_x;
		int m_n_flux_y;

		util::matrix_t<double> m_helio_positions;

		//double *m_eta_map;
		//int m_nrows_eta_map;
		//int m_ncols_eta_map;
		util::matrix_t<double> m_eta_map;

		//double *m_flux_positions;
		//int m_nfluxpos;
		//int m_nfposdim;
		util::matrix_t<double> m_flux_positions;

		//double *m_flux_maps;
		//int m_nfluxmap;
		//int m_nfluxcol;
		util::matrix_t<double> m_flux_maps;

        util::matrix_t<double> m_sf_adjust; // array of length equal to number of time steps

		double m_land_area;

		double m_A_sf;		//[m2]

		S_params()
		{
			// Integers
			m_run_type = m_n_flux_x = m_n_flux_y = -1;

			// Doubles
			m_helio_width = m_helio_height = m_dens_mirror = 
				m_p_start = m_p_track = m_hel_stow_deploy = m_v_wind_max = m_interp_nug =
				m_interp_beta = m_land_area = m_A_sf = std::numeric_limits<double>::quiet_NaN();

		}		
	};

	S_params ms_params;

	struct S_outputs
	{
		double m_q_dot_field_inc;	//[MWt] Field incident thermal power (from the sun!)

		util::matrix_t<double> m_flux_map_out;
		double m_pparasi;		//[MWe]
		double m_eta_field;		//[-]
        double m_sf_adjust_out;

		S_outputs()
		{
			m_q_dot_field_inc = m_pparasi = m_eta_field = m_sf_adjust_out =  std::numeric_limits<double>::quiet_NaN();
		}
	};

	S_outputs ms_outputs;

	//void init(bool(*callback)(simulation_info* siminfo, void *data), void *cdata);
	void init();

	void call(const C_csp_weatherreader::S_outputs &weather, 
		double field_control_in, 
		const C_csp_solver_sim_info &sim_info);

	void off(const C_csp_solver_sim_info &sim_info);

	void converged();
};






#endif
