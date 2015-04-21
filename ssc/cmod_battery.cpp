#include <math.h>

#include "common.h"
#include "core.h"
#include "lib_util.h"
#include "cmod_battery.h"




var_info vtab_battery[] = {
/*   VARTYPE           DATATYPE         NAME                                            LABEL                                                   UNITS      META                             GROUP                  REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	
	// generic battery inputs
	{ SSC_INPUT,        SSC_NUMBER,      "batt_computed_parallel",                     "Number of Cells in Parallel",                             "",        "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_computed_series",                       "Number of Cells in Serial",                               "",        "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_chem",                                  "Battery chemistry",                                       "",        "0=LeadAcid,1=LiIon",   "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,		 "batt_bank_size",                             "Battery bank desired size",                               "kWh",     "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_minimum_SOC",		                   "Minimum allowed state-of-charge",                         "V",       "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_current_charge_max",                    "Maximum charge current",                                  "A",       "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_current_discharge_max",                 "Maximum discharge current",                               "A",       "",                     "Battery",       "",                           "",                              "" },


	// Voltage discharge curve
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Vfull",                                 "Fully charged cell voltage",                              "V",       "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Vexp",                                  "Cell Voltage at end of exponential zone",                 "V",       "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Vnom",                                  "Cell Voltage at end of nominal zone",                     "V",       "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Qfull",                                 "Fully charged cell capacity",                             "Ah",      "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Qexp",                                  "Cell capacity at end of exponential zone",                "Ah",      "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_Qnom",                                  "Cell capacity at end of nominal zone",                    "Ah",      "",                     "Battery",       "",                           "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,      "batt_C_rate",                                "Rate at which voltage vs. capacity curve input",          "",        "",                     "Battery",       "",                           "",                              "" },

	// lead-acid inputs
	{ SSC_INPUT,		SSC_NUMBER,		"LeadAcid_q20_computed",	                   "Capacity at 20-hour discharge rate",                     "Ah",       "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,		SSC_NUMBER,		"LeadAcid_q10_computed",	                   "Capacity at 10-hour discharge rate",                     "Ah",       "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,		SSC_NUMBER,		"LeadAcid_qn_computed",	                       "Capacity at discharge rate for n-hour rate",             "Ah",       "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,		SSC_NUMBER,		"LeadAcid_tn",	                               "Time to discharge",                                      "h",        "",                     "Battery",       "",                           "",                             "" },
																																																						     
	// lithium-ion inputs (currently none specific to LiIon)

	// lifetime inputs
	{ SSC_INPUT,		SSC_MATRIX,     "batt_lifetime_matrix",                        "Cycles vs capacity at different depths-of-discharge",    "",         "",                     "Battery",       "",                           "",                             "" },

	// thermal inputs
	{ SSC_INPUT,        SSC_NUMBER,     "batt_mass",                                   "Mass of the battery",                                    "kg",       "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "batt_length",                                 "Battery length",                                         "m",        "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "batt_width",                                  "Battery width",                                          "m",        "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "batt_height",                                 "Battery height",                                         "m",        "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "batt_Cp",                                     "Battery specific heat capacity",                         "J/KgK",    "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "batt_h_to_ambient",                           "Heat transfer between battery and environment",          "W/m2K",    "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_NUMBER,     "T_room",                                      "Temperature of storage room",                            "C",        "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_MATRIX,     "cap_vs_temp",                                 "Effective capacity as function of temperature",          "C,%",      "",                     "Battery",       "",                           "",                             "" },

	// storage dispatch
	{ SSC_INPUT,        SSC_ARRAY,      "dispatch_manual_charge",                      "Periods 1-4 Charging Allowed?",                          "",         "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_ARRAY,      "dispatch_manual_discharge",                   "Periods 1-4 Discharging Allowed?",                       "",         "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_ARRAY,      "dispatch_manual_gridcharge",                  "Periods 1-4 Grid Charging Allowed?",                     "",         "",                     "Battery",       "",                           "",                             "" },
	{ SSC_INPUT,        SSC_MATRIX,     "dispatch_manual_sched",                       "Battery Dispatch Schedule",                              "",         "",                     "Battery",       "",                           "",                             "" },
		


// battery/storage outputs, currently allows all subhourly.  May want to acculumulate up to hourly for most 
	{ SSC_OUTPUT,        SSC_ARRAY,      "batt_q0",                              "Battery Total Charge",                                  "Ah",    "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "batt_q1",                              "Battery Available Charge",                              "Ah",    "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "batt_q2",                              "Battery Bound Charge",                                  "Ah",    "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "SOC",                                  "Battery State of Charge",                               "%",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "DOD",                                  "Battery Cycle Depth of Discharge",                      "%",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "qmaxI",                                "Battery Max Capacity at Current",                       "Ah",    "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "qmax",                                 "Battery Max Charge",                                    "Ah",    "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "batt_I",                               "Battery Current",                                       "A",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "voltage_cell",                         "Battery Cell Voltage",                                  "V",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "voltage_battery",                      "Battery Voltage",	                                      "V",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "Cycles",                               "Battery Number of Cycles",                              "",      "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "forty_percent_cycles",                 "Battery Number of 40% DOD Cycles",                      "",      "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "hundred_percent_cycles",               "Battery Number of 100% DOD Cycles",                     "",      "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "battery_temperature",                  "Battery Temperature",                                   "C",     "", "Battery", "", "", "" }, 
	{ SSC_OUTPUT,        SSC_ARRAY,      "capacity_percent",                     "Battery Capacity Percent for Lifetime",                 "%",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "capacity_thermal_percent",             "Battery Capacity Percent for Temperature",              "%",     "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "battery_energy",                       "Energy to/from Battery",                                "kWh",   "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "grid_energy",                          "Energy to/from Grid",                                   "kWh",   "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "pv_to_load",                           "Energy to load from PV",                                "kWh",   "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "battery_to_load",                      "Energy to load from battery",                           "kWh",   "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "grid_to_load",                         "Energy to load from grid",                              "kWh",   "", "Battery", "", "", "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "average_cycle_efficiency",             "Battery Cycle Efficiency",                              "%",     "", "Annual", "*", "", "" },
	
var_info_invalid };




battstor::battstor( compute_module &cm, bool setup_model, size_t nrec, double dt_hr )
{
	voltage_model = 0;
	lifetime_model = 0;
	thermal_model = 0;
	battery_model = 0;
	capacity_model = 0;
	dispatch_model = 0;
	losses_model = 0;

	outTotalCharge = 0;
	outAvailableCharge = 0;
	outBoundCharge = 0;
	outMaxChargeAtCurrent = 0;
	outMaxCharge = 0;
	outSOC = 0;
	outDOD = 0;
	outCurrent = 0;
	outCellVoltage = 0;
	outBatteryVoltage = 0;
	outCapacityPercent = 0;
	outCycles = 0;
	out40Cycles = 0;
	out100Cycles = 0;
	outBatteryTemperature = 0;
	outCapacityThermalPercent = 0;
	outBatteryEnergy = 0;
	outGridEnergy = 0;
	outPVToLoad = 0;
	outBatteryToLoad = 0;
	outGridToLoad = 0;
	outAverageCycleEfficiency = 0;


	en = setup_model;
	if ( !en ) return;

	chem = cm.as_integer( "batt_chem" );

	size_t ncharge, ndischarge, ngridcharge;
	ssc_number_t *pcharge = cm.as_array( "dispatch_manual_charge", &ncharge );
	ssc_number_t *pdischarge = cm.as_array( "dispatch_manual_discharge", &ndischarge );
	ssc_number_t *pgridcharge = cm.as_array( "dispatch_manual_gridcharge", &ngridcharge );
	if ( ncharge != 4 || ndischarge != 4 || ngridcharge != 4 )
		throw compute_module::exec_error("battery", "invalid manual dispatch control vector lengths");

	for( size_t i=0;i<4;i++ )
	{
		dm_charge[i] = pcharge[i]!=0.0f ? 1 : 0;
		dm_discharge[i] = pdischarge[i]!=0.0f ? 1 : 0;
		dm_gridcharge[i] = pgridcharge[i]!=0.0f ? 1 : 0;
	}
	size_t m,n;
	ssc_number_t *psched = cm.as_matrix("dispatch_manual_sched", &m, &n);
	if ( m != 12 || n != 24 )
		throw compute_module::exec_error("battery", "invalid manual dispatch schedule matrix dimensions, must be 12 x 24" );

	for( size_t i=0;i<12;i++ )
		for( size_t j=0;j<24;j++ )
			dm_sched(i,j) = psched[ i*24 + j ];
	
	util::matrix_t<double>  batt_lifetime_matrix = cm.as_matrix("batt_lifetime_matrix");
	if (batt_lifetime_matrix.nrows() < 3 || batt_lifetime_matrix.ncols() != 3)
		throw compute_module::exec_error("battery", "Battery lifetime matrix must have three columns and at least three rows");

	/* **********************************************************************
	Initialize outputs
	********************************************************************** */		
	outTotalCharge = cm.allocate("batt_q0", nrec);

	// only allocate if lead-acid
	if ( chem == 0 )
	{
		outAvailableCharge = cm.allocate("batt_q1", nrec);
		outBoundCharge = cm.allocate("batt_q2", nrec);
	}
	outMaxCharge = cm.allocate("qmax", nrec);
	outSOC = cm.allocate("SOC", nrec);
	outDOD = cm.allocate("DOD", nrec);
	outCurrent = cm.allocate("batt_I", nrec);
	outCellVoltage = cm.allocate("voltage_cell", nrec);
	outBatteryVoltage = cm.allocate("voltage_battery", nrec);
	outCapacityPercent = cm.allocate("capacity_percent", nrec);
	outCycles = cm.allocate("Cycles", nrec);
	out40Cycles = cm.allocate("forty_percent_cycles", nrec);
	out100Cycles = cm.allocate("hundred_percent_cycles", nrec);
	outBatteryTemperature = cm.allocate("battery_temperature", nrec);
	outCapacityThermalPercent = cm.allocate("capacity_thermal_percent", nrec);
	outBatteryEnergy = cm.allocate("battery_energy", nrec);
	outGridEnergy = cm.allocate("grid_energy", nrec); // Net grid energy required.  Positive indicates putting energy on grid.  Negative indicates pulling off grid
	outPVToLoad = cm.allocate("pv_to_load", nrec);
	outBatteryToLoad = cm.allocate("battery_to_load", nrec);
	outGridToLoad = cm.allocate("grid_to_load", nrec);

	// model initialization
	voltage_model = new voltage_dynamic_t(cm.as_integer("batt_computed_series"), cm.as_double("batt_Vnom"), cm.as_double("batt_Vfull"), cm.as_double("batt_Vexp"),
		cm.as_double("batt_Vnom"), cm.as_double("batt_Qfull"), cm.as_double("batt_Qexp"), cm.as_double("batt_Qnom"), cm.as_double("batt_C_rate"));
	lifetime_model = new  lifetime_t(batt_lifetime_matrix);

	util::matrix_t<double> cap_vs_temp = cm.as_matrix( "cap_vs_temp" );
	if ( cap_vs_temp.nrows() < 2 || cap_vs_temp.ncols() != 2 )
		throw compute_module::exec_error("battery", "capacity vs temperature matrix must have two columns and at least two rows");

	// thermal_outputs = new thermal_outputs_t();
	thermal_model = new thermal_t(
		cm.as_double("batt_mass"), // [kg]
		cm.as_double("batt_length"), // [m]
		cm.as_double("batt_width"), // [m]
		cm.as_double("batt_height"), // [m]
		cm.as_double("batt_Cp"), // [J/kgK]
		cm.as_double("batt_h_to_ambient"), // W/m2K
		273.15 + cm.as_double("T_room"), // K
		cap_vs_temp);
		
		
	battery_model = new battery_t( 
		dt_hr,
		chem);

	double Vfull = cm.as_double("batt_Vfull");
	int ncell = cm.as_integer("batt_computed_series") + cm.as_integer("batt_computed_parallel");

	if ( chem == 0 )
	{
		capacity_model = new capacity_kibam_t(
			cm.as_double( "LeadAcid_q20_computed" ),
			cm.as_double( "LeadAcid_tn" ),
			cm.as_double( "LeadAcid_qn_computed" ),
			cm.as_double("LeadAcid_q10_computed"));
	}
	else if ( chem == 1 )
	{
		capacity_model = new capacity_lithium_ion_t(
			cm.as_double("batt_Qfull") );
	}
	
	losses_model = new losses_t(
		lifetime_model,
		thermal_model,
		capacity_model);

	battery_model->initialize( capacity_model, voltage_model, lifetime_model, thermal_model, losses_model);
	dispatch_model = new dispatch_manual_t(battery_model, dt_hr, cm.as_double("batt_minimum_SOC"), cm.as_double("batt_current_charge_max"), cm.as_double("batt_current_discharge_max"), dm_sched, dm_charge, dm_discharge, dm_gridcharge);
}

