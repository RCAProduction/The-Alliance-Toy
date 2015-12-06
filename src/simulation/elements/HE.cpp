#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_HE PT_HE 196
Element_HE::Element_HE()
{
	Identifier = "DEFAULT_PT_HE";
	Name = "HE";
	Colour = PIXPACK(0x583838);
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
	Description = "Helium. He-4/100";
	
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
	
	Update = &Element_HE::update;
	
}

//#TPT-Directive ElementHeader Element_HE static int update(UPDATE_FUNC_ARGS)
int Element_HE::update(UPDATE_FUNC_ARGS)
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
				if ((parts[r>>8].type==PT_SPRK || parts[r>>8].type==PT_BTRY) && parts[i].tmp2==0)
				{
					parts[i].tmp2 = 20;
					nb = sim->create_part(-3, x, y, PT_PHOT);
					parts[nb].ctype = 1072693248;
					parts[nb].temp = 999;
					angle = rand()*2.0f*M_PI/RAND_MAX;
					v = (float)(rand())*5.0f/RAND_MAX;
					parts[nb].vx = v*cosf(angle);
					parts[nb].vy = v*sinf(angle);
				}
				if (parts[r>>8].type==PT_PU && parts[r>>8].tmp==240)
				{
					parts[r>>8].tmp=244; //Make Curium
				}
			}
	return 0;
}


Element_HE::~Element_HE() {}
