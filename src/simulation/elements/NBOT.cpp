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

int r, rx, ry, ct = parts[i].ctype, giveAmount;

if(ct==PT_NBOT)
{
	ct = PT_NONE;
}
if(parts[i].temp>=1825.15)
	if(parts[i].tmp==1)
	{
		parts[i].temp = 1800;
		parts[i].life = parts[i].life - 2;
	}
	else
	{
	sim->part_change_type(i,x,y,PT_BREC);
	}
if(parts[i].tmp2==2)
{
	parts[i].life = 255;
	parts[i].tmp2 = 3;
}
if(parts[i].life<=0 && parts[i].tmp2!=0)
{
	parts[i].tmp2 = 4;
}
if(parts[i].life>=1 && parts[i].tmp2==4)
	parts[i].tmp2 = 3;
if(parts[i].life>=1 && parts[i].tmp2==1 || parts[i].life>=1 && parts[i].tmp2==3) //Movement when active
{
	if(parts[i].tmp==3 && parts[i].tmp3>=1)
	{
		if(parts[i].x>parts[i].tmp3)
		{
			parts[i].vx = (rand() % 21)-16;
		}
		else if(parts[i].x<parts[i].tmp3)
		{
			parts[i].vx = (rand() % 21)-4;
		}
		if(parts[i].y>parts[i].tmp4)
		{
			parts[i].vy = (rand() % 9)-7;
		}
		else if(parts[i].y<parts[i].tmp4)
		{
			parts[i].vy = (rand() % 9)-1;
		}
	}
	else if(parts[i].tmp3>=1 && parts[i].x>parts[i].tmp3 && parts[i].tmp!=6)
	{
		parts[i].vx = (rand() % 11)-8;
	}
	else if(parts[i].tmp3>=1 && parts[i].x<parts[i].tmp3 && parts[i].tmp!=6)
	{
		parts[i].vx = (rand() % 11)-2;
	}
	if(parts[i].tmp4>=1 && parts[i].y>parts[i].tmp4 && parts[i].tmp!=6)
	{
		parts[i].vy = (rand() % 11)-8;
	}
	else if(parts[i].tmp4>=1 && parts[i].y<parts[i].tmp4 && parts[i].tmp!=6)
	{
		parts[i].vy = (rand() % 11)-2;
	}
	else if(parts[i].tmp==3 && parts[i].tmp3==0)
	{
		parts[i].vx = (rand() % 21)-10;
		parts[i].vy = (rand() % 9)-4;
	}
	else if(parts[i].tmp3==0)
	{
		parts[i].vx = (rand() % 11)-5;
		parts[i].vy = (rand() % 11)-5;
	}
}
if(parts[i].tmp==1 && parts[i].life<=10 && parts[i].life>=1)
{
	parts[i].tmp = 1;
	parts[i].life = 1;
	sim->part_change_type(i,x,y,PT_FIRW);
}

if(parts[i].life<=0)
	parts[i].life = 0;
if(parts[i].life>2000 && parts[i].tmp!=2)
	parts[i].life = 2000;
	
	for (rx=-1; rx<2; rx++)
		for (ry=-1; ry<2; ry++)
			if (BOUNDS_CHECK && (rx || ry))
			{
				r = pmap[y+ry][x+rx];
				if (!r)
					continue;
				
				if(parts[r>>8].life<parts[i].life && parts[r>>8].type==PT_NBOT && parts[i].tmp2>=1 && parts[i].tmp2<=4 && parts[r>>8].tmp2>=1 && parts[r>>8].tmp2<=4)
				{
					if(parts[i].life>255 && parts[i].tmp==2)
					{
						giveAmount = 255 - parts[r>>8].life;
						parts[r>>8].life = parts[r>>8].life + giveAmount;
						parts[i].life = parts[i].life - giveAmount;
					}
					else
					{
						giveAmount = (parts[i].life - parts[r>>8].life)/2;
						parts[r>>8].life = parts[r>>8].life + giveAmount;
						parts[i].life = parts[i].life - giveAmount;
					}
				}
				if(parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_PSCN || parts[r>>8].type==PT_VSPK && parts[r>>8].ctype==PT_PSCN)
				{
					parts[i].life = 255;
					parts[i].tmp2 = 1; //Set bot as active from PSCN
				}
				if(parts[r>>8].type==PT_NBOT && parts[i].tmp2==1 && parts[r>>8].tmp2==0 || parts[r>>8].type==PT_NBOT && parts[i].tmp2==2 && parts[r>>8].tmp2==0) //Activates touching bots
				{
					parts[r>>8].tmp2 = 2;
				}

				if(parts[i].tmp==0 && parts[r>>8].type==ct && parts[i].tmp2>=1 && parts[i].tmp2<=3 && parts[r>>8].tmp<=1000) //Check for retrieve flag. It is the default, 0
				{
					parts[i].tmp = 256; //Set as full
					sim->kill_part(r>>8);
				}
				if(parts[i].tmp==1)
				{
					if(parts[i].life>=1 && parts[i].tmp==1 && ct==parts[r>>8].type)
					{
						parts[i].tmp = 1;
						parts[i].life = 1;
						sim->part_change_type(i,x,y,PT_FIRW);
					}
				}
				if(parts[i].tmp==2 && parts[i].tmp2>=1 && parts[i].tmp2<=3 && parts[r>>8].type==ct && parts[i].life<7000) //Chargers
				{
					parts[i].life = parts[i].life + ((rand() % 100)+1000);
					sim->kill_part(r>>8);
				}
				if(parts[i].tmp==4 && parts[i].life>=1 && parts[r>>8].type==ct) //Break flag
				{
					sim->kill_part(r>>8);
					parts[i].vx = (rand() % 17)-8;
					parts[i].vy = (rand() % 17)-8;
					sim->part_change_type(i,x,y,PT_BREC);
				}
				if(parts[i].tmp==5 && parts[i].life>=1 && parts[r>>8].type==PT_BREC) //Replicate flag
				{
					sim->part_change_type(r>>8,x,y,PT_NBOT);
					parts[r>>8].tmp2 = 4;
				}
				if(parts[i].tmp==6 && parts[i].life>=1 && parts[r>>8].type==ct)
				{
						for (rx=-20; rx<21; rx++)
							for (ry=-20; ry<21; ry++)
								if (BOUNDS_CHECK && (rx || ry))
								{
									r = pmap[y+ry][x+rx];
									if (!r)
										continue;
									if(parts[r>>8].type==PT_NBOT && parts[r>>8].tmp!=6)
									{
										parts[r>>8].tmp3 = parts[i].x;
										parts[r>>8].tmp4 = parts[i].y;
									}
								}
						return 0;
				}
				if(parts[i].tmp==6 && parts[i].life==0)
				{
						for (rx=-20; rx<21; rx++)
							for (ry=-20; ry<21; ry++)
								if (BOUNDS_CHECK && (rx || ry))
								{
									r = pmap[y+ry][x+rx];
									if (!r)
										continue;
									
									parts[r>>8].tmp3 = 0;
									parts[r>>8].tmp4 = 0;

								}
						return 0;
				}

				if(parts[r>>8].type==PT_SPRK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp==256 || parts[r>>8].type==PT_VSPK && parts[r>>8].ctype==PT_NSCN && parts[i].tmp==256) //If NSCN is sparked, drop element
				{
					parts[i].vx = 0;
					parts[i].vy = 0;
					if(ct==PT_QRTZ || ct==PT_PQRT)
					{
						parts[i].tmp2 = 5;
						parts[i].tmp=1000;
					}
					else
					{
						parts[i].tmp2 = 0;
						parts[i].tmp=1000;
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
