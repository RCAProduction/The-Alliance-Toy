#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CRBN PT_CRBN 182
Element_CRBN::Element_CRBN()
{
	Identifier = "DEFAULT_PT_CRBN";
	Name = "CRBN";
	Colour = PIXPACK(0x464646);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;
	
	Weight = 30;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 211;
	Description = "Carbon. A basic building block for many composites.";
	
	State = ST_SOLID;
	Properties = TYPE_PART|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = NT;
	HighTemperatureTransition = NT;
	
	Update = NULL;
	
}

//#TPT-Directive ElementHeader Element_CRBN static int update(UPDATE_FUNC_ARGS)
int Element_CRBN::update(UPDATE_FUNC_ARGS)
{
return 0;
}
Element_CRBN::~Element_CRBN() {}
