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
	HighTemperature = 1050.0f;
	HighTemperatureTransition = NT;
	
	Update = &Element_SR::update;
	Graphics = &Element_SR::graphics;
	
}

//#TPT-Directive ElementHeader Element_SR static int update(UPDATE_FUNC_ARGS)
int Element_SR::update(UPDATE_FUNC_ARGS)
 {
int angle, nb, v, r, rx, ry, rt;

for (rx=-2; rx<3; rx++)
		for (ry=-2; ry<3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				rt = r&0xFF;
				if (parts[r>>8].type==PT_WATR && parts[i].tmp!=1)
				{
					parts[r>>8].type=PT_H;
					parts[r>>8].temp=600;
					parts[i].temp=490;
					parts[i].tmp=1;
				}
				/*if (parts[r>>8].type==PT_PLSM && parts[i].temp>=212)
				{
					parts[i].type=PT_PLSM;
					parts[i].life=100;
					//parts[i].temp=5000;
					sim->pv[y/CELL][x/CELL] = 6;
					
				}  //Burning*/
			}

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
if (parts[i].temp>=1050)
{
	parts[i].ctype=parts[i].type;
	parts[i].tmp3=1050;
	parts[i].tmp4=1650; //These values store melting and boiling points respectively. In tmp3 and 4 to avoid tampering. I don't want to create a new universal value to store gasseous, so I won't. Melting point is redundant, but might be useful. 
	parts[i].type=PT_LAVA;
	
}
	return 0;
}


//#TPT-Directive ElementHeader Element_SR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_SR::graphics(GRAPHICS_FUNC_ARGS)
{
if (cpart->tmp==1)
{
	*colr =100;
	*colg =100;
	*colb =100;
}

	return 0;
}

Element_SR::~Element_SR() {}
