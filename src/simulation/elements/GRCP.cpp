#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GRCP PT_GRCP 183
Element_GRCP::Element_GRCP()
{
	Identifier = "DEFAULT_PT_GRCP";
	Name = "GRCP";
	Colour = PIXPACK(0x6F636E);
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
	Hardness = 500;
	
	Weight = 325;
	
	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 5;
	Description = "Graphene Composite. Extremely strong.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 200;
	HighPressureTransition = PT_CRBN;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = NT;
	HighTemperatureTransition = NT;
	
	Update = NULL;
	
}

//#TPT-Directive ElementHeader Element_GRCP static int update(UPDATE_FUNC_ARGS)
int Element_GRCP::update(UPDATE_FUNC_ARGS)
{
return 0;
}
Element_GRCP::~Element_GRCP() {}
