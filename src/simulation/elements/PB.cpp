#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PB PT_PB 211
Element_PB::Element_PB()
{
	Identifier = "DEFAULT_PT_PB";
	Name = "PB";
	Colour = PIXPACK(0x6d6a65);
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
	Hardness = 0;

	Weight = 100;

	Temperature = R_TEMP + 0.0f + 273.15f;
	HeatConduct = 251;
	Description = "Pb 82, lead. ";

	Properties = TYPE_SOLID | PROP_CONDUCTS | PROP_LIFE_DEC;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 600.0f;
	HighTemperatureTransition = PT_LAVA;

	Update = &Element_PB::update;
}
#define ADVECTION 0.1f
//#TPT-Directive ElementHeader Element_PB static int update(UPDATE_FUNC_ARGS)
int Element_PB::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, vx, vy;
	parts[i].tmp = 0;
	//Set destination.
	parts[i].vx += ADVECTION*sim->vx[y / CELL][x / CELL];
	parts[i].vy += ADVECTION*sim->vy[y / CELL][x / CELL];
	//Check if any PB is disconnected from 2+ PB if I move to destination. set tmp2 to 1 if so.
	for (rx = -1; rx < 2; rx++)
	for (ry = -1; ry < 2; ry++)
	if (BOUNDS_CHECK && (rx || ry))
	{
		r = pmap[y + ry][x + rx];
		if (!r) continue;
		if (parts[r>>8].type == PT_PB)
		{
			parts[i].tmp++;
		}
		if (parts[r>>8].type == PT_ACID)
		{
			parts[i].type = PT_SPRK;
			parts[i].ctype = PT_PB;
			parts[i].life = 8;
		}
		if (parts[r>>8].type == PT_O2 && rand()%20==1)
		{
			sim->part_change_type(i, x, y, PT_BMTL);
			parts[i].tmp = (rand() % 10) + 20;
		}
	}
	//If tmp is greater than or equal to 5, do not move.
	if (parts[i].tmp >= 5)
	{
		parts[i].vx = 0;
		parts[i].vy = 0;
	}
	return 0;
}
Element_PB::~Element_PB() {}
