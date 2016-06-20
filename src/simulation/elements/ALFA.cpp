#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_ALFA PT_ALFA 208
Element_ALFA::Element_ALFA()
{
	Identifier = "DEFAULT_PT_ALFA";
	Name = "ALFA";
	Colour = PIXPACK(0x583838);
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
	Description = "Alpha Particles. HE-4 0E";
	
	Properties = TYPE_PART;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_ALFA::update;
	Graphics = &Element_ALFA::graphics;
}

//#TPT-Directive ElementHeader Element_ALFA static int update(UPDATE_FUNC_ARGS)
int Element_ALFA::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry;
	
	if (parts[i].vx<=5 && parts[i].vx>=-5)
		parts[i].vx = parts[i].vx*2;
	
	if (parts[i].vy<=5 && parts[i].vy>=-5)
		parts[i].vy = parts[i].vy*2;
		
		
	int pressureFactor = 3 + (int)sim->pv[y/CELL][x/CELL];
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK)
			{
				r = pmap[y+ry][x+rx];
				if (parts[r>>8].type==PT_NBOT && parts[r>>8].life<=10000)
				{
					parts[r>>8].life = parts[r>>8].life + 500;
					sim->kill_part(i);
				}
				
				switch (r&0xFF)
				{
					case PT_U:
						if (parts[r>>8].tmp==235)
							parts[r>>8].tmp = 236;
						if (parts[r>>8].tmp==238)
						{
							if ((rand()%4)>=3)
								parts[r>>8].tmp = 239;
						}
						sim->kill_part(i);
					break;
					case PT_PU:
						if (parts[r>>8].tmp==240)
						{
							if ((rand()%4)>=3)
								parts[r>>8].tmp = 244;
						}
				}
			}
	return 0;
}



//#TPT-Directive ElementHeader Element_ALFA static int graphics(GRAPHICS_FUNC_ARGS)
int Element_ALFA::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 120;
	*firer = 10;
	*fireg = 80;
	*fireb = 120;

	*pixel_mode |= FIRE_ADD;
	return 1;
}

//#TPT-Directive ElementHeader Element_ALFA static int DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t)
int Element_ALFA::DeutExplosion(Simulation * sim, int n, int x, int y, float temp, int t)//testing a new deut create part
{
	int i;
	n = (n/50);
	if (n < 1)
		n = 1;
	else if (n > 340)
		n = 340;
	
	for (int c = 0; c < n; c++)
	{
		i = sim->create_part(-3, x, y, t);
		if (i >= 0)
			sim->parts[i].temp = temp;
		else if (sim->pfree < 0)
			break;
	}
	sim->pv[y/CELL][x/CELL] += (6.0f * CFDS)*n;
	return 0;
}

Element_ALFA::~Element_ALFA() {}
