#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CBNF PT_CBNF 184
Element_CBNF::Element_CBNF()
{
	Identifier = "DEFAULT_PT_CBNF";
	Name = "CBNF";
	Colour = PIXPACK(0x2B2A39);
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
	Hardness = 500;
	
	Weight = 325;
	
	Temperature = R_TEMP+0.0f +273.15f;
	HeatConduct = 5;
	Description = "Carbon Fiber.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = NT;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = NT;
	HighTemperatureTransition = NT;
	
	Update = &Element_CBNF::update;
	Graphics = &Element_CBNF::graphics;
	
}

//#TPT-Directive ElementHeader Element_CBNF static int update(UPDATE_FUNC_ARGS)
int Element_CBNF::update(UPDATE_FUNC_ARGS)
{
	if(sim->pv[y/CELL][x/CELL]>=(223+-((float)(rand()%1000-500))/500.0f))
	{
		if((rand() % 10)>5)
		{
			sim->part_change_type(i,x,y,PT_BGLA);
		}
		else
		{
			sim->part_change_type(i,x,y,PT_CRBN);
		}
	}
	if(parts[i].tmp2<=0)
	{
		parts[i].tmp2 = (rand() % 3)+5;
	}
int r, rx, ry;
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK) {
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if(parts[i].temp>=1000 && parts[r>>8].temp<=parts[i].temp-200)
				{
					parts[i].life = parts[i].life+50;
					if(parts[i].life>=500)
					{
						if((rand() % 10)>8)
						{
							if(parts[i].temp>=2000)
							{
								sim->part_change_type(i,x,y,PT_BCOL);
							}
							else
							{
								sim->part_change_type(i,x,y,PT_BGLA);
							}
						}
						else
						{
							sim->part_change_type(i,x,y,PT_CRBN);
						}
					}
				}
			}
	return 0;
}
//#TPT-Directive ElementHeader Element_CBNF static int graphics(GRAPHICS_FUNC_ARGS)
int Element_CBNF::graphics(GRAPHICS_FUNC_ARGS)
{
	int z = cpart->tmp2 - 5;//speckles!
	*colr += z * 16;
	*colg += z * 16;
	*colb += z * 16;
	return 0;
}


Element_CBNF::~Element_CBNF() {}
