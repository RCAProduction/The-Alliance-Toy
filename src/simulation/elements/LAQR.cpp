#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_LAQR PT_LAQR 185
Element_LAQR::Element_LAQR()
{
	Identifier = "DEFAULT_PT_LAQR";
	Name = "LAQR";
	Colour = PIXPACK(0xB41D0049);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.09f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.05f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.05f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;
	
	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;
	
	Weight = 25;
	
	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 5;
	Description = "Lacquer. Combine with CRBN under pressure and heat to make CBNF.";
	
	State = ST_LIQUID;
	Properties = TYPE_LIQUID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = NT;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = NT;
	HighTemperatureTransition = NT;
	
	Update = NULL;
	
}

//#TPT-Directive ElementHeader Element_LAQR static int update(UPDATE_FUNC_ARGS)
int Element_LAQR::update(UPDATE_FUNC_ARGS)
{
return 0;
}
Element_LAQR::~Element_LAQR() {}
