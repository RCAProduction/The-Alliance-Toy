#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_SGLM PT_SGLM 215
Element_SGLM::Element_SGLM()
{
	Identifier = "DEFAULT_PT_SGLM";
	Name = "SGLM";
	Colour = PIXPACK(0xd9d9d9);
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
	Meltable = 1;
	Hardness = 1;

	Weight = 100;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "The basic conductor. Meltable.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_HOT_GLOW;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 283.15;
	HighTemperatureTransition = PT_GALM;

	Update = NULL;
}

Element_SGLM::~Element_SGLM() {}
