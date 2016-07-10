#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_CE PT_CE 206
Element_CE::Element_CE()
{
	Identifier = "DEFAULT_PT_CE";
	Name = "CE";
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
	Description = "Cerium. 58";
	
	Properties = TYPE_PART | PROP_RADIOACTIVE;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_CE::update;
	
}

//#TPT-Directive ElementHeader Element_CE static int update(UPDATE_FUNC_ARGS)
int Element_CE::update(UPDATE_FUNC_ARGS)
 {
int angle, nb, v;
if (parts[i].tmp==141 && (rand()%2)>=1)
{
	nb = sim->create_part(-3, x, y, PT_E1);
	parts[nb].temp = MAX_TEMP/2;
	angle = rand()*2.0f*M_PI/RAND_MAX;
	v = (float)(rand())*5.0f/RAND_MAX+10;
	parts[nb].vx = v*cosf(angle);
	parts[nb].vy = v*sinf(angle);
	sim->part_change_type(i,x,y,PT_PR);

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
				sim->pv[y/CELL][x/CELL] = 175;
			}
}
	return 0;
}


Element_CE::~Element_CE() {}
