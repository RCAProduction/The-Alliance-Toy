#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_BA PT_BA 198
Element_BA::Element_BA()
{
	Identifier = "DEFAULT_PT_BA";
	Name = "BA";
	Colour = PIXPACK(0x798898);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 90;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Barium. Ba-138/71.698 Ba-141/0 Ba-137/11.232 Ba-136/7.854 Ba-135/6.592";
	
	State = ST_SOLID;
	Properties = TYPE_PART | PROP_RADIOACTIVE;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_BA::update;
	
}

//#TPT-Directive ElementHeader Element_BA static int update(UPDATE_FUNC_ARGS)
int Element_BA::update(UPDATE_FUNC_ARGS)
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


Element_BA::~Element_BA() {}
