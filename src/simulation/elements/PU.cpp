#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PU PT_PU 194
Element_PU::Element_PU()
{
	Identifier = "DEFAULT_PT_PU";
	Name = "PU";
	Colour = PIXPACK(0x729C00);
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
	Meltable = 1;
	Hardness = 1;
	
	Weight = 240;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Plutonium. Pu-238/d Pu-239/d Pu-240/d";
	
	Properties = TYPE_PART | PROP_RADIOACTIVE | PROP_NEUTPENETRATE;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_PU::update;
	
}

//#TPT-Directive ElementHeader Element_PU static int update(UPDATE_FUNC_ARGS)
int Element_PU::update(UPDATE_FUNC_ARGS)
{
int angle, v, nb;

if (parts[i].tmp==0)
	parts[i].tmp=239;
	
if (parts[i].tmp==239)
{
	
	if ((rand()%175200000)==1)
	{
		sim->create_part(i, x, y, PT_U);
		nb = sim->create_part(-3, x, y, PT_ALFA);
		parts[nb].temp = 9000;
		parts[nb].temp = MAX_TEMP/2;
		angle = rand()*2.0f*M_PI/RAND_MAX;
		v = (float)(rand())*5.0f/RAND_MAX;
		parts[nb].vx = v*cosf(angle);
		parts[nb].vy = v*sinf(angle);
		sim->pv[y/CELL][x/CELL] = 175;
	}
}
if (parts[i].tmp==240)
{
	if (sim->pv[y/CELL][x/CELL] >= 2)
		parts[i].tmp = 244;
}
if (parts[i].tmp==244)
{
	nb = sim->create_part(-3, x, y, PT_ALFA);
	parts[nb].temp = 9000;
	parts[nb].tmp = 141;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	sim->pv[y/CELL][x/CELL] = 175;
	
	nb = sim->create_part(-3, x, y, PT_U);
	parts[nb].temp = 9000;
	parts[nb].tmp = 235;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	
	nb = sim->create_part(-3, x, y, PT_KR);
	parts[nb].temp = 9000;
	parts[nb].tmp = 92;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	
	parts[i].tmp = 141;
	sim->part_change_type(i,x,y,PT_BA);

	//Energy released; Travels through surrounding particles, heating them. [[May cause reactions/ionization later.]]

	int rx, ry, r;

	for (rx=-7; rx<8; rx++) 
		for (ry=-7; ry<8; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				parts[r>>8].temp = 10000;
				sim->pv[y/CELL][x/CELL] = 175;
			}
}
	return 0;
}


Element_PU::~Element_PU() {}
