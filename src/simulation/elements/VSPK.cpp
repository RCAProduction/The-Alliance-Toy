#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_VSPK PT_VSPK 180
Element_VSPK::Element_VSPK()
{
	Identifier = "DEFAULT_PT_VSPK";
	Name = "VSPK";
	Colour = PIXPACK(0xFFFF80);
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
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Voltage Spark. Voltage set in life value.";
	
	Properties = TYPE_SOLID|PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_VSPK::update;
	Graphics = &Element_VSPK::graphics;
}

//#TPT-Directive ElementHeader Element_VSPK static int update(UPDATE_FUNC_ARGS)
int Element_VSPK::update(UPDATE_FUNC_ARGS)
{

int r, rx, ry, ct = parts[i].ctype, sender, receiver, pavg, giveAmount;
	
if (parts[i].ctype==PT_VSPK)
	sim->kill_part(i);

if (parts[i].life<=0 || !(sim->elements[parts[i].ctype].Properties&PROP_CONDUCTS))
{
	parts[i].life = 15;
	sim->part_change_type(i,x,y,ct);
}

if (parts[i].tmp2>=1)
	parts[i].tmp2 = parts[i].tmp2 - 1;
	
if (parts[i].ctype!=PT_ZRNT)
{
	parts[i].temp = 5000;
	sim->part_change_type(i, x, y, PT_LAVA);
}
	
	for (rx=-2; rx<3; rx++) 
		for (ry=-2; ry<3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				
				pavg = sim->parts_avg(r>>8, i,PT_INSL);
					
				if (pavg==PT_INSL || pavg==PT_CBNF)
					continue;
				
				if ((sim->elements[parts[r>>8].type].Properties&PROP_CONDUCTS) && parts[r>>8].type!=PT_ZRNT)
				{
					parts[r>>8].ctype = parts[r>>8].type;
					parts[r>>8].temp = 5000;
					sim->part_change_type(r>>8, x, y, PT_LAVA);
					continue;
				}
				
				if (parts[r>>8].life==0 && parts[r>>8].type!=PT_VSPK && parts[i].tmp2==0)
				{
					parts[r>>8].ctype = parts[r>>8].type;
					sim->part_change_type(r>>8, x+rx, y+ry, PT_VSPK);
					parts[r>>8].life = parts[i].life - 1;
					parts[r>>8].tmp2 = 2;
				}
				
				if (parts[i].life>parts[r>>8].life && parts[r>>8].type==PT_VSPK && parts[i].life>=16)
				{
					parts[r>>8].life = parts[i].life - 1;
				}
			}
	return 0;
}



//#TPT-Directive ElementHeader Element_VSPK static int graphics(GRAPHICS_FUNC_ARGS)
int Element_VSPK::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 60;
	*firer = *colr/2;
	*fireg = *colg/2;
	*fireb = *colb/2;
	*pixel_mode |= FIRE_SPARK;
	return 1;
}


Element_VSPK::~Element_VSPK() {}