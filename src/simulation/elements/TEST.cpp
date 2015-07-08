#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_TEST PT_TEST 255
Element_TEST::Element_TEST()
{
	Identifier = "DEFAULT_PT_TEST";
	Name = "TEST";
	Colour = PIXPACK(0xFF0000);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 10;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;
	
	Weight = 85;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 70;
	Description = "TEST, use for testing code, etc.";
	
	State = ST_SOLID;
	Properties = TYPE_PART|PROP_LIFE_DEC|PROP_LIFE_KILL_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_TEST::update;
	Graphics = NULL;
}
//#TPT-Directive ElementHeader Element_TEST static int update(UPDATE_FUNC_ARGS)
int Element_TEST::update(UPDATE_FUNC_ARGS)
{
parts[i].x = 200;
	return 0;
}
Element_TEST::~Element_TEST() {}
