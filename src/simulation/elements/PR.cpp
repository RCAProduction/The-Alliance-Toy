#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_PR PT_PR 207
Element_PR::Element_PR()
{
	Identifier = "DEFAULT_PT_PR";
	Name = "PR";
	Colour = PIXPACK(0x798898);
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
	
	Weight = 235;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Praseodymium. 59";

	Properties = TYPE_PART;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_PR::update;
	
}

//#TPT-Directive ElementHeader Element_PR static int update(UPDATE_FUNC_ARGS)
int Element_PR::update(UPDATE_FUNC_ARGS)
 {
int angle, nb, v;
/*if (parts[i].tmp==141 && (rand()%2)>=1)
{
	nb = sim->create_part(-3, x, y, PT_E1);
	parts[nb].temp = MAX_TEMP/2;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+10;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	sim->part_change_type(i,x,y,PT_RB);
}*/
	return 0;
}


Element_PR::~Element_PR() {}
