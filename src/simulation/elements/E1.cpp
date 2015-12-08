#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_E1 PT_E1 209
Element_E1::Element_E1()
{
	Identifier = "DEFAULT_PT_E1";
	Name = "E1";
	Colour = PIXPACK(0xDFEFFF);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 1.00f;
	Collision = -0.99f;
	Gravity = 0.0f;
	Diffusion = 0.01f;
	HotAir = 0.002f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = -1;
	
	Temperature = R_TEMP+4.0f	+273.15f;
	HeatConduct = 60;
	Description = "Electrons. E-";
	
	State = ST_GAS;
	Properties = TYPE_ENERGY;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_E1::update;
	Graphics = &Element_E1::graphics;
}

//#TPT-Directive ElementHeader Element_E1 static int update(UPDATE_FUNC_ARGS)
int Element_E1::update(UPDATE_FUNC_ARGS)
 {
	 int r, rt, rx, ry, nb, rrx, rry, angle, v;
	 
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+5;
	parts[i].vx = v*cosf(angle);
	parts[i].vy = v*sinf(angle);
	
	for (rx=-2; rx<=2; rx++)
		for (ry=-2; ry<=2; ry++)
			if (BOUNDS_CHECK) {
				r = pmap[y+ry][x+rx];
				if (!r)
					r = sim->photons[y+ry][x+rx];
				if (!r)
					continue;
				if (parts[r>>8].type==PT_NBOT && parts[r>>8].life<=10000)
				{
					parts[r>>8].life = parts[r>>8].life + 500;
					sim->kill_part(i);
				}
				if (parts[r>>8].type==PT_ALFA)
				{
					sim->part_change_type(i, x, y, PT_HE);
					sim->kill_part(r>>8);
				}
				rt = r&0xFF;
				switch (rt)
				{
				case PT_GLAS:
					for (rrx=-1; rrx<=1; rrx++)
						for (rry=-1; rry<=1; rry++)
							if (x+rx+rrx>=0 && y+ry+rry>=0 && x+rx+rrx<XRES && y+ry+rry<YRES) {
								nb = sim->create_part(-1, x+rx+rrx, y+ry+rry, PT_EMBR);
								if (nb!=-1) {
									parts[nb].tmp = 0;
									parts[nb].life = 50;
									parts[nb].temp = parts[i].temp*0.8f;
									parts[nb].vx = rand()%20-10;
									parts[nb].vy = rand()%20-10;
								}
							}
					sim->kill_part(i);
					return 1;
				case PT_LCRY:
					parts[r>>8].tmp2 = 5+rand()%5;
					break;
				case PT_WATR:
				case PT_DSTW:
				case PT_SLTW:
				case PT_CBNW:
					if(!(rand()%3))
						sim->create_part(r>>8, x+rx, y+ry, PT_O2);
					else
						sim->create_part(r>>8, x+rx, y+ry, PT_H2);
					sim->kill_part(i);
					return 1;
				case PT_PROT: // this is the correct reaction, not NEUT, but leaving NEUT in anyway
					if (parts[r>>8].tmp2 & 0x1)
						break;
				case PT_NEUT:
					sim->part_change_type(r>>8, x+rx, y+ry, PT_H2);
					parts[r>>8].life = 0;
					parts[r>>8].ctype = 0;
					sim->kill_part(i);
					break;
				case PT_DEUT:
					if(parts[r>>8].life < 6000)
						parts[r>>8].life += 1;
					parts[r>>8].temp = 0;
					sim->kill_part(i);
					return 1;
				case PT_EXOT:
					parts[r>>8].tmp2 += 5;
					parts[r>>8].life = 1000;
					break;
				case PT_NONE: //seems to speed up E1 even if it isn't used
					break;
				default:
					if ((sim->elements[rt].Properties & PROP_CONDUCTS) && (rt!=PT_NBLE||parts[i].temp<2273.15))
					{
						sim->create_part(-1, x+rx, y+ry, PT_SPRK);
						sim->kill_part(i);
						return 1;
					}
					break;
				}
			}
	return 0;
}

//#TPT-Directive ElementHeader Element_E1 static int graphics(GRAPHICS_FUNC_ARGS)
int Element_E1::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 70;
	*firer = *colr;
	*fireg = *colg;
	*fireb = *colb;

	*pixel_mode |= FIRE_ADD;
	return 0;
}


Element_E1::~Element_E1() {}
