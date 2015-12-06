#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_H PT_H 197
Element_H::Element_H()
{
	Identifier = "DEFAULT_PT_H";
	Name = "H";
	Colour = PIXPACK(0x707020);
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
	Description = "Hydrogen. H-1/100 H-2/0 H-3/0";
	
	State = ST_GAS;
	Properties = TYPE_GAS | PROP_RADIOACTIVE;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_H::update;
	
}

//#TPT-Directive ElementHeader Element_H static int update(UPDATE_FUNC_ARGS)
int Element_H::update(UPDATE_FUNC_ARGS)
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


Element_H::~Element_H() {}