battstor::~battstor()
{
	if( voltage_model ) delete voltage_model;
	if( lifetime_model ) delete lifetime_model;
	if( thermal_model ) delete thermal_model;
	if( battery_model ) delete battery_model;
	if( capacity_model ) delete capacity_model;
	if (losses_model) delete losses_model;
	if( dispatch_model ) delete dispatch_model;
}

void battstor::advance( compute_module &cm, size_t idx, size_t hour_of_year, size_t step, double PV, double LOAD )
{
	dispatch_model->dispatch( hour_of_year, PV, LOAD );

	// Capacity Output with Losses Applied
	if (capacity_kibam_t * kibam = dynamic_cast<capacity_kibam_t*>(capacity_model))
	{
		outAvailableCharge[idx] = (ssc_number_t)(kibam->q1());
		outBoundCharge[idx] = (ssc_number_t)(kibam->q2());
	}
	
	outMaxCharge[idx] = (ssc_number_t)(capacity_model->qmax());
	outTotalCharge[idx] = (ssc_number_t)(capacity_model->q0());
	outSOC[idx] = (ssc_number_t)(capacity_model->SOC());
	outCurrent[idx] = (capacity_model->I());

	// Voltage Output
	outCellVoltage[idx] = (ssc_number_t)(voltage_model->cell_voltage() );
	outBatteryVoltage[idx] = (ssc_number_t)(voltage_model->battery_voltage() );

	// Lifetime Output
	outCapacityPercent[idx] = (ssc_number_t)(lifetime_model->capacity_percent());
	outCycles[idx] = (int)(lifetime_model->cycles_elapsed());
	out40Cycles[idx] = (int)(lifetime_model->forty_percent_cycles());
	out100Cycles[idx] = (int)(lifetime_model->hundred_percent_cycles());
	outDOD[idx] = (ssc_number_t)(lifetime_model->cycle_range());

	// Thermal Output
	outBatteryTemperature[idx] = (ssc_number_t)(thermal_model->T_battery()) - 273.15;
	outCapacityThermalPercent[idx] = (ssc_number_t)(thermal_model->capacity_percent()); 

	// Dispatch output
	outBatteryEnergy[idx] = (ssc_number_t)(dispatch_model->energy_tofrom_battery());
	outGridEnergy[idx] = (ssc_number_t)(dispatch_model->energy_tofrom_grid());
	outPVToLoad[idx] = (ssc_number_t)(dispatch_model->pv_to_load() );
	outBatteryToLoad[idx] = (ssc_number_t)(dispatch_model->battery_to_load());
	outGridToLoad[idx] = (ssc_number_t)(dispatch_model->grid_to_load());

	if (outBatteryEnergy[idx] > 0)
		e_discharge += outBatteryEnergy[idx];
	else
		e_charge += (-outBatteryEnergy[idx]);

	outAverageCycleEfficiency = (100.*(e_discharge / e_charge));
}
///////////////////////////////////////////////////
static var_info _cm_vtab_battery[] = {
	/*   VARTYPE           DATATYPE         NAME                      LABEL                              UNITS     META                      GROUP          REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	
	// system energy and load
	{ SSC_INOUT, SSC_ARRAY, "hourly_energy", "Hourly energy", "kWh", "", "Time Series", "*", "", "" },
	{ SSC_INOUT, SSC_ARRAY, "e_load", "Electric load", "kWh", "", "Load Profile Estimator", "", "", "" },

	// other variables come from battstor common table

	var_info_invalid };

class cm_battery : public compute_module
{
public:

	cm_battery()
	{
		add_var_info(_cm_vtab_battery);
		add_var_info( vtab_battery );
	}

	void exec() throw(general_error)
	{
		
		size_t nrec, len;
		ssc_number_t *hourly_energy = as_array("hourly_energy", &nrec);
		ssc_number_t *e_load = as_array("e_load", &len);

		if (len != nrec) 
			throw exec_error("battery", "Load and PV power do not match weatherfile length");

		size_t step_per_hour = nrec / 8760;
		if (step_per_hour < 1 || step_per_hour > 60 || step_per_hour * 8760 != nrec)
			throw exec_error("swh", util::format("invalid number of data records (%d): must be an integer multiple of 8760", (int)nrec));

		double ts_hour = 1.0 / step_per_hour;
		double ts_sec = 3600.0 / step_per_hour;


		battstor batt( *this, true, nrec, ts_hour );

		size_t hour = 0;
		
		/* *********************************************************************************************
		Run Simulation
		*********************************************************************************************** */

		int count = 0;
		for (hour = 0; hour < 8760; hour++)
		{
			// Loop over subhourly
			for (size_t jj = 0; jj<step_per_hour; jj++)
			{
				batt.advance( *this, count, hour, jj, hourly_energy[count], e_load[count] );
				count++;
			}	// End loop over subhourly
		} // End loop over hourly
	}

};

DEFINE_MODULE_ENTRY(battery, "Battery storage standalone model .", 10)