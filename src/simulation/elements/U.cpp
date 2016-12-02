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
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 2;
	Hardness = 2;

	Weight = 90; //235
	
	Temperature = R_TEMP+30.0f+273.15f;
	HeatConduct = 251;
	Description = "Uranium. U-234/.0054 U-235/.72 U-238/99.2746";
	
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
if (parts[i].tmp==235)
{
	if (sim->pv[y/CELL][x/CELL] >= 3)
		parts[i].tmp = 236;
}
if (parts[i].tmp==236)
{
	sim->pv[y/CELL][x/CELL] = 175;
	nb = sim->create_part(-3, x, y, PT_KR);
	parts[nb].temp = MAX_TEMP/2;
	parts[nb].tmp=92;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+1;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	
	nb = sim->create_part(-3, x, y, PT_BA);
	parts[nb].temp = MAX_TEMP/2;
	parts[nb].tmp=141;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+1;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	
	sim->kill_part(i);
	
	createcount=0;
	
	while (createcount<2)
	{
		createcount++;
		nb = sim->create_part(-3, x, y, PT_N0);
		parts[nb].temp = MAX_TEMP/2;
		angle = rand()*2.0f*M_PI/RAND_MAX;
		v = (float)(rand())*5.0f/RAND_MAX+1;
		parts[nb].vx = v*cosf(angle);
		parts[nb].vy = v*sinf(angle);
	}
	
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
				sim->pv[y/CELL][x/CELL] = 300;
			}

}
if (parts[i].tmp==239)
{
	sim->create_part(i, x, y, PT_PU);
}
	return 0;
}


Element_U::~Element_U() {}
