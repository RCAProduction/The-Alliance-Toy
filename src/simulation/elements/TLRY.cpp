#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_TLRY PT_TLRY 190
Element_TLRY::Element_TLRY()
{
	Identifier = "DEFAULT_PT_TLRY";
	Name = "TLRY";
	Colour = PIXPACK(0x484848);
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
	
	Weight = 90;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 211;
	Description = "Artillery Shell. Explodes.";
	
	State = ST_SOLID;
	Properties = TYPE_PART|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1273.0f;
	HighTemperatureTransition = ST;
	
	Update = &Element_TLRY::update;
	
}

//#TPT-Directive ElementHeader Element_TLRY static int update(UPDATE_FUNC_ARGS)
int Element_TLRY::update(UPDATE_FUNC_ARGS)
 {
	int r, rx, ry, tempFactor;
	if (parts[i].temp > 523.15f)//250.0f+273.15f
	{
		tempFactor = 1000 - ((523.15f-parts[i].temp)*2);
		if(tempFactor < 2)
			tempFactor = 2;
		for (rx=-1; rx<2; rx++)
			for (ry=-1; ry<2; ry++)
				if (BOUNDS_CHECK && (rx || ry))
				{
					r = pmap[y+ry][x+rx];
					if (!r)
						continue;
					if ((r&0xFF)==PT_BREC && !(rand()%tempFactor))
					{
						if(rand()%2)
						{
							sim->create_part(r>>8, x+rx, y+ry, PT_THRM);
						}
						else
							sim->create_part(i, x, y, PT_THRM);
						//part_change_type(r>>8,x+rx,y+ry,PT_BMTL);
						//parts[r>>8].tmp=(parts[i].tmp<=7)?parts[i].tmp=1:parts[i].tmp-(rand()%5);//rand()/(RAND_MAX/300)+100;
					}
				}
	}
	return 0;
}


Element_TLRY::~Element_TLRY() {}
