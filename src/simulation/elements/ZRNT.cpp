#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_ZRNT PT_ZRNT 210
Element_ZRNT::Element_ZRNT()
{
	Identifier = "DEFAULT_PT_ZRNT";
	Name = "ZRNT";
	Colour = PIXPACK(0x787A93);
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
	
	Weight = 150;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Zirconium Nitrade. Super conductor for VSPK.";

	Properties = TYPE_SOLID|PROP_CONDUCTS|PROP_LIFE_DEC|PROP_HOT_GLOW;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = NT;
	HighPressureTransition = ST;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 3273.0f;
	HighTemperatureTransition = PT_LAVA;
	
	Update = NULL;
	
}

Element_ZRNT::~Element_ZRNT() {}
