#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_NBOT PT_NBOT 185
Element_NBOT::Element_NBOT()
{
	Identifier = "DEFAULT_PT_NBOT";
	Name = "NBOT";
	Colour = PIXPACK(0x3E3E36);
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
	Description = "Nanobots. Perform work, enable with PSCN.";
	
	State = ST_SOLID;
	Properties = TYPE_PART|PROP_DRAWONCTYPE|PROP_LIFE_DEC;
	
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
	ct = PT_NONE;
}
if(parts[i].temp>=1825.15)
	sim->part_change_type(i,x,y,PT_BREC);
if(parts[i].tmp2==2)
{
	parts[i].life = 255;
	parts[i].tmp2 = 3;
}
if(parts[i].life<=0 || parts[i].tmp2==0)
{
	parts[i].tmp2 = 0;
}
if(parts[i].life>=1 && parts[i].tmp2==1 || parts[i].life>=1 && parts[i].tmp2==3) //Movement when active
{
	parts[i].vx = (rand() % 11)-5;
	parts[i].vy = (rand() % 11)-5;
}
if(parts[i].tmp==1 && parts[i].life<=10 && parts[i].life>=1) //Explode flag
{
	parts[i].tmp = 1;
	parts[i].life = 1;
	sim->part_change_type(i,x,y,PT_FIRW);
}
if(parts[i].life<=0)
	parts[i].life = 0;
	
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
					parts[i].tmp2 = 1; //Set bot as active from PSCN
				}
				if(parts[r>>8].type==PT_NBOT && parts[i].tmp2==1 && parts[r>>8].tmp2==0 || parts[r>>8].type==PT_NBOT && parts[i].tmp2==2 && parts[r>>8].tmp2==0) //Activates touching bots
				{
					parts[r>>8].tmp2 = 2;
				}
					
				if(parts[r>>8].type==PT_PHOT || parts[r>>8].type==PT_NEUT || parts[r>>8].type==PT_ELEC || parts[r>>8].type==PT_SPRK || parts[r>>8].type==PT_VSPK) //Charge bots from energy
					parts[i].life = 255;

				if(parts[i].tmp==0 && parts[r>>8].type==ct) //Check for retrieve flag. It is the default, 0
				{
					parts[i].tmp = 11; //Set as full
					sim->kill_part(r>>8);
				}
				if(parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp>0 || parts[r>>8].type==PT_VSPK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp>0) //If NSCN is sparked, drop element
				{
					parts[i].vx = 0;
					parts[i].vy = 0;
					if(ct==PT_QRTZ || ct==PT_PQRT)
					{
						parts[i].tmp2 = 5;
					}
					else
					{
						parts[i].tmp2 = 0;
					}
					sim->part_change_type(i,x,y,ct);
					ct = PT_NONE;
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
