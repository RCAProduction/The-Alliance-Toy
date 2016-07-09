#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_GALM PT_GALM 214
Element_GALM::Element_GALM()
{
	Identifier = "DEFAULT_PT_GALM";
	Name = "GALM";
	Colour = PIXPACK(0xB3B3B3);
	MenuVisible = 1;
	MenuSection = SC_ALLY;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 4.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 90;

	Temperature = R_TEMP-2.0f	+273.15f;
	HeatConduct = 255;
	Description = "Liquid Gallium";

	Properties = TYPE_LIQUID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_NEUTPASS;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = 283.15f;
	LowTemperatureTransition = PT_SGLM;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &Element_GALM::update;
}

//#TPT-Directive ElementHeader Element_GALM static int update(UPDATE_FUNC_ARGS)
int Element_GALM::update(UPDATE_FUNC_ARGS)
{
	unsigned char color, prevcol;
	prevcol = 0;
	if (parts[i].dcolour &0x00FFFFFF == 0)
	{
		parts[i].dcolour = 0xFFB3B3B3;
	}
	if (parts[i].tmp2 == 0)
		{
			color = rand() % 35 + 155;
			parts[i].dcolour = (0xFF000000 | color | color << 8 | color << 16);
			parts[i].tmp2 = 1;
			parts[i].pavg[2] = 300.0f;
		}
	else if (parts[i].tmp == 0)
		{
			prevcol = parts[i].dcolour & 0x000000FF;
			prevcol += 1;
			parts[i].dcolour = (0xFF000000 | prevcol | prevcol << 8 | prevcol << 16);
		}
	else if (parts[i].tmp == 1)
		{
			prevcol = parts[i].dcolour & 0x000000FF;
			prevcol -= 1;
			parts[i].dcolour = (0xFF000000 | prevcol | prevcol << 8 | prevcol << 16);
		}
	if (prevcol > 190)
	{
		parts[i].tmp = 1;
	}
	else if (prevcol < 150)
	{
		parts[i].tmp = 0;
	}
	

	
	float heat, aheat, pheat;

	heat = parts[i].temp;
	pheat = parts[i].pavg[2];
	aheat = ((heat - pheat)/4.0f);
	parts[i].temp = parts[i].pavg[2] + aheat;
	parts[i].pavg[2] = parts[i].temp;

	return 0;
}

Element_GALM::~Element_GALM() {}
