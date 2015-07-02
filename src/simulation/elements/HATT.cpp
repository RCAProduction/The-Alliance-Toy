#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_HATT PT_HATT 181
Element_HATT::Element_HATT()
{
	Identifier = "DEFAULT_PT_HATT";
	Name = "HATT";
	Colour = PIXPACK(0x4D7641);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;
	
	Weight = 31;
	
	Temperature = R_TEMP-2.0f	+325.15f;
	HeatConduct = 251;
	Description = "Hattonite.";
	
	State = ST_LIQUID;
	Properties = TYPE_LIQUID|PROP_NEUTPASS;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_HATT::update;
	Graphics = &Element_HATT::graphics;
}

//#TPT-Directive ElementHeader Element_HATT static int update(UPDATE_FUNC_ARGS)
int Element_HATT::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry, trade, np;
	float gravtot = fabs(sim->gravy[(y/CELL)*(XRES/CELL)+(x/CELL)])+fabs(sim->gravx[(y/CELL)*(XRES/CELL)+(x/CELL)]);
	int maxlife = ((10000/(parts[i].temp + 1))-1);
	if ((10000%((int)parts[i].temp+1))>rand()%((int)parts[i].temp+1))
		maxlife ++;
	// Compress when Newtonian gravity is applied
	// multiplier=1 when gravtot=0, multiplier -> 5 as gravtot -> inf
	maxlife = maxlife*(5.0f - 8.0f/(gravtot+2.0f));
	if (parts[i].life < maxlife)
	{
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r || (parts[i].life >=maxlife))
						continue;
					if ((r&0xFF)==PT_HATT&& !(rand()%3))
					{
						// If neighbour life+1 fits in the free capacity for this particle, absorb neighbour
						// Condition is written in this way so that large neighbour life values don't cause integer overflow
						if (parts[r>>8].life <= maxlife - parts[i].life - 1)
						{
							parts[i].life += parts[r>>8].life + 1;
							sim->kill_part(r>>8);
						}
					}
				}
	}
	else
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (parts[i].life<=maxlife)
						continue;
					if ((!r)&&parts[i].life>=1)//if nothing then create HATT
					{
						np = sim->create_part(-1,x+rx,y+ry,PT_HATT);
						if (np<0) continue;
						parts[i].life--;
						parts[np].temp = parts[i].temp;
						parts[np].life = 0;
					}
				}
	for ( trade = 0; trade<4; trade ++)
	{
		rx = rand()%5-2;
		ry = rand()%5-2;
		if (BOUNDS_CHECK && (rx || ry))
		{
			r = pmap[y+ry][x+rx];
			if (!r)
				continue;
			if ((r&0xFF)==PT_HATT&&(parts[i].life>parts[r>>8].life)&&parts[i].life>0)//diffusion
			{
				int temp = parts[i].life - parts[r>>8].life;
				if (temp ==1)
				{
					parts[r>>8].life ++;
					parts[i].life --;
				}
				else if (temp>0)
				{
					parts[r>>8].life += temp/2;
					parts[i].life -= temp/2;
				}
			}
		}
	}
	return 0;
}



//#TPT-Directive ElementHeader Element_HATT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_HATT::graphics(GRAPHICS_FUNC_ARGS)

{
	if(cpart->life>=240)
	{
		*firea = 60;
		*firer = *colr += 255;
		*fireg = *colg += 255;
		*fireb = *colb += 255;
		*pixel_mode |= PMODE_GLOW | FIRE_ADD;
	}
	else if(cpart->life>0)
	{
		*colr += cpart->life*1;
		*colg += cpart->life*2;
		*colb += cpart->life*3;
		*pixel_mode |= PMODE_BLUR;
	}
	else
	{
		*pixel_mode |= PMODE_BLUR;
	}
	return 0;
}


Element_HATT::~Element_HATT() {}
