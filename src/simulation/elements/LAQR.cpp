#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_LAQR PT_LAQR 185
Element_LAQR::Element_LAQR()
{
	Identifier = "DEFAULT_PT_LAQR";
	Name = "LAQR";
	Colour = PIXPACK(0xB41D0049);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.01f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.05f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;
	
	Flammable = 1;
	Explosive = 0;
	Meltable = 0;
	Hardness = 2;
	
	Weight = 25;
	
	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 5;
	Description = "Lacquer. Combine with CRBN under pressure and heat to make CBNF.";
	
	State = ST_LIQUID;
	Properties = TYPE_LIQUID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = NT;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1500.00f;
	HighTemperatureTransition = PT_BGLA;
	
	Update = &Element_LAQR::update;
	
}

//#TPT-Directive ElementHeader Element_LAQR static int update(UPDATE_FUNC_ARGS)
int Element_LAQR::update(UPDATE_FUNC_ARGS)
{
int r, rx, ry;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK) {
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if(parts[r>>8].type==PT_CRBN && parts[i].temp>=600 && parts[r>>8].temp>=600 && sim->pv[y/CELL][x/CELL]>=15)
				{
					sim->part_change_type(r>>8,x,y,PT_CBNF);
					sim->part_change_type(i,x,y,PT_CBNF);
				}
			}
	return 0;
}
Element_LAQR::~Element_LAQR() {}
