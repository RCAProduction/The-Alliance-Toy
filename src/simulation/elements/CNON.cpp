#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CNON PT_CNON 191
Element_CNON::Element_CNON()
{
	Identifier = "DEFAULT_PT_CNON";
	Name = "CNON";
	Colour = PIXPACK(0x848484);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f+273.15f;
	HeatConduct = 186;
	Description = "Cannon. Aim with mouse.";
	
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_CNON::update;
	
}
//#TPT-Directive ElementHeader Element_CNON static int update(UPDATE_FUNC_ARGS)
int Element_CNON::update(UPDATE_FUNC_ARGS)
{

	return 0;
}

Element_CNON::~Element_CNON() {}
