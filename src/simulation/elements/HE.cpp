#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_HE PT_HE 196
Element_HE::Element_HE()
{
	Identifier = "DEFAULT_PT_HE";
	Name = "HE";
	Colour = PIXPACK(0xFF8210);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 0;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Helium. He-4/100";
	
	State = ST_GAS;
	Properties = TYPE_GAS | PROP_RADIOACTIVE | PROP_CONDUCTS;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_HE::update;
	
}

//#TPT-Directive ElementHeader Element_HE static int update(UPDATE_FUNC_ARGS)
int Element_HE::update(UPDATE_FUNC_ARGS)
 {
	if (!sim->legacy_enable && sim->pv[y/CELL][x/CELL]>0.0f)
	{
		if (parts[i].temp == MIN_TEMP)
		{
			parts[i].temp += .01f;
		}
		else
		{
			float atemp = parts[i].temp + (-MIN_TEMP);
			parts[i].temp = restrict_flt((atemp*(1+(sim->pv[y/CELL][x/CELL]/2000)))+MIN_TEMP, MIN_TEMP, MAX_TEMP);
		}
	}
	return 0;
}


Element_HE::~Element_HE() {}
