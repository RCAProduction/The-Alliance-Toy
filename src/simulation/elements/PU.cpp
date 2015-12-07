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
	
	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 90;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Plutonium. Pu-238/d Pu-239/d Pu-240/d";
	
	State = ST_SOLID;
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
if (parts[i].tmp==244)
{
	sim->part_change_type(i,x,y,PT_BA);
	nb = sim->create_part(-3, x, y, PT_ALFA);
	parts[nb].temp = 9000;
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
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
}
	return 0;
}


Element_PU::~Element_PU() {}
