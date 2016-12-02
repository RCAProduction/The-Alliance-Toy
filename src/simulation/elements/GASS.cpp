#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GASS PT_GASS 220
Element_GASS::Element_GASS()
{
	Identifier = "DEFAULT_PT_GASS";
	Name = "GASS";
	Colour = PIXPACK(0x82301E);
	MenuVisible = 0;
	MenuSection = SC_GAS;
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

	Weight = 1;

	Temperature = R_TEMP+2.0f	+273.15f;
	HeatConduct = 106;
	Description = "NA";

	Properties = TYPE_GAS|PROP_CONDUCTS|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_GASS::update;
}

//#TPT-Directive ElementHeader Element_GASS static int update(UPDATE_FUNC_ARGS)
int Element_GASS::update(UPDATE_FUNC_ARGS)
{
if(parts[i].temp<=parts[i].tmp4)
{
	parts[i].type=PT_LAVA;
}
	return 0;
}


Element_GASS::~Element_GASS() {}
