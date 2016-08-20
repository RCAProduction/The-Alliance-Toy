#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_EQB2 PT_EQB2 188
Element_EQB2::Element_EQB2()
{
	Identifier = "DEFAULT_PT_EQB2";
	Name = "EQB2";
	Colour = PIXPACK(0xFECB67);
	MenuVisible = 0;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.3f;
	AirDrag = 0.02f;
	AirLoss = 0.98f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.15f;
	Diffusion = 0.00f;
	HotAir = 0.000f;
	Falldown = 2;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;
	Weight = 20;
	
	Temperature = R_TEMP+20.0f+273.15f;
	HeatConduct = 164;
	Description = "EQB2 - Equilibrium test element 2";
	
	Properties = TYPE_LIQUID|PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_EQB2::update;
	Graphics = &Element_EQB2::graphics;
}

//#TPT-Directive ElementHeader Element_EQB2 static int update(UPDATE_FUNC_ARGS)
int Element_EQB2::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, some_variable;
	int ave_temp;
	for (rx = -1; rx<2; rx++)
	for (ry = -1; ry<2; ry++)
	if (BOUNDS_CHECK && (rx || ry))
	{
		r = pmap[y + ry][x + rx];
		if (!r)
			continue;
		if (parts[r>>8].type == 188)
		{
			ave_temp = 10001-((parts[i].temp + parts[r>>8].temp) / 2);
			//if a random number from 1 to f(average temp) = 1 and touching EQBM then set both types to EQB2 and increase temperature.
			some_variable = (ceil(0.0001*(ave_temp)));
			{
				if (!floor(rand() % (some_variable)))
				{
					sim->part_change_type(r>>8, x + rx, y + ry, 187);
					sim->part_change_type(i, x, y, 187);
					parts[i].temp-=10;
					parts[r>>8].temp-=10;
				}
			}
		}
	}
	return 0;
}
//#TPT-Directive ElementHeader Element_EQB2 static int graphics(GRAPHICS_FUNC_ARGS)
int Element_EQB2::graphics(GRAPHICS_FUNC_ARGS)
{
	return 0;
}

Element_EQB2::~Element_EQB2() {}
