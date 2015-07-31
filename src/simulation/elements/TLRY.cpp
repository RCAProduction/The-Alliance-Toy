#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_TLRY PT_TLRY 189
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
	AirLoss = 0.90f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 0;
	Explosive = 1;
	Meltable = 0;
	Hardness = 2;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Artillery Shell.";
	
	State = ST_SOLID;
	Properties = TYPE_PART|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 1.0f;
	HighPressureTransition = ST;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = NT;
	HighTemperatureTransition = NT;
	
	Update = &Element_TLRY::update;
	
}

//#TPT-Directive ElementHeader Element_TLRY static int update(UPDATE_FUNC_ARGS)
int Element_TLRY::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry;
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
						
					if (parts[r>>8].type!=PT_TLRY)
					{
						for (rx=-5; rx<6; rx++)
							for (ry=-5; ry<6; ry++)
						sim->create_part(1, x+rx, y+ry, PT_FIRE);
					}

				}
	return 0;
}


Element_TLRY::~Element_TLRY() {}
