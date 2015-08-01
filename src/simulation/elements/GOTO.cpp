#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GOTO PT_GOTO 189
Element_GOTO::Element_GOTO()
{
	Identifier = "DEFAULT_PT_GOTO";
	Name = "GOTO";
	Colour = PIXPACK(0xAE00FF);
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
	Meltable = 0;
	Hardness = 1;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "A linking element. Set link ID in tmp.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 8.8f;
	HighPressureTransition = PT_STNE;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1223.0f;
	HighTemperatureTransition = PT_LAVA;
	
	Update = &Element_GOTO::update;
	Graphics = &Element_GOTO::graphics;
}

//#TPT-Directive ElementHeader Element_GOTO static int update(UPDATE_FUNC_ARGS)
int Element_GOTO::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	for (rx=-2; rx<3; rx++) 
		for (ry=-2; ry<3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				
				if (parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_PSCN)
				{
					parts[i].tmp2 = 0;
				}
				if (parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_NSCN)
				{
					parts[i].tmp2 = 1;
				}
			}
	return 0;
}
//#TPT-Directive ElementHeader Element_GOTO static int graphics(GRAPHICS_FUNC_ARGS)
int Element_GOTO::graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->tmp3 == 1)
	{
		*pixel_mode |= FIRE_ADD;
		*colb += 100;

		*firea = 40;
		*firer = *colr;
		*fireg = *colg;
		*fireb = *colb;
		
		return 1;
	}
	if (cpart->tmp2 == 0)
	{
		*pixel_mode |= FIRE_ADD;
		*colb += 100;

		*firea = 40;
		*firer = *colr;
		*fireg = *colg;
		*fireb = *colb;
	}
	else
	{
		*pixel_mode |= FIRE_ADD;
		*colr = *colr/3;
		*colb = *colb/3;
		*colg = *colg/3;
	
		*firea = 40;
		*firer = 0;
		*fireg = 0;
		*fireb = 0;
	}
	return 0;  
}

Element_GOTO::~Element_GOTO() {}