#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_BLLT PT_BLLT 219
Element_BLLT::Element_BLLT()
{
	Identifier = "DEFAULT_PT_BLLT";
	Name = "BLLT";
	Colour = PIXPACK(0xA9724B);
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

	Flammable = 0;
	Explosive = 0;
	Meltable = 2;
	Hardness = 2;

	Weight = 90;

	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 1;
	Description = "Bullet. When SPRKed, fires similar to ARAY.";

	Properties = TYPE_SOLID|PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_BLLT::update;
}

//#TPT-Directive ElementHeader Element_BLLT static int update(UPDATE_FUNC_ARGS)
int Element_BLLT::update(UPDATE_FUNC_ARGS)
{
	if (!parts[i].life)
	{
		for (int rx = -1; rx <= 1; rx++)
			for (int ry = -1; ry <= 1; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					int r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if (parts[r>>8].type==PT_SPRK)
					{
						parts[i].type=PT_EMBR;
						parts[i].life=100;
						parts[i].temp=6000;
						parts[i].vx=(rx*(-1))*100;
						parts[i].vy=(ry*(-1))*100;
					}
					else if (parts[i].type==PT_BLLT && parts[i].temp>=1000)
					{
						parts[i].temp=6000;
						parts[i].type=PT_EMBR;
						parts[i].life=200;
						sim->pv[y/CELL][x/CELL] = 5;
						parts[r>>8].temp=500;
					}
		}
	}
	return 0;
}


Element_BLLT::~Element_BLLT() {}
