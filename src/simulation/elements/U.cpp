#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_U PT_U 193
Element_U::Element_U()
{
	Identifier = "DEFAULT_PT_U";
	Name = "U";
	Colour = PIXPACK(0x707020);
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
	Description = "Uranium. U-234/.0054 U-235/.72 U-238/99.2746";
	
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
	
	Update = &Element_U::update;
	
}

//#TPT-Directive ElementHeader Element_U static int update(UPDATE_FUNC_ARGS)
int Element_U::update(UPDATE_FUNC_ARGS)
 {
int r, rx, ry, nb, v, angle, createcount;
if (parts[i].tmp==0)
	parts[i].tmp=238;
if (parts[i].tmp==236)
{
	sim->pv[y/CELL][x/CELL] = 175;
	if ((rand()%2)>=1)
	{
		parts[i].tmp = 92;
		parts[i].type = PT_KR;
	}
	else
	{
		parts[i].tmp = 141;
		parts[i].type = PT_BA;
	}
	
	createcount=0;
	
	while (createcount<3)
	{
		createcount++;
		nb = sim->create_part(-3, x, y, PT_N0);
		parts[nb].temp = MAX_TEMP/2;
		angle = rand()*2.0f*M_PI/RAND_MAX;
		v = (float)(rand())*5.0f/RAND_MAX+1;
		parts[nb].vx = v*cosf(angle);
		parts[nb].vy = v*sinf(angle);
	}
	
	for (rx=-6; rx<7; rx++) 
				for (ry=-6; ry<7; ry++)
					if (BOUNDS_CHECK && (rx || ry))
					{
						r = pmap[y+ry][x+rx];
						if (!r)
							continue;
						parts[r>>8].temp = 9000;
					}
}
if (parts[i].tmp==239)
{
	sim->create_part(i, x, y, PT_PU);
}
	return 0;
}


Element_U::~Element_U() {}
