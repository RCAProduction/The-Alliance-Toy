#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_VSPK PT_VSPK 180
Element_VSPK::Element_VSPK()
{
	Identifier = "DEFAULT_PT_VSPK";
	Name = "VSPK";
	Colour = PIXPACK(0xFFFF80);
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
	HotAir = 0.001f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Voltage Spark. Voltage set in life value.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID|PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_VSPK::update;
	Graphics = &Element_VSPK::graphics;
}

//#TPT-Directive ElementHeader Element_VSPK static int update(UPDATE_FUNC_ARGS)
int Element_VSPK::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry, nearp, pavg, ct = parts[i].ctype, sender, receiver;
	Element_FIRE::update(UPDATE_FUNC_SUBCALL_ARGS);

	if (parts[i].life<=0)
	{
		if (ct!=PT_ZRNT)
		{
			parts[i].temp = 5000;
		}
		if (ct<=0 || ct>=PT_NUM || !sim->elements[parts[i].ctype].Enabled)
			sim->kill_part(i);
		sim->part_change_type(i,x,y,ct);
		parts[i].ctype = PT_NONE;
		parts[i].life = 4;

		return 0;
	}
	//Some functions of VSPK based on ctype (what it is on)
	switch(ct)
	{
	case PT_VSPK:
		sim->kill_part(i);
		return 1;
	}
	for (rx=-2; rx<3; rx++)
		for (ry=-2; ry<3; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				receiver = r&0xFF;
				sender = ct;
				pavg = sim->parts_avg(r>>8, i,PT_INSL);
				//receiver is the element VSPK is trying to conduct to
				//sender is the element the VSPK is on
				//First, some checks usually for (de)activation of elements


				if (pavg == PT_INSL) continue; //Insulation blocks everything past here
				if (!((sim->elements[receiver].Properties&PROP_CONDUCTS)||receiver==PT_INST||receiver==PT_QRTZ)) continue; //Stop non-conducting receivers, allow INST and QRTZ as special cases

				if (parts[r>>8].life==0) {
					parts[r>>8].life = parts[i].life - 1;
					parts[r>>8].ctype = receiver;
					sim->part_change_type(r>>8,x+rx,y+ry,PT_VSPK);
					if (parts[r>>8].type!=PT_ZRNT)
						parts[r>>8].temp = parts[r>>8].temp+10.0f;
					if(parts[i].temp>=4500)
						sim->part_change_type(i,x,y,ct);
					//if(parts[i].x+1==PT_VSPK || parts[i].y+1==PT_VSPK)
					//	parts[i].life = parts[i].life;
				}
			}
	return 0;
}



//#TPT-Directive ElementHeader Element_VSPK static int graphics(GRAPHICS_FUNC_ARGS)
int Element_VSPK::graphics(GRAPHICS_FUNC_ARGS)

{
	*firea = 60;
	*firer = *colr/2;
	*fireg = *colg/2;
	*fireb = *colb/2;
	*pixel_mode |= FIRE_SPARK;
	return 1;
}


Element_VSPK::~Element_VSPK() {}
