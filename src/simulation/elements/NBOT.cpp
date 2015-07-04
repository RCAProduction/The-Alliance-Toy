#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_NBOT PT_NBOT 186
Element_NBOT::Element_NBOT()
{
	Identifier = "DEFAULT_PT_NBOT";
	Name = "NBOT";
	Colour = PIXPACK(0x3E3E36);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;
	
	Advection = 0.8f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.9f;
	Loss = 0.70f;
	Collision = -0.1f;
	Gravity = 0.0f;
	Diffusion = 3.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;
	
	Weight = 1;
	
	Temperature = R_TEMP +273.15f;
	HeatConduct = 100;
	Description = "Nanobot. TMP: 1 - GET";
	
	State = ST_GAS;
	Properties = PROP_DRAWONCTYPE | PROP_LIFE_DEC;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_NBOT::update;
	Graphics = &Element_NBOT::graphics;
}

//#TPT-Directive ElementHeader Element_NBOT static int update(UPDATE_FUNC_ARGS)
int Element_NBOT::update(UPDATE_FUNC_ARGS)
{

int r, rx, ry, ct = parts[i].ctype;

if(ct==PT_NBOT)
{
	parts[i].ctype = PT_NONE;
}
if(parts[i].life<=0 || parts[i].tmp2==0) //Check and set inactive bots
{
	parts[i].vx = 0;
	parts[i].vy = 0;
	parts[i].tmp2 = 0; //Set bot as inactive
}
if(parts[i].life>=1 || parts[i].tmp2==-1) //If active remove life
{
	parts[i].life = parts[i].life-1;
}
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				
				if(parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_PSCN || parts[r>>8].type==PT_VSPK && parts[r>>8].ctype==PT_PSCN)
				{
					parts[i].life = 255;
					parts[i].tmp2 = -1; //Set bot as active
				}
				if(parts[i].tmp==1 && parts[r>>8].type==ct) //Check for get flag
				{
					parts[i].tmp = parts[r>>8].type+4; //Add 4 so that flags aren't read as elements
					sim->kill_part(r>>8);
				}

				if(parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp>4 || parts[r>>8].type==PT_VSPK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp>4) //If NSCN is sparked, drop element
				{
					sim->part_change_type(i,x,y,(parts[i].tmp-4));
				}
			}
	return 0;
}

//#TPT-Directive ElementHeader Element_NBOT static int graphics(GRAPHICS_FUNC_ARGS)
int Element_NBOT::graphics(GRAPHICS_FUNC_ARGS)
{
	//*colr = *colg = *colb = *cola = 0;
	//*pixel_mode &= ~PMODE;
	return 0;
}

Element_NBOT::~Element_NBOT() {}
