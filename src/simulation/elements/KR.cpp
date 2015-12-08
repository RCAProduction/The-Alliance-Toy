#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_KR PT_KR 195
Element_KR::Element_KR()
{
	Identifier = "DEFAULT_PT_KR";
	Name = "KR";
	Colour = PIXPACK(0x1F3C3C);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 1.0f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.30f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 0.75f;
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 3;
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Krypton. Kr-78/.35 Kr-86/17.3 Kr-84/57.0 Kr-82/11.58 Kr-83/13.8";
	
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
	
	Update = &Element_KR::update;
	
}

//#TPT-Directive ElementHeader Element_KR static int update(UPDATE_FUNC_ARGS)
int Element_KR::update(UPDATE_FUNC_ARGS)
 {
int rx, ry, r, nb, v, angle;

if (parts[i].tmp==0)
	parts[i].tmp=83;
	
if (parts[i].tmp==92 && (rand()%2)>=1)
{
	nb = sim->create_part(-3, x, y, PT_E1);
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	
	parts[i].type = PT_RB;
}

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
				if ((parts[r>>8].type==PT_SPRK || parts[r>>8].type==PT_BTRY) && parts[i].tmp2==0)
				{
					parts[i].tmp2 = 20;
					nb = sim->create_part(-3, x, y, PT_PHOT);
					parts[nb].ctype = 2013782271;
					parts[nb].temp = 999;
					angle = rand()*2.0f*M_PI/RAND_MAX;
					v = (float)(rand())*5.0f/RAND_MAX;
					parts[nb].vx = v*cosf(angle);
					parts[nb].vy = v*sinf(angle);
				}
			}
	return 0;
}


Element_KR::~Element_KR() {}
