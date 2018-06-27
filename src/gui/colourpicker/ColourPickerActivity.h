#pragma once

#include <vector>
#include "common/String.h"
#include "Activity.h"
#include "gui/interface/Window.h"
#include "gui/interface/Textbox.h"
//#include "ColourPickerActivity.cpp"

#ifndef MY_HUD_RED // must be unique name in the project
#define MY_HUD_RED

extern int hudr; // prototype declaration of the function in a.cpp

#endif

class ColourPickedCallback
{
public:
	ColourPickedCallback() {}
	virtual  ~ColourPickedCallback() {}
	virtual void ColourPicked(ui::Colour colour) {}
};

class ColourPickerActivity: public WindowActivity {
	int currentHue;
	int currentSaturation;
	int currentValue;
	int currentAlpha;

	bool mouseDown;
	bool valueMouseDown;

	ui::Textbox * rValue;
	ui::Textbox * gValue;
	ui::Textbox * bValue;
	ui::Textbox * aValue;
	ui::Label * hexValue;

	ColourPickedCallback * callback;

	void UpdateTextboxes(int r, int g, int b, int a);
public:
	virtual void OnMouseMove(int x, int y, int dx, int dy);
	virtual void OnMouseDown(int x, int y, unsigned button);
	virtual void OnMouseUp(int x, int y, unsigned button);
	virtual void OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
	virtual void OnTryExit(ExitMethod method);
	ColourPickerActivity(ui::Colour initialColour, ColourPickedCallback * callback = NULL);
	virtual ~ColourPickerActivity();
	virtual void OnDraw();
};
