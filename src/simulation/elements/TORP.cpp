#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_TORP PT_TORP 217
Element_TORP::Element_TORP()
{
	Identifier = "DEFAULT_PT_TORP";
	Name = "TORP";
	Colour = PIXPACK(0x0E0E0E);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;

	Advection = 0;
	AirDrag = 0;
	AirLoss = 0;
	Loss = 1;
	Collision = 0;
	Gravity = 0.3;
	Diffusion = 0.00;
	HotAir = 0.000;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 90;

	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 0;
	Description = "PSCN and NSCN to aim, SPRK to fire.";

	Properties = TYPE_PART;
	
	Update = &Element_TORP::update;
}
//#TPT-Directive ElementHeader Element_TORP static int update(UPDATE_FUNC_ARGS)
int Element_TORP::update(UPDATE_FUNC_ARGS)
{
	int r, rx, ry;
	int aim = 0;
	if (parts[i].tmp == 1)
	{
		parts[i].tmp = 0;
	}
	for (rx = -2; rx < 3; rx++)
	for (ry = -2; ry < 3; ry++)
	if (BOUNDS_CHECK && (rx || ry))
	{
		r = pmap[y + ry][x + rx];
		if (!r)
			continue;
		if (parts[r>>8].type == PT_SPRK && parts[r>>8].life == 3)
		{
			if (parts[i].tmp == 1)
			{
				if (parts[r>>8].ctype == PT_PSCN)
				{
					aim++;
				}
				else if (parts[r>>8].ctype == PT_NSCN)
				{
					aim--;
				}
				else
				{
					parts[i].life = 1;
				}
			}
		}
	}
	//((aim+0.5)/(abs(aim)+0.5))*pow(10,abs(aim))
	return 0;
}
Element_TORP::~Element_TORP() {}