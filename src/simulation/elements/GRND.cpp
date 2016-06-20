#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GRND PT_GRND 192
Element_GRND::Element_GRND()
{
	Identifier = "DEFAULT_PT_GRND";
	Name = "GRND";
	Colour = PIXPACK(0x375E34);
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
	Description = "Grenade.";
	
	Properties = TYPE_PART|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_GRND::update;
	
}

//#TPT-Directive ElementHeader Element_GRND static int update(UPDATE_FUNC_ARGS)
int Element_GRND::update(UPDATE_FUNC_ARGS)
 {
int r, rx, ry, sx, sy;

parts[i].tmp3=parts[i].tmp3+1;
if (parts[i].tmp3>=150)
{
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (parts[r>>8].type!=PT_GRND && parts[r>>8].type!=PT_FIRE && parts[r>>8].type!=PT_PLSM)
				{
					for (sx=-5; sx<6; sx++)
						for (sy=-5; sy<6; sy++)
							sim->create_part(-1, x+sx, y+sy, PT_FIRE);
							
					sim->create_part(i, x, y, PT_BRMT);
					sim->pv[y/CELL][x/CELL] = 50;
				}
				
					
			}
	}
	return 0;
}


Element_GRND::~Element_GRND() {}
