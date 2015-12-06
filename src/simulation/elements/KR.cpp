#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_KR PT_KR 195
Element_KR::Element_KR()
{
	Identifier = "DEFAULT_PT_KR";
	Name = "KR";
	Colour = PIXPACK(0xB98D9C);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 3;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Krypton. Kr-78/.35 Kr-86/17.3 Kr-84/57.0 Kr-82/11.58 Kr-83/13.8";
	
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
	
	Update = &Element_KR::update;
	
}

//#TPT-Directive ElementHeader Element_KR static int update(UPDATE_FUNC_ARGS)
int Element_KR::update(UPDATE_FUNC_ARGS)
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


Element_KR::~Element_KR() {}
