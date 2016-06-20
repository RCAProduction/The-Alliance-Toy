#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_EQBM PT_EQBM 187
Element_EQBM::Element_EQBM()
{
	Identifier = "DEFAULT_PT_EQBM";
	Name = "EQBM";
	Colour = PIXPACK(0xA6FFB4);
	MenuVisible = 1;
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
	
	Weight = 10;
	
	Temperature = R_TEMP+20.0f+273.15f;
	HeatConduct = 164;
	Description = "EQBM - Equilibrium test element 1";

	Properties = TYPE_LIQUID|PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_EQBM::update;
	Graphics = &Element_EQBM::graphics;
}

//#TPT-Directive ElementHeader Element_EQBM static int update(UPDATE_FUNC_ARGS)
int Element_EQBM::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, someVariable;
	int ave_temp;
	for (rx = -1; rx<2; rx++)
	for (ry = -1; ry<2; ry++)
	if (BOUNDS_CHECK && (rx || ry))
	{
		r = pmap[y + ry][x + rx];
		if (!r)
			continue;
		if (parts[r>>8].type == 187)
		{
			ave_temp = (parts[i].temp + parts[r>>8].temp) / 2;
			//if a random number from 1 to f(average temp) = 1 and touching EQBM then set both types to EQB2 and increase temperature.
			someVariable = (floor(ave_temp/10));
			if (ave_temp > 50)
			{
				if (floor(rand() % (someVariable)) == 1)
				{
					sim->part_change_type(r>>8, x + rx, y + ry, 188);
					sim->part_change_type(i, x, y, 188);
					parts[i].temp++;
					parts[r >> 8].temp++;
					parts[i].tmp = ave_temp;
				}
			}
		}
	}
	return 0;
}
//#TPT-Directive ElementHeader Element_EQBM static int graphics(GRAPHICS_FUNC_ARGS)
int Element_EQBM::graphics(GRAPHICS_FUNC_ARGS)
{
	return 0;
}

Element_EQBM::~Element_EQBM() {}
