#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_TLRY PT_TLRY 190
Element_TLRY::Element_TLRY()
{
	Identifier = "DEFAULT_PT_TLRY";
	Name = "TLRY";
	Colour = PIXPACK(0x484848);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.9f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 2;
	Hardness = 2;
	
	Weight = 50;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 211;
	Description = "Artillery Shell. Explodes.";
	
	Properties = TYPE_PART|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_TLRY::update;
	
}

//#TPT-Directive ElementHeader Element_TLRY static int update(UPDATE_FUNC_ARGS)
int Element_TLRY::update(UPDATE_FUNC_ARGS)
 {
int r, rx, ry, sx, sy;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (parts[r>>8].type!=PT_TLRY && parts[r>>8].type!=PT_FIRE && parts[r>>8].type!=PT_PLSM)
				{
					for (sx=-5; sx<6; sx++)
						for (sy=-5; sy<6; sy++)
							if ((rand()%10)>=3)
							{
								sim->create_part(-1, x+sx, y+sy, PT_FIRE);
							}
							else
							{
								sim->create_part(-1, x+sx, y+sy, PT_PLSM);
							}
							
					sim->create_part(i, x, y, PT_FIRE);
					sim->pv[y/CELL][x/CELL] = 50;
				}
				
					
			}
	return 0;
}


Element_TLRY::~Element_TLRY() {}
