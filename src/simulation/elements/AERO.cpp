#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_AERO PT_AERO 186
Element_AERO::Element_AERO()
{
	Identifier = "DEFAULT_PT_AERO";
	Name = "AERO";
	Colour = PIXPACK(0x8AA2A1);//0x8AA2A1
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.5f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = -0.0000001f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0.5;

	Weight = 100;

	Temperature = R_TEMP + 0.0f + 273.15f;
	HeatConduct = 0;
	Description = "Aerogel. Powerful heat insulator.";

	Properties = TYPE_SOLID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1473.15f;
	HighTemperatureTransition = ST;

	Update = &Element_AERO::update;
	Graphics = &Element_AERO::graphics;

}

//#TPT-Directive ElementHeader Element_AERO static int update(UPDATE_FUNC_ARGS)
int Element_AERO::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, give_temp;
	if (parts[i].temp > 1473.15)
	{
		sim->part_change_type(i, x, y, PT_GEL);
	}
	for (rx = -1; rx < 2; rx++)
		for (ry = -1; ry < 2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y + ry][x + rx];
				if (!r)
					continue;
					
				if (parts[r>>8].type==PT_ARAY || parts[r>>8].type==PT_GPMP || parts[r>>8].type==PT_INSL || (parts[r>>8].type==PT_HSWC && parts[r>>8].life==0)) //Stop conduction to non-conducting elements
					continue;
					
				if (parts[i].temp - parts[r>>8].temp != 0)
				{
					give_temp = ((parts[i].temp - parts[r>>8].temp) / 300);
					parts[i].temp = parts[i].temp - give_temp;
					parts[r>>8].temp = parts[r>>8].temp + give_temp;
				}
			}
	return 0;
}




//#TPT-Directive ElementHeader Element_AERO static int graphics(GRAPHICS_FUNC_ARGS)
int Element_AERO::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 20;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;
	*pixel_mode |= FIRE_ADD;
	return 1;
}


Element_AERO::~Element_AERO() {}
