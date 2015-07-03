#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CBNF PT_CBNF 184
Element_CBNF::Element_CBNF()
{
	Identifier = "DEFAULT_PT_CBNF";
	Name = "CBNF";
	Colour = PIXPACK(0x2B2A39);
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
	Description = "Carbon Fiber.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID;
	
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

//#TPT-Directive ElementHeader Element_CBNF static int update(UPDATE_FUNC_ARGS)
int Element_CBNF::update(UPDATE_FUNC_ARGS)
{
sim->kill_part(i);
/*if(sim->pv[y/CELL][x/CELL]>=2 || parts[i].temp>=300)
{
	sim->part_change_type(i,x,y,PT_CRBN);
}*/
return 0;
}
Element_CBNF::~Element_CBNF() {}
