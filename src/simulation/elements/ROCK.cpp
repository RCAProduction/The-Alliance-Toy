#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_ROCK PT_ROCK 216
Element_ROCK::Element_ROCK()
{
	Identifier = "DEFAULT_PT_ROCK";
	Name = "ROCK";
	Colour = PIXPACK(0x727272);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.00f;
	Collision = -0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 5;
	Hardness = 70;

	Weight = 120;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 150;
	Description = "Rock. CNCT can stack on top of it.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 130;
	HighPressureTransition = PT_STNE;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 983.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &Element_ROCK::update;;
	Graphics = &Element_ROCK::graphics;
}

//#TPT-Directive ElementHeader Element_ROCK static int update(UPDATE_FUNC_ARGS)
int Element_ROCK::update(UPDATE_FUNC_ARGS)
{
	if (parts[i].tmp2<=0)
	{
		parts[i].tmp2=(rand() % 5);
	}
	return 0;
}

//#TPT-Directive ElementHeader Element_ROCK static int graphics(GRAPHICS_FUNC_ARGS)
int Element_ROCK::graphics(GRAPHICS_FUNC_ARGS)

{
	int z = (cpart->tmp2) * 7;//speckles!
	*colr += z;
	*colg += z;
	*colb += z;
	return 0;
}


Element_ROCK::~Element_ROCK() {}
