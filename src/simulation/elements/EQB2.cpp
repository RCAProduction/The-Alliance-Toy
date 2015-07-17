#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_EQB2 PT_EQB2 188
Element_EQB2::Element_EQB2()
{
	Identifier = "DEFAULT_PT_EQB2";
	Name = "EQB2";
	Colour = PIXPACK(0xFECB67);
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
	Description = "EQB2 - Equilibrium test element 2";
	
	State = ST_LIQUID;
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
	int r, rx, ry;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if (((floor(rand() % 50)) + 1) == 1)
				{
					parts[i].tmp = 5;
					if (parts[r>>8].type == 188)
					{
						parts[i].tmp = 10;
						sim->part_change_type(r>>8,x+rx,y+ry,187);
						sim->part_change_type(i,x,y,187);
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
