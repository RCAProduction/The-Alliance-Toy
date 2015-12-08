#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_H PT_H 197
Element_H::Element_H()
{
	Identifier = "DEFAULT_PT_H";
	Name = "H";
	Colour = PIXPACK(0x382F39);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 0;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Hydrogen. H-1/100 H-2/0 H-3/0";
	
	State = ST_GAS;
	Properties = TYPE_GAS | PROP_RADIOACTIVE | PROP_CONDUCTS | PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_H::update;
	
}

//#TPT-Directive ElementHeader Element_H static int update(UPDATE_FUNC_ARGS)
int Element_H::update(UPDATE_FUNC_ARGS)
 {
int rx, ry, r, nb, v, angle;

if (parts[i].tmp2!=0)
{
	parts[i].tmp2--;
	parts[i].life = 5;
}

	for (rx=-1; rx<2; rx++) 
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				if ((parts[r>>8].type==PT_SPRK || parts[r>>8].type==PT_BTRY) && parts[i].tmp2<=0)
				{
					parts[i].tmp2 = 20;
					nb = sim->create_part(-3, x, y, PT_PHOT);
					parts[nb].ctype = 1065402431;
					parts[nb].temp = 999;
					angle = rand()*2.0f*M_PI/RAND_MAX;
					v = (float)(rand())*5.0f/RAND_MAX;
					parts[nb].vx = v*cosf(angle);
					parts[nb].vy = v*sinf(angle);
				}
			}
	return 0;
}


Element_H::~Element_H() {}
