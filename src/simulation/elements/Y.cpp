#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_Y PT_Y 203
Element_Y::Element_Y()
{
	Identifier = "DEFAULT_PT_Y";
	Name = "Y";
	Colour = PIXPACK(0x5D6F49);
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
	HeatConduct = 240;
	Description = "Yttrium.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 312.0f;
	HighTemperatureTransition = NT;
	
	Update = &Element_Y::update;
	
}

//#TPT-Directive ElementHeader Element_Y static int update(UPDATE_FUNC_ARGS)
int Element_Y::update(UPDATE_FUNC_ARGS)
 {
int angle, nb, v;
if (parts[i].tmp==141)
{
	parts[i].tmp=140;
	nb = sim->create_part(-3, x, y, PT_ELEC);
	parts[nb].temp = MAX_TEMP/2;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+10;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	sim->part_change_type(i,x,y,PT_ZR);
}
	return 0;
}

Element_Y::~Element_Y() {}
