#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_SR PT_SR 202
Element_SR::Element_SR()
{
	Identifier = "DEFAULT_PT_SR";
	Name = "SR";
	Colour = PIXPACK(0x7B8784);
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
	Description = "Strontium.";
	
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 312.0f;
	HighTemperatureTransition = NT;
	
	Update = &Element_SR::update;
	
}

//#TPT-Directive ElementHeader Element_SR static int update(UPDATE_FUNC_ARGS)
int Element_SR::update(UPDATE_FUNC_ARGS)
 {
int angle, nb, v;
if (parts[i].tmp==92 && (rand()%3)==1)
{
	nb = sim->create_part(-3, x, y, PT_E1);
	parts[nb].temp = MAX_TEMP/2;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+10;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	sim->part_change_type(i,x,y,PT_Y);
}
	return 0;
}

Element_SR::~Element_SR() {}
