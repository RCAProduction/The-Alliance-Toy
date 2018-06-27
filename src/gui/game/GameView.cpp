#include <iomanip>

#include <ctime>
#include <stdio.h>      /* printf */
#include <time.h>       /* time_t, struct tm, time, localtime, asctime */

#include "graphics/Renderer.h"

#include <algorithm>
#include "GameView.h"


#include "Config.h"
#include "Favorite.h"
#include "Format.h"
#include "IntroText.h"
#include "QuickOptions.h"
#include "DecorationTool.h"
#include "client/SaveFile.h"
#include "graphics/Graphics.h"
#include "gui/Style.h"
#include "gui/dialogues/ConfirmPrompt.h"
#include "gui/dialogues/InformationMessage.h"
#include "gui/interface/Button.h"
#include "gui/interface/Colour.h"
#include "gui/interface/Keys.h"
#include "gui/interface/Mouse.h"
#include "gui/interface/Slider.h"
#include "gui/interface/Window.h"
#include "gui/search/Thumbnail.h"
#include "simulation/SaveRenderer.h"
#include "simulation/SimulationData.h"
//<<<<<<< HEAD
#include "gui/dialogues/ConfirmPrompt.h"
#include "client/SaveFile.h"
#include "Format.h"
#include "QuickOptions.h"
#include "IntroText.h"
#include "DecorationTool.h"

#include "gui/colourpicker/ColourPickerActivity.h"
#include "simulation/Simulation.h"
#include "gui/options/OptionsModel.h"

#include "Favorite.h"
//All above to HEAD was delete



class SplitButton;
class SplitButtonAction
{
public:
	virtual void ActionCallbackLeft(ui::Button * sender) {}
	virtual void ActionCallbackRight(ui::Button * sender) {}
	virtual ~SplitButtonAction() {}
};
class SplitButton : public ui::Button
{
private:
	bool rightDown;
	bool leftDown;
	bool showSplit;
	int splitPosition;
	String toolTip2;
	SplitButtonAction * splitActionCallback;
public:
	SplitButton(ui::Point position, ui::Point size, String buttonText, String toolTip, String toolTip2, int split) :
		Button(position, size, buttonText, toolTip),
		showSplit(true),
		splitPosition(split),
		toolTip2(toolTip2),
		splitActionCallback(NULL)
	{

	}
	void SetRightToolTip(String tooltip) { toolTip2 = tooltip; }
	bool GetShowSplit() { return showSplit; }
	void SetShowSplit(bool split) { showSplit = split; }
	SplitButtonAction * GetSplitActionCallback() { return splitActionCallback; }
	void SetSplitActionCallback(SplitButtonAction * newAction) { splitActionCallback = newAction; }
	void SetToolTip(int x, int y)
	{
		if(x >= splitPosition || !showSplit)
		{
			if(toolTip2.length()>0 && GetParentWindow())
			{
				GetParentWindow()->ToolTip(Position, toolTip2);
			}
		}
		else if(x < splitPosition)
		{
			if(toolTip.length()>0 && GetParentWindow())
			{
				GetParentWindow()->ToolTip(Position, toolTip);
			}
		}
	}
	virtual void OnMouseUnclick(int x, int y, unsigned int button)
	{
		if(isButtonDown)
		{
			if(leftDown)
				DoLeftAction();
			else if(rightDown)
				DoRightAction();
		}
		ui::Button::OnMouseUnclick(x, y, button);

	}
	virtual void OnMouseHover(int x, int y, int dx, int dy)
	{
		SetToolTip(x, y);
	}
	virtual void OnMouseHover(int x, int y)
	{
		SetToolTip(x, y);
	}
	virtual void OnMouseEnter(int x, int y)
	{
		isMouseInside = true;
		if(!Enabled)
			return;
		SetToolTip(x, y);
	}
	virtual void TextPosition(String ButtonText)
	{
		ui::Button::TextPosition(ButtonText);
		textPosition.X += 3;
	}
	void SetToolTips(String newToolTip1, String newToolTip2)
	{
		toolTip = newToolTip1;
		toolTip2 = newToolTip2;
	}
	virtual void OnMouseClick(int x, int y, unsigned int button)
	{
		ui::Button::OnMouseClick(x, y, button);
		rightDown = false;
		leftDown = false;
		if(x >= splitPosition)
			rightDown = true;
		else if(x < splitPosition)
			leftDown = true;
	}
	void DoRightAction()
	{
		if(!Enabled)
			return;
		if(splitActionCallback)
			splitActionCallback->ActionCallbackRight(this);
	}
	void DoLeftAction()
	{
		if(!Enabled)
			return;
		if(splitActionCallback)
			splitActionCallback->ActionCallbackLeft(this);
	}
	void Draw(const ui::Point& screenPos)
	{
		ui::Button::Draw(screenPos);
		Graphics * g = GetGraphics();
		drawn = true;

		if(showSplit)
			g->draw_line(splitPosition+screenPos.X, screenPos.Y+1, splitPosition+screenPos.X, screenPos.Y+Size.Y-2, 180, 180, 180, 255);
	}
	virtual ~SplitButton()
	{
		delete splitActionCallback;
	}
};


GameView::GameView():
	ui::Window(ui::Point(0, 0), ui::Point(WINDOWW, WINDOWH)),
	isMouseDown(false),

	elCount(false),
	FPSGvar(false),
	INFOvar(false),
	invertvar(false),
	fired(false),
	superDebug(false),
	skipDraw(false),
	zoomEnabled(false),
	zoomCursorFixed(false),
	mouseInZoom(false),
	drawSnap(false),
	shiftBehaviour(false),
	ctrlBehaviour(false),
	altBehaviour(false),
	showHud(true),
	showDebug(false),
	delayedActiveMenu(-1),
	wallBrush(false),
	toolBrush(false),
	decoBrush(false),
	windTool(false),
	toolIndex(0),
	currentSaveType(0),
	lastMenu(-1),
	pmessage(false),
	newsPause(false),

	toolTipPresence(0),
	toolTip(""),
	isToolTipFadingIn(false),
	toolTipPosition(-1, -1),
	infoTipPresence(0),
	infoTip(""),
	buttonTipShow(0),
	buttonTip(""),
	isButtonTipFadingIn(false),
	introText(2048),

	news(false),
	scroll(255),
	localServer(false),

	introTextMessage(ByteString(introTextData).FromAscii()),

	doScreenshot(false),
	screenshotIndex(0),
	recording(false),
	recordingFolder(0),
	recordingIndex(0),
	currentPoint(ui::Point(0, 0)),
	lastPoint(ui::Point(0, 0)),
	ren(NULL),
	activeBrush(NULL),
	saveSimulationButtonEnabled(false),
	saveReuploadAllowed(true),
	drawMode(DrawPoints),
	drawPoint1(0, 0),
	drawPoint2(0, 0),
	selectMode(SelectNone),
	selectPoint1(0, 0),
	selectPoint2(0, 0),
	currentMouse(0, 0),
	mousePosition(0, 0),
	placeSaveThumb(NULL),
	placeSaveOffset(0, 0)
{
	scrolling=0;

	FPS1=60;
	FPS2=60;
	FPS3=60;
	FPS4=60;
	FPS5=60;
	frameCount = 0;
	int currentX = 1;
	xMove = 0;
	
	pa20 = 0;
	pa19 = 0;
	pa18 = 0;
	pa17 = 0;
	pa16 = 0;
	pa15 = 0;
	pa14 = 0;
	pa13 = 0;
	pa12 = 0;
	pa11 = 0;
	pa10 = 0;
	pa9 = 0;
	pa8 = 0;
	pa7 = 0;
	pa6 = 0;
	pa5 = 0;
	pa4 = 0;
	pa3 = 0;
	pa2 = 0;
	pa1 = 0;
	
	targetx = 0;
	targety = 0;
	accelcount = 0;
	acceltimer = 0;
	
	int anarx = 0;
	int anary = 0;
	int st;

	factor = 1;
	
	//Set up UI
	class SearchAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		SearchAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			if(v->CtrlBehaviour())
				v->c->OpenLocalBrowse();
			else
				v->c->OpenSearch("");
		}
	};

	scrollBar = new ui::Button(ui::Point(0,YRES+21), ui::Point(XRES, 2), "");
	scrollBar->Appearance.BorderHover = ui::Colour(200, 200, 200);
	scrollBar->Appearance.BorderActive = ui::Colour(200, 200, 200);
	scrollBar->Appearance.HorizontalAlign = ui::Appearance::AlignCentre;
	scrollBar->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
	AddComponent(scrollBar);

	searchButton = new ui::Button(ui::Point(currentX, Size.Y-16), ui::Point(17, 15), "", "Find & open a simulation. Hold Ctrl to load offline saves.");  //Open
	searchButton->SetIcon(IconOpen);
	currentX+=18;
	searchButton->SetTogglable(false);
	searchButton->SetActionCallback(new SearchAction(this));
	AddComponent(searchButton);

	class ReloadAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		ReloadAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->ReloadSim();
		}
		void AltActionCallback(ui::Button * sender)
		{
			v->c->OpenSavePreview();
		}
	};
	reloadButton = new ui::Button(ui::Point(currentX, Size.Y-16), ui::Point(17, 15), "", "Reload the simulation");
	reloadButton->SetIcon(IconReload);
	reloadButton->Appearance.Margin.Left+=2;
	currentX+=18;
	reloadButton->SetActionCallback(new ReloadAction(this));
	AddComponent(reloadButton);

	class SaveSimulationAction : public SplitButtonAction
	{
		GameView * v;
	public:
		SaveSimulationAction(GameView * _v) { v = _v; }
		void ActionCallbackRight(ui::Button * sender)
		{
			if(v->CtrlBehaviour() || !Client::Ref().GetAuthUser().UserID)
				v->c->OpenLocalSaveWindow(false);
			else
				v->c->OpenSaveWindow();
		}
		void ActionCallbackLeft(ui::Button * sender)
		{
			if(v->CtrlBehaviour() || !Client::Ref().GetAuthUser().UserID)
				v->c->OpenLocalSaveWindow(true);
			else
				v->c->SaveAsCurrent();
		}
	};
	saveSimulationButton = new SplitButton(ui::Point(currentX, Size.Y-16), ui::Point(150, 15), "[untitled simulation]", "", "", 19);
	saveSimulationButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	saveSimulationButton->SetIcon(IconSave);
	currentX+=151;
	((SplitButton*)saveSimulationButton)->SetSplitActionCallback(new SaveSimulationAction(this));
	SetSaveButtonTooltips();
	AddComponent(saveSimulationButton);

	class UpVoteAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		UpVoteAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->Vote(1);
		}
	};
	upVoteButton = new ui::Button(ui::Point(currentX, Size.Y-16), ui::Point(39, 15), "", "Like this save");
	upVoteButton->SetIcon(IconVoteUp);
	upVoteButton->Appearance.Margin.Top+=2;
	upVoteButton->Appearance.Margin.Left+=2;
	currentX+=38;
	upVoteButton->SetActionCallback(new UpVoteAction(this));
	AddComponent(upVoteButton);

	class DownVoteAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		DownVoteAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->Vote(-1);
		}
	};
	downVoteButton = new ui::Button(ui::Point(currentX, Size.Y-16), ui::Point(15, 15), "", "Dislike this save");
	downVoteButton->SetIcon(IconVoteDown);
	downVoteButton->Appearance.Margin.Bottom+=2;
	downVoteButton->Appearance.Margin.Left+=2;
	currentX+=16;
	downVoteButton->SetActionCallback(new DownVoteAction(this));
	AddComponent(downVoteButton);

	class TagSimulationAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		TagSimulationAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->OpenTags();
		}
	};
	tagSimulationButton = new ui::Button(ui::Point(currentX, Size.Y-16), ui::Point(227, 15), "[no tags set]", "Add simulation tags");
	tagSimulationButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	tagSimulationButton->SetIcon(IconTag);
	//currentX+=252;
	tagSimulationButton->SetActionCallback(new TagSimulationAction(this));
	AddComponent(tagSimulationButton);

	class ClearSimAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		ClearSimAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->ClearSim();
		}
	};
	clearSimButton = new ui::Button(ui::Point(Size.X-159, Size.Y-16), ui::Point(17, 15), "", "Erase everything");
	clearSimButton->SetIcon(IconNew);
	clearSimButton->Appearance.Margin.Left+=2;
	clearSimButton->SetActionCallback(new ClearSimAction(this));
	AddComponent(clearSimButton);

	class LoginAction : public SplitButtonAction
	{
		GameView * v;
	public:
		LoginAction(GameView * _v) { v = _v; }
		void ActionCallbackLeft(ui::Button * sender)
		{
			v->c->OpenLogin();
		}
		void ActionCallbackRight(ui::Button * sender)
		{
			v->c->OpenProfile();
		}
	};
	loginButton = new SplitButton(ui::Point(Size.X-141, Size.Y-16), ui::Point(92, 15), "[sign in]", "Sign into simulation server", "Edit Profile", 19);
	loginButton->Appearance.HorizontalAlign = ui::Appearance::AlignLeft;
	loginButton->SetIcon(IconLogin);
	((SplitButton*)loginButton)->SetSplitActionCallback(new LoginAction(this));
	AddComponent(loginButton);

	class SimulationOptionAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		SimulationOptionAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->OpenOptions();
		}
	};
	simulationOptionButton = new ui::Button(ui::Point(Size.X-48, Size.Y-16), ui::Point(15, 15), "", "Simulation options");
	simulationOptionButton->SetIcon(IconSimulationSettings);
	simulationOptionButton->Appearance.Margin.Left+=2;
	simulationOptionButton->SetActionCallback(new SimulationOptionAction(this));
	AddComponent(simulationOptionButton);

	class DisplayModeAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		DisplayModeAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->OpenRenderOptions();
		}
	};
	displayModeButton = new ui::Button(ui::Point(Size.X-32, Size.Y-16), ui::Point(15, 15), "", "Renderer options");
	displayModeButton->SetIcon(IconRenderSettings);
	displayModeButton->Appearance.Margin.Left+=2;
	displayModeButton->SetActionCallback(new DisplayModeAction(this));
	AddComponent(displayModeButton);

	class PauseAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		PauseAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->SetPaused(sender->GetToggleState());
		}
	};
	pauseButton = new ui::Button(ui::Point(Size.X-16, Size.Y-16), ui::Point(15, 15), "", "Pause/Resume the simulation");  //Pause
	pauseButton->SetIcon(IconPause);
	pauseButton->SetTogglable(true);
	pauseButton->SetActionCallback(new PauseAction(this));
	AddComponent(pauseButton);

	class ElementSearchAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		ElementSearchAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->OpenElementSearch();
		}
	};
	
	
		class ServerAction : public ui::ButtonAction
	{
		//GameView * v;
	public:
		ServerAction(/*GameView * _v*/); //{ v = _v; }
		void ActionCallback(ui::Button * sender)
		{

		}
	};
		serverButton = new ui::Button(ui::Point(Size.X-177, Size.Y-16), ui::Point(17, 15), "S", "Switch Servers, normally on "+ SERVER);
	serverButton->SetTogglable(true); 
	AddComponent(serverButton);
	
	ui::Button * tempButton = new ui::Button(ui::Point(WINDOWW-16, WINDOWH-32), ui::Point(15, 15), 0xE065, "Search for elements");

	tempButton->Appearance.Margin = ui::Border(0, 2, 3, 2);
	tempButton->SetActionCallback(new ElementSearchAction(this));
	AddComponent(tempButton);

	class ColourPickerAction : public ui::ButtonAction
	{
		GameView * v;
	public:
		ColourPickerAction(GameView * _v) { v = _v; }
		void ActionCallback(ui::Button * sender)
		{
			v->c->OpenColourPicker();
		}
	};
	colourPicker = new ui::Button(ui::Point((XRES/2)-8, YRES+1), ui::Point(16, 16), "", "Pick Colour");
	colourPicker->SetActionCallback(new ColourPickerAction(this));
}

GameView::~GameView()
{
	if(!colourPicker->GetParentWindow())
		delete colourPicker;

	for(std::vector<ToolButton*>::iterator iter = colourPresets.begin(), end = colourPresets.end(); iter != end; ++iter)
	{
		ToolButton * button = *iter;
		if(!button->GetParentWindow())
		{
			delete button;
		}

	}

	delete placeSaveThumb;
}

class GameView::MenuAction: public ui::ButtonAction
{
	GameView * v;
public:
	int menuID;
	bool needsClick;
	MenuAction(GameView * _v, int menuID_)
	{
		v = _v;
		menuID = menuID_;
		/*if (menuID == SC_DECO)
			needsClick = true;
		else
			needsClick = false;*/
		needsClick=false;
	}
	void MouseEnterCallback(ui::Button * sender)
	{
		// don't immediately change the active menu, the actual set is done inside GameView::OnMouseMove
		// if we change it here it causes components to be removed, which causes the window to stop sending events
		// and then the previous menusection button never gets sent the OnMouseLeave event and is never unhighlighted
		if(!needsClick && !v->GetMouseDown())
			v->SetActiveMenuDelayed(menuID);
	}
	void ActionCallback(ui::Button * sender)
	{
		if (needsClick)
			v->c->SetActiveMenu(menuID);
		else
			MouseEnterCallback(sender);
	}
};

class GameView::OptionAction: public ui::ButtonAction
{
	QuickOption * option;
public:
	OptionAction(QuickOption * _option) { option = _option; }
	void ActionCallback(ui::Button * sender)
	{
		option->Perform();
	}
};

class GameView::OptionListener: public QuickOptionListener
{
	ui::Button * button;
public:
	OptionListener(ui::Button * _button) { button = _button; }
	virtual void OnValueChanged(QuickOption * option)
	{
		switch(option->GetType())
		{
		case QuickOption::Toggle:
			button->SetTogglable(true);
			button->SetToggleState(option->GetToggle());
			break;
		default:
			break;
		}
	}
};

class GameView::ToolAction: public ui::ButtonAction
{
	GameView * v;
public:
	Tool * tool;
	ToolAction(GameView * _v, Tool * tool_) { v = _v; tool = tool_; }
	void ActionCallback(ui::Button * sender_)
	{
		ToolButton *sender = (ToolButton*)sender_;
		if (v->ShiftBehaviour() && v->CtrlBehaviour() && !v->AltBehaviour())
		{
			if (Favorite::Ref().IsFavorite(tool->GetIdentifier()) && sender->GetSelectionState() == 1)
				Favorite::Ref().RemoveFavorite(tool->GetIdentifier());
			else if (sender->GetSelectionState() == 0)
				Favorite::Ref().AddFavorite(tool->GetIdentifier());
			else if (sender->GetSelectionState() == 2)
				v->c->SetActiveMenu(SC_FAVORITES);

			v->c->RebuildFavoritesMenu();
		}
		else
		{
			if (v->CtrlBehaviour() && v->AltBehaviour() && !v->ShiftBehaviour())
				if (tool->GetIdentifier().BeginsWith("DEFAULT_PT_"))
					sender->SetSelectionState(3);

			if (sender->GetSelectionState() >= 0 && sender->GetSelectionState() <= 3)
				v->c->SetActiveTool(sender->GetSelectionState(), tool);
		}
	}
};

void GameView::NotifyQuickOptionsChanged(GameModel * sender)
{
	for (size_t i = 0; i < quickOptionButtons.size(); i++)
	{
		RemoveComponent(quickOptionButtons[i]);
		delete quickOptionButtons[i];
	}

	int currentY = 1;
	vector<QuickOption*> optionList = sender->GetQuickOptions();
	for(vector<QuickOption*>::iterator iter = optionList.begin(), end = optionList.end(); iter != end; ++iter)
	{
		QuickOption * option = *iter;
		ui::Button * tempButton = new ui::Button(ui::Point(WINDOWW-16, currentY), ui::Point(15, 15), option->GetIcon(), option->GetDescription());
		//tempButton->Appearance.Margin = ui::Border(0, 2, 3, 2);
		tempButton->SetTogglable(true);
		tempButton->SetActionCallback(new OptionAction(option));
		option->AddListener(new OptionListener(tempButton));
		AddComponent(tempButton);

		quickOptionButtons.push_back(tempButton);
		currentY += 16;
	}
}

void GameView::NotifyMenuListChanged(GameModel * sender)
{
	int currentY = WINDOWH-48;//-(sender->GetMenuList().size()*16);
	for (size_t i = 0; i < menuButtons.size(); i++)
	{
		RemoveComponent(menuButtons[i]);
		delete menuButtons[i];
	}
	menuButtons.clear();
	for (size_t i = 0; i < toolButtons.size(); i++)
	{
		RemoveComponent(toolButtons[i]);
		delete toolButtons[i];
	}
	toolButtons.clear();
	vector<Menu*> menuList = sender->GetMenuList();
	for (int i = (int)menuList.size()-1; i >= 0; i--)
	{
		if (menuList[i]->GetVisible())
		{
			String tempString = "";
			tempString += menuList[i]->GetIcon();
			String description = menuList[i]->GetDescription();
			if (i == SC_FAVORITES && Favorite::Ref().AnyFavorites())
				description += " (Use ctrl+shift+click to favorite an element)";
			ui::Button * tempButton = new ui::Button(ui::Point(WINDOWW-16, currentY), ui::Point(15, 15), tempString, description);
			tempButton->Appearance.Margin = ui::Border(0, 2, 3, 2);
			tempButton->SetTogglable(true);
			tempButton->SetActionCallback(new MenuAction(this, i));
			currentY-=16;
			AddComponent(tempButton);
			menuButtons.push_back(tempButton);
		}
	}
}

void GameView::SetSample(SimulationSample sample)
{
	this->sample = sample;
}

void GameView::SetHudEnable(bool hudState)
{
	showHud = hudState;
}

bool GameView::GetHudEnable()
{
	return showHud;
}

void GameView::SetDebugHUD(bool mode)
{
	showDebug = mode;
	if (ren)
		ren->debugLines = showDebug;
}

bool GameView::GetDebugHUD()
{
	return showDebug;
}

ui::Point GameView::GetMousePosition()
{
	return currentMouse;
}

bool GameView::GetPlacingSave()
{
	return selectMode != SelectNone;
}

bool GameView::GetPlacingZoom()
{
	return zoomEnabled && !zoomCursorFixed;
}

void GameView::NotifyActiveToolsChanged(GameModel * sender)
{
	decoBrush = false;
	for (size_t i = 0; i < toolButtons.size(); i++)
	{
		Tool * tool = ((ToolAction*)toolButtons[i]->GetActionCallback())->tool;
		if(sender->GetActiveTool(0) == tool)
		{
			toolButtons[i]->SetSelectionState(0);	//Primary
			windTool = tool->GetIdentifier() == "DEFAULT_UI_WIND";

			if (sender->GetActiveTool(0)->GetIdentifier().BeginsWith("DEFAULT_DECOR_"))
				decoBrush = true;
		}
		else if(sender->GetActiveTool(1) == tool)
		{
			toolButtons[i]->SetSelectionState(1);	//Secondary
			if (sender->GetActiveTool(1)->GetIdentifier().BeginsWith("DEFAULT_DECOR_"))
				decoBrush = true;
		}
		else if(sender->GetActiveTool(2) == tool)
		{
			toolButtons[i]->SetSelectionState(2);	//Tertiary
		}
		else if(sender->GetActiveTool(3) == tool)
		{
			toolButtons[i]->SetSelectionState(3);	//Replace Mode
		}
		else
		{
			toolButtons[i]->SetSelectionState(-1);
		}
	}
	//need to do this for all tools every time just in case it wasn't caught if you weren't in the menu a tool was changed to
	c->ActiveToolChanged(0, sender->GetActiveTool(0));
	if (sender->GetRenderer()->findingElement)
	{
		Tool *active = sender->GetActiveTool(0);
		if (!active->GetIdentifier().Contains("_PT_"))
			ren->findingElement = 0;
		else
			ren->findingElement = sender->GetActiveTool(0)->GetToolID()%256;
	}
	c->ActiveToolChanged(1, sender->GetActiveTool(1));
	c->ActiveToolChanged(2, sender->GetActiveTool(2));
	c->ActiveToolChanged(3, sender->GetActiveTool(3));
}

void GameView::NotifyLastToolChanged(GameModel * sender)
{
	if (sender->GetLastTool())
	{
		wallBrush = sender->GetLastTool()->GetBlocky();
		toolBrush = sender->GetLastTool()->GetIdentifier().BeginsWith("DEFAULT_TOOL_");
	}
}

void GameView::NotifyToolListChanged(GameModel * sender)
{
	//Set the menu scroll / start of menu position here. Fixes the first element being cut off.
	int currentX = WINDOWW-70;
//HEAD This wanted to be cut out in the merge ^
	for (size_t i = 0; i < menuButtons.size(); i++)
	{
		if (((MenuAction*)menuButtons[i]->GetActionCallback())->menuID==sender->GetActiveMenu())
		{
			menuButtons[i]->SetToggleState(true);
		}
		else
		{
			menuButtons[i]->SetToggleState(false);
		}
	}
	for (size_t i = 0; i < toolButtons.size(); i++)
	{
		RemoveComponent(toolButtons[i]);
		delete toolButtons[i];
	}
	toolButtons.clear();
	vector<Tool*> toolList = sender->GetToolList();
	int currentX = 0;
	for (size_t i = 0; i < toolList.size(); i++)
	{
		VideoBuffer * tempTexture = toolList[i]->GetTexture(26, 14);
		ToolButton * tempButton;

		//get decotool texture manually, since it changes depending on it's own color
		if (sender->GetActiveMenu() == SC_DECO)
			tempTexture = ((DecorationTool*)toolList[i])->GetIcon(toolList[i]->GetToolID(), 26, 14);

		if(tempTexture)
			tempButton = new ToolButton(ui::Point(currentX, YRES+1), ui::Point(30, 18), "", toolList[i]->GetIdentifier(), toolList[i]->GetDescription());
		else
			tempButton = new ToolButton(ui::Point(currentX, YRES+1), ui::Point(30, 18), toolList[i]->GetName(), toolList[i]->GetIdentifier(), toolList[i]->GetDescription());

		//currentY -= 17;
		currentX -= 31;
		tempButton->SetActionCallback(new ToolAction(this, toolList[i]));

		tempButton->Appearance.SetTexture(tempTexture);
		delete tempTexture;

		tempButton->Appearance.BackgroundInactive = ui::Colour(toolList[i]->colRed, toolList[i]->colGreen, toolList[i]->colBlue);

		if(sender->GetActiveTool(0) == toolList[i])
		{
			tempButton->SetSelectionState(0);	//Primary
		}
		else if(sender->GetActiveTool(1) == toolList[i])
		{
			tempButton->SetSelectionState(1);	//Secondary
		}
		else if(sender->GetActiveTool(2) == toolList[i])
		{
			tempButton->SetSelectionState(2);	//Tertiary
		}
		else if(sender->GetActiveTool(3) == toolList[i])
		{
			tempButton->SetSelectionState(3);	//Replace mode
		}

		tempButton->Appearance.HorizontalAlign = ui::Appearance::AlignCentre;
		tempButton->Appearance.VerticalAlign = ui::Appearance::AlignMiddle;
		AddComponent(tempButton);
		toolButtons.push_back(tempButton);
	}
	if (sender->GetActiveMenu() != SC_DECO)
		lastMenu = sender->GetActiveMenu();

	updateToolButtonScroll();
}

void GameView::NotifyColourSelectorVisibilityChanged(GameModel * sender)
{
	for(std::vector<ToolButton*>::iterator iter = colourPresets.begin(), end = colourPresets.end(); iter != end; ++iter)
	{
		ToolButton * button = *iter;
		RemoveComponent(button);
		button->SetParentWindow(NULL);
	}

	RemoveComponent(colourPicker);
	colourPicker->SetParentWindow(NULL);

	if(sender->GetColourSelectorVisibility())
	{
		for(std::vector<ToolButton*>::iterator iter = colourPresets.begin(), end = colourPresets.end(); iter != end; ++iter)
		{
			ToolButton * button = *iter;
			AddComponent(button);
		}
		AddComponent(colourPicker);
		c->SetActiveColourPreset(-1);
	}
}

void GameView::NotifyColourPresetsChanged(GameModel * sender)
{
	class ColourPresetAction: public ui::ButtonAction
	{
		GameView * v;
	public:
		int preset;
		ColourPresetAction(GameView * _v, int preset) : preset(preset) { v = _v; }
		void ActionCallback(ui::Button * sender_)
		{
			v->c->SetActiveColourPreset(preset);
			v->c->SetColour(sender_->Appearance.BackgroundInactive);
		}
	};


	for(std::vector<ToolButton*>::iterator iter = colourPresets.begin(), end = colourPresets.end(); iter != end; ++iter)
	{
		ToolButton * button = *iter;
		RemoveComponent(button);
		delete button;
	}
	colourPresets.clear();

	int currentX = 5;
	std::vector<ui::Colour> colours = sender->GetColourPresets();
	int i = 0;
	for(std::vector<ui::Colour>::iterator iter = colours.begin(), end = colours.end(); iter != end; ++iter)
	{
		ToolButton * tempButton = new ToolButton(ui::Point(currentX, YRES+1), ui::Point(30, 18), "", "", "Decoration Presets.");
		tempButton->Appearance.BackgroundInactive = *iter;
		tempButton->SetActionCallback(new ColourPresetAction(this, i));

		currentX += 31;

		if(sender->GetColourSelectorVisibility())
			AddComponent(tempButton);
		colourPresets.push_back(tempButton);

		i++;
	}
	NotifyColourActivePresetChanged(sender);
}

void GameView::NotifyColourActivePresetChanged(GameModel * sender)
{
	for (size_t i = 0; i < colourPresets.size(); i++)
	{
		if (sender->GetActiveColourPreset() == i)
		{
			colourPresets[i]->SetSelectionState(0);	//Primary
		}
		else
		{
			colourPresets[i]->SetSelectionState(-1);
		}
	}
}

void GameView::NotifyColourSelectorColourChanged(GameModel * sender)
{
	colourPicker->Appearance.BackgroundInactive = sender->GetColourSelectorColour();
	colourPicker->Appearance.BackgroundHover = sender->GetColourSelectorColour();
	NotifyToolListChanged(sender);
}

void GameView::NotifyRendererChanged(GameModel * sender)
{
	ren = sender->GetRenderer();
}

void GameView::NotifySimulationChanged(GameModel * sender)
{

}
void GameView::NotifyUserChanged(GameModel * sender)
{
	if(!sender->GetUser().UserID)
	{
		loginButton->SetText("[sign in]");
		((SplitButton*)loginButton)->SetShowSplit(false);
		((SplitButton*)loginButton)->SetRightToolTip("Sign in to simulation server");
	}
	else
	{
		loginButton->SetText(sender->GetUser().Username.FromUtf8());
		((SplitButton*)loginButton)->SetShowSplit(true);
		((SplitButton*)loginButton)->SetRightToolTip("Edit profile");
	}
	// saveSimulationButtonEnabled = sender->GetUser().ID;
	saveSimulationButtonEnabled = true;
	NotifySaveChanged(sender);
}


void GameView::NotifyPausedChanged(GameModel * sender)
{
	pauseButton->SetToggleState(sender->GetPaused());
}

void GameView::NotifyToolTipChanged(GameModel * sender)
{
	toolTip = sender->GetToolTip();
}

void GameView::NotifyInfoTipChanged(GameModel * sender)
{
	infoTip = sender->GetInfoTip();
	infoTipPresence = 120;
}

void GameView::NotifySaveChanged(GameModel * sender)
{
	saveReuploadAllowed = true;
	if (sender->GetSave())
	{
		if (introText > 50)
			introText = 50;

		saveSimulationButton->SetText(sender->GetSave()->GetName());
		if (sender->GetSave()->GetUserName() == sender->GetUser().Username)
			((SplitButton*)saveSimulationButton)->SetShowSplit(true);
		else
			((SplitButton*)saveSimulationButton)->SetShowSplit(false);
		reloadButton->Enabled = true;
		upVoteButton->Enabled = (sender->GetSave()->GetID() && sender->GetUser().UserID && sender->GetSave()->GetVote()==0);
		if(sender->GetSave()->GetID() && sender->GetUser().UserID && sender->GetSave()->GetVote()==1)
			upVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 108, 10, 255));
		else
			upVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));

		downVoteButton->Enabled = upVoteButton->Enabled;
		if (sender->GetSave()->GetID() && sender->GetUser().UserID && sender->GetSave()->GetVote()==-1)
			downVoteButton->Appearance.BackgroundDisabled = (ui::Colour(108, 0, 10, 255));
		else
			downVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));

		if (sender->GetUser().UserID)
		{
			upVoteButton->Appearance.BorderDisabled = upVoteButton->Appearance.BorderInactive;
			downVoteButton->Appearance.BorderDisabled = downVoteButton->Appearance.BorderInactive;
		}
		else
		{
			upVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100);
			downVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100);
		}

		tagSimulationButton->Enabled = sender->GetSave()->GetID();
		if (sender->GetSave()->GetID())
		{
			StringBuilder tagsStream;
			std::list<ByteString> tags = sender->GetSave()->GetTags();
			if (tags.size())
			{
				for (std::list<ByteString>::const_iterator iter = tags.begin(), begin = tags.begin(), end = tags.end(); iter != end; iter++)
				{
					if (iter != begin)
						tagsStream << " ";
					tagsStream << iter->FromUtf8();
				}
				tagSimulationButton->SetText(tagsStream.Build());
			}
			else
			{
				tagSimulationButton->SetText("[set search tags here]");
			}
		}
		else
		{
			tagSimulationButton->SetText("[set search tags here]");
		}
		currentSaveType = 1;
		int saveID = sender->GetSave()->GetID();
		if (saveID == 404 || saveID == 2157797)
			saveReuploadAllowed = false;
	}
	else if (sender->GetSaveFile())
	{
		if (ctrlBehaviour)
			((SplitButton*)saveSimulationButton)->SetShowSplit(true);
		else
			((SplitButton*)saveSimulationButton)->SetShowSplit(false);
		saveSimulationButton->SetText(sender->GetSaveFile()->GetDisplayName());
		reloadButton->Enabled = true;
		upVoteButton->Enabled = false;
		upVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));
		upVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100);
		downVoteButton->Enabled = false;
		downVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));
		downVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100);
		tagSimulationButton->Enabled = false;
		tagSimulationButton->SetText("[set search tags here]");
		currentSaveType = 2;
	}
	else
	{
		((SplitButton*)saveSimulationButton)->SetShowSplit(false);
		saveSimulationButton->SetText("[save simulation]");
		reloadButton->Enabled = false;
		upVoteButton->Enabled = false;
		upVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));
		upVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100),
		downVoteButton->Enabled = false;
		downVoteButton->Appearance.BackgroundDisabled = (ui::Colour(0, 0, 0));
		downVoteButton->Appearance.BorderDisabled = ui::Colour(100, 100, 100),
		tagSimulationButton->Enabled = false;
		tagSimulationButton->SetText("[set search tags here]");
		currentSaveType = 0;
	}
	saveSimulationButton->Enabled = (saveSimulationButtonEnabled && saveReuploadAllowed) || ctrlBehaviour;
	SetSaveButtonTooltips();
}

void GameView::NotifyBrushChanged(GameModel * sender)
{
	activeBrush = sender->GetBrush();
}

void GameView::screenshot()
{
	doScreenshot = true;
}

int GameView::Record(bool record)
{
	if (!record)
	{
		recording = false;
		recordingIndex = 0;
		recordingFolder = 0;
	}
	else if (!recording)
	{
		// block so that the return value is correct
		bool record = ConfirmPrompt::Blocking("Recording", "You're about to start recording all drawn frames. This will use a load of disk space.");
		if (record)
		{
			time_t startTime = time(NULL);
			recordingFolder = startTime;
			Client::Ref().MakeDirectory("recordings");
			Client::Ref().MakeDirectory(ByteString::Build("recordings", PATH_SEP, recordingFolder).c_str());
			recording = true;
			recordingIndex = 0;
		}
	}
	return recordingFolder;
}

void GameView::updateToolButtonScroll()
{
	if(toolButtons.size())
	{
		int x = currentMouse.X, y = currentMouse.Y;
		int newInitialX = WINDOWW-56;
		int totalWidth = (toolButtons[0]->Size.X+1)*toolButtons.size();
		int scrollSize = (int)(((float)(XRES-BARSIZE))/((float)totalWidth) * ((float)XRES-BARSIZE));
		if (scrollSize>XRES-1)
			scrollSize = XRES-1;
		if(totalWidth > XRES-15)
		{
			int mouseX = x;
			if(mouseX > XRES)
				mouseX = XRES;
			//if (mouseX < 15) //makes scrolling a little nicer at edges but apparently if you put hundreds of elements in a menu it makes the end not show ...
			//	mouseX = 15;

			scrollBar->Position.X = (int)(((float)mouseX/((float)XRES))*(float)(XRES-scrollSize));

			float overflow = totalWidth-(XRES-BARSIZE), mouseLocation = float(XRES-3)/float((XRES-2)-mouseX); //mouseLocation adjusted slightly in case you have 200 elements in one menu

			newInitialX += overflow/mouseLocation;
		}
		else
		{
			scrollBar->Position.X = 1;
		}
		scrollBar->Size.X=scrollSize;
		int offsetDelta = toolButtons[0]->Position.X - newInitialX;
		for(vector<ToolButton*>::iterator iter = toolButtons.begin(), end = toolButtons.end(); iter!=end; ++iter)
		{
			ToolButton * button = *iter;
			button->Position.X -= offsetDelta;
			if (button->Position.X+button->Size.X <= 0 || (button->Position.X+button->Size.X) > XRES-2)
				button->Visible = false;
			else
				button->Visible = true;
		}

		//Ensure that mouseLeave events are make their way to the buttons should they move from underneath the mouse pointer
		if(toolButtons[0]->Position.Y < y && toolButtons[0]->Position.Y+toolButtons[0]->Size.Y > y)
		{
			for(vector<ToolButton*>::iterator iter = toolButtons.begin(), end = toolButtons.end(); iter!=end; ++iter)
			{
				ToolButton * button = *iter;
				if(button->Position.X < x && button->Position.X+button->Size.X > x)
					button->OnMouseEnter(x, y);
				else
					button->OnMouseLeave(x, y);
			}
		}
	}
}

void GameView::OnMouseMove(int x, int y, int dx, int dy)
{
	bool newMouseInZoom = c->MouseInZoom(ui::Point(x, y));
	mousePosition = c->PointTranslate(ui::Point(x, y));
	currentMouse = ui::Point(x, y);
	if (selectMode != SelectNone)
	{
		if (selectMode == PlaceSave)
			selectPoint1 = c->PointTranslate(ui::Point(x, y));
		if (selectPoint1.X != -1)
			selectPoint2 = c->PointTranslate(ui::Point(x, y));
	}
	else if (isMouseDown)
	{
		if (newMouseInZoom == mouseInZoom)
		{
			if (drawMode == DrawPoints)
			{
				currentPoint = mousePosition;
				c->DrawPoints(toolIndex, lastPoint, currentPoint, true);
				lastPoint = currentPoint;
				skipDraw = true;
			}
			else if (drawMode == DrawFill)
			{
				c->DrawFill(toolIndex, mousePosition);
				skipDraw = true;
			}
		}
		else if (drawMode == DrawPoints || drawMode == DrawFill)
		{
			isMouseDown = false;
			drawMode = DrawPoints;
			c->MouseUp(x, y, 0, 2);
		}
	}
	mouseInZoom = newMouseInZoom;

	// set active menu (delayed)
	if (delayedActiveMenu != -1)
	{
		c->SetActiveMenu(delayedActiveMenu);
		delayedActiveMenu = -1;
	}

	updateToolButtonScroll();
}

void GameView::OnMouseDown(int x, int y, unsigned button)
{
	currentMouse = ui::Point(x, y);
	if (altBehaviour && !shiftBehaviour && !ctrlBehaviour)
		button = SDL_BUTTON_MIDDLE;
	if  (!(zoomEnabled && !zoomCursorFixed))
	{
		if (selectMode != SelectNone)
		{
			isMouseDown = true;
			if (button == SDL_BUTTON_LEFT && selectPoint1.X == -1)
			{
				selectPoint1 = c->PointTranslate(currentMouse);
				selectPoint2 = selectPoint1;
			}
			return;
		}
		if (currentMouse.X >= 0 && currentMouse.X < XRES && currentMouse.Y >= 0 && currentMouse.Y < YRES)
		{
			// update tool index, set new "last" tool so GameView can detect certain tools properly
			if (button == SDL_BUTTON_LEFT)
				toolIndex = 0;
			if (button == SDL_BUTTON_RIGHT)
				toolIndex = 1;
			if (button == SDL_BUTTON_MIDDLE)
				toolIndex = 2;
			Tool *lastTool = c->GetActiveTool(toolIndex);
			c->SetLastTool(lastTool);
			UpdateDrawMode();

			isMouseDown = true;
			c->HistorySnapshot();
			if (drawMode == DrawRect || drawMode == DrawLine)
			{
				drawPoint1 = c->PointTranslate(currentMouse);
			}
			else if (drawMode == DrawPoints)
			{
				lastPoint = currentPoint = c->PointTranslate(currentMouse);
				c->DrawPoints(toolIndex, lastPoint, currentPoint, false);
			}
			else if (drawMode == DrawFill)
			{
				c->DrawFill(toolIndex, c->PointTranslate(currentMouse));
			}
		}
	}
}

void GameView::OnMouseUp(int x, int y, unsigned button)
{
	currentMouse = ui::Point(x, y);
	if (zoomEnabled && !zoomCursorFixed)
	{
		zoomCursorFixed = true;
		drawMode = DrawPoints;
		isMouseDown = false;
	}
	else if (isMouseDown)
	{
		isMouseDown = false;
		if (selectMode != SelectNone)
		{
			if (button == SDL_BUTTON_LEFT && selectPoint1.X != -1 && selectPoint1.Y != -1 && selectPoint2.X != -1 && selectPoint2.Y != -1)
			{
				if (selectMode == PlaceSave)
				{
					if (placeSaveThumb && y <= WINDOWH-BARSIZE)
					{
						int thumbX = selectPoint2.X - ((placeSaveThumb->Width-placeSaveOffset.X)/2);
						int thumbY = selectPoint2.Y - ((placeSaveThumb->Height-placeSaveOffset.Y)/2);

						if (thumbX < 0)
							thumbX = 0;
						if (thumbX+(placeSaveThumb->Width) >= XRES)
							thumbX = XRES-placeSaveThumb->Width;

						if (thumbY < 0)
							thumbY = 0;
						if (thumbY+(placeSaveThumb->Height) >= YRES)
							thumbY = YRES-placeSaveThumb->Height;

						c->PlaceSave(ui::Point(thumbX, thumbY), !shiftBehaviour);
					}
				}
				else
				{
					int x2 = (selectPoint1.X>selectPoint2.X) ? selectPoint1.X : selectPoint2.X;
					int y2 = (selectPoint1.Y>selectPoint2.Y) ? selectPoint1.Y : selectPoint2.Y;
					int x1 = (selectPoint2.X<selectPoint1.X) ? selectPoint2.X : selectPoint1.X;
					int y1 = (selectPoint2.Y<selectPoint1.Y) ? selectPoint2.Y : selectPoint1.Y;
					if (selectMode ==SelectCopy)
						c->CopyRegion(ui::Point(x1, y1), ui::Point(x2, y2), !shiftBehaviour);
					else if (selectMode == SelectCut)
						c->CutRegion(ui::Point(x1, y1), ui::Point(x2, y2), !shiftBehaviour);
					else if (selectMode == SelectStamp)
						c->StampRegion(ui::Point(x1, y1), ui::Point(x2, y2), !shiftBehaviour);
				}
			}
			selectMode = SelectNone;
			return;
		}

		ui::Point finalDrawPoint2 = c->PointTranslate(currentMouse);
		if (drawMode == DrawRect || drawMode == DrawLine)
		{
			drawPoint2 = finalDrawPoint2;
			if (drawSnap && drawMode == DrawLine)
			{
				finalDrawPoint2 = lineSnapCoords(c->PointTranslate(drawPoint1), drawPoint2);
			}
			if (drawSnap && drawMode == DrawRect)
			{
				finalDrawPoint2 = rectSnapCoords(c->PointTranslate(drawPoint1), drawPoint2);
			}

			if (drawMode == DrawRect)
			{
				c->DrawRect(toolIndex, c->PointTranslate(drawPoint1), finalDrawPoint2);
			}
			if (drawMode == DrawLine)
			{
				c->DrawLine(toolIndex, c->PointTranslate(drawPoint1), finalDrawPoint2);
			}
		}
		else if (drawMode == DrawPoints)
		{
			// draw final line
			c->DrawPoints(toolIndex, lastPoint, finalDrawPoint2, true);
			// plop tool stuff (like STKM)
			c->ToolClick(toolIndex, finalDrawPoint2);
		}
		else if (drawMode == DrawFill)
		{
			c->DrawFill(toolIndex, finalDrawPoint2);
		}
	}
	// this shouldn't happen, but do this just in case
	else if (selectMode != SelectNone && button != SDL_BUTTON_LEFT)
		selectMode = SelectNone;

	// update the drawing mode for the next line
	// since ctrl/shift state may have changed since we started drawing
	UpdateDrawMode();
}

void GameView::ToolTip(ui::Point senderPosition, String toolTip)
{
	// buttom button tooltips
	if (senderPosition.Y > Size.Y-17)
	{
		if (selectMode == PlaceSave || selectMode == SelectNone)
		{
			buttonTip = toolTip;
			isButtonTipFadingIn = true;
		}
	}
	// quickoption and menu tooltips
	else if(senderPosition.X > Size.X-BARSIZE)// < Size.Y-(quickOptionButtons.size()+1)*16)
	{
		this->toolTip = toolTip;
		toolTipPosition = ui::Point(Size.X-27-Graphics::textwidth(toolTip), senderPosition.Y+3);
		if(toolTipPosition.Y+10 > Size.Y-MENUSIZE)
			toolTipPosition = ui::Point(Size.X-27-Graphics::textwidth(toolTip), Size.Y-MENUSIZE-10);
		isToolTipFadingIn = true;
	}
	// element tooltips
	else
	{
		this->toolTip = toolTip;
		toolTipPosition = ui::Point(Size.X-27-Graphics::textwidth(toolTip), Size.Y-MENUSIZE-10);
		isToolTipFadingIn = true;
	}
}

void GameView::OnMouseWheel(int x, int y, int d)
{
	if (!d)
		return;
	if (news==true)
	{
		scrolling=scrolling+(-d*12);
		return;
	}
	if (selectMode != SelectNone)
	{
		return;
	}
	if (zoomEnabled && !zoomCursorFixed)
	{
		c->AdjustZoomSize(d);
	}
	else
	{
		c->AdjustBrushSize(d, false, shiftBehaviour, ctrlBehaviour);
	}
}

void GameView::BeginStampSelection()
{
	selectMode = SelectStamp;
	selectPoint1 = selectPoint2 = ui::Point(-1, -1);
	isMouseDown = false;
	buttonTip = "\x0F\xEF\xEF\020Click-and-drag to specify an area to create a stamp (right click = cancel)";
	buttonTipShow = 120;
}

void GameView::OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if (introText > 50)
	{
		introText = 50;
	}

	if (selectMode != SelectNone)
	{
		if (selectMode == PlaceSave)
		{
			switch (key)
			{
			case SDLK_RIGHT:
				c->TranslateSave(ui::Point(1, 0));
				return;
			case SDLK_LEFT:
				c->TranslateSave(ui::Point(-1, 0));
				return;
			case SDLK_UP:
				c->TranslateSave(ui::Point(0, -1));
				return;
			case SDLK_DOWN:
				c->TranslateSave(ui::Point(0, 1));
				return;
			case 'r':
				if (repeat)
					return;
				if (ctrl && shift)
				{
					//Vertical flip
					c->TransformSave(m2d_new(1,0,0,-1));
				}
				else if (!ctrl && shift)
				{
					//Horizontal flip
					c->TransformSave(m2d_new(-1,0,0,1));
				}
				else
				{
					//Rotate 90deg
					c->TransformSave(m2d_new(0,1,-1,0));
				}
				return;
			}
		}
	}

	if (repeat)
		return;
	if (scan == SDL_SCANCODE_GRAVE)
	{
		c->ShowConsole();
		return;
	}
	switch(key)
	{
	case SDLK_LALT:
	case SDLK_RALT:
		enableAltBehaviour();
		break;
	case SDLK_LCTRL:
	case SDLK_RCTRL:
		enableCtrlBehaviour();
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		enableShiftBehaviour();
		break;
	case ' ': //Space
		if (news==true)
		{
			newsPause=!newsPause;
		}
		c->SetPaused();
		break;
	case 'z':
		if (selectMode != SelectNone && isMouseDown)
			break;
		if (ctrl && !isMouseDown)
		{
			if (shift)
				c->HistoryForward();
			else
				c->HistoryRestore();
		}
		else
		{
			isMouseDown = false;
			zoomCursorFixed = false;
			c->SetZoomEnabled(true);
		}
		break;
	case SDLK_TAB: //Tab
		c->ChangeBrush();
		break;
	case 'p':

	case SDLK_F2:
		if (ctrl)
		{
/*<<<<<<< HEAD
			elCount = !elCount;
		}
		else
		{
			screenshot();
		}

=======*/
			if (shift)
				c->SetActiveTool(1, "DEFAULT_UI_PROPERTY");
			else
				c->SetActiveTool(0, "DEFAULT_UI_PROPERTY");
		}
		else
			screenshot();
//>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945
		break;
	case SDLK_F3:
		SetDebugHUD(!GetDebugHUD());
		break;
	case SDLK_F5:
		c->ReloadSim();
		break;
	case 'a':
		if ((Client::Ref().GetAuthUser().UserElevation == User::ElevationModerator
		     || Client::Ref().GetAuthUser().UserElevation == User::ElevationAdmin) && ctrl)
		{
			ByteString authorString = Client::Ref().GetAuthorInfo().toStyledString();
			new InformationMessage("Save authorship info", authorString.FromUtf8(), true);
		}
		break;
	case 'r':
		if (ctrl)
			c->ReloadSim();
		break;
	case 'e':
		c->OpenElementSearch();
		break;
	case 'f':
		if (ctrl)
		{
			Tool *active = c->GetActiveTool(0);
			if (!active->GetIdentifier().Contains("_PT_") || ren->findingElement == active->GetToolID()%256)
				ren->findingElement = 0;
			else
				ren->findingElement = active->GetToolID()%256;
		}
		else
			c->FrameStep();
		break;
	case 'g':
		if (ctrl)
			c->ShowGravityGrid();
		else if(shift)
			c->AdjustGridSize(-1);
		else
			c->AdjustGridSize(1);
		break;
	case SDLK_F1:
		if(!introText)
			introText = 8047;
		else
			introText = 0;
		break;
	case 'h':
		if(ctrl)
		{
			if(!introText)
				introText = 8047;
			else
				introText = 0;
		}
		else
			showHud = !showHud;
		break;
		
	case SDLK_LEFT:
		if (news==true)
		{
			scroll=scroll-50;
		}
		targetxadd = true;
	break;
	case SDLK_RIGHT:
		if (news==true)
		{
			scroll=scroll+50;
		}
		targetxsubtract = true;
	break;
	case SDLK_DOWN:
		if (news==true)
		{
			scrolling=scrolling+290;
		}
		targetyadd = true;
	break;
	case SDLK_UP:
		if (news==true)
		{
			scrolling=scrolling-290;
		}
		targetysubtract = true;
	break;
	case 'd':
		if(ctrl)
		{
			if(shift)
			{
				superDebug = !superDebug;
				if (superDebug==true)
				{
					showDebug = true;
				}
			}
		}
		break;
	case 'b':
		if(ctrl)
			c->SetDecoration();
		else
			if (colourPicker->GetParentWindow())
				c->SetActiveMenu(lastMenu);
			else
			{
				c->SetDecoration(true);
				c->SetPaused(true);
				c->SetActiveMenu(SC_DECO);
			}
		break;
	case 'y':
		if (ctrl)
		{
			c->HistoryForward();
		}
		else
		{
			c->SwitchAir();
		}
		break;
	case SDLK_ESCAPE:
	case 'q':
		ui::Engine::Ref().ConfirmExit();
		break;
	case 'u':
		c->ToggleAHeat();
		break;
	case 'n':
		if(shift)
		{
			news=!news;
			scroll=646;
			showHud=true;
		}
		else
			c->ToggleNewtonianGravity();
		break;
	case '=':
		if(ctrl)
			c->ResetSpark();
		else
			c->ResetAir();
		break;
	case 'c':
		if(ctrl)
		{
			selectMode = SelectCopy;
			selectPoint1 = selectPoint2 = ui::Point(-1, -1);
			isMouseDown = false;
			buttonTip = "\x0F\xEF\xEF\020Click-and-drag to specify an area to copy (right click = cancel)";
			buttonTipShow = 120;
		}
		break;
	case 'x':
		if(ctrl)
		{
			selectMode = SelectCut;
			selectPoint1 = selectPoint2 = ui::Point(-1, -1);
			isMouseDown = false;
			buttonTip = "\x0F\xEF\xEF\020Click-and-drag to specify an area to copy then cut (right click = cancel)";
			buttonTipShow = 120;
		}
		break;
	case 'v':
		if (ctrl)
		{
			if (c->LoadClipboard())
			{
				selectPoint1 = selectPoint2 = mousePosition;
				isMouseDown = false;
			}
		}
		break;
	case 'l':
	{
		std::vector<ByteString> stampList = Client::Ref().GetStamps(0, 1);
		if (stampList.size())
		{
			SaveFile *saveFile = Client::Ref().GetStamp(stampList[0]);
			c->LoadStamp(saveFile->GetGameSave());
			delete saveFile;
			selectPoint1 = selectPoint2 = mousePosition;
			isMouseDown = false;
			break;
		}
	}
	case 'k':
		selectMode = SelectNone;
		selectPoint1 = selectPoint2 = ui::Point(-1, -1);
		c->OpenStamps();
		break;
	case ']':
		if(zoomEnabled && !zoomCursorFixed)
			c->AdjustZoomSize(1, !alt);
		else
			c->AdjustBrushSize(1, !alt, shiftBehaviour, ctrlBehaviour);
		break;
	case '[':
		if(zoomEnabled && !zoomCursorFixed)
			c->AdjustZoomSize(-1, !alt);
		else
			c->AdjustBrushSize(-1, !alt, shiftBehaviour, ctrlBehaviour);
		break;
	case 'i':
		if(ctrl)
		{
			if(shift)
				invertvar=!invertvar;
			else
				c->Install();
		}
		else if(invertvar==true)
		{
			targetfire=true;
			fired=true;
		}
		else
		{
			c->InvertAirSim();
		}
		break;
	case ';':
		if (ctrl)
		{
			c->SetReplaceModeFlags(c->GetReplaceModeFlags()^SPECIFIC_DELETE);
			break;
		}
		//fancy case switch without break
	case SDLK_INSERT:
		c->SetReplaceModeFlags(c->GetReplaceModeFlags()^REPLACE_MODE);
		break;
	case SDLK_DELETE:
		c->SetReplaceModeFlags(c->GetReplaceModeFlags()^SPECIFIC_DELETE);
		break;
	case 'm':
		c->GetSimulation()->REALvar = !c->GetSimulation()->REALvar;
		break;
	case 'o':
		if (ctrl)
			pmessage=!pmessage;
		break;
	case 'a':
		localServer=!localServer;
		break;
	}

	if (shift && showDebug && key == '1')
		c->LoadRenderPreset(10);
	else if(key >= '0' && key <= '9')
	{
		c->LoadRenderPreset(key-'0');
	}
}

void GameView::OnKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if (repeat)
		return;
	switch(key)
	{
	case SDLK_LALT:
	case SDLK_RALT:
		disableAltBehaviour();
		break;
	case SDLK_LCTRL:
	case SDLK_RCTRL:
		disableCtrlBehaviour();
		break;
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		disableShiftBehaviour();
		break;
	case 'z':
		if(!zoomCursorFixed && !alt)
			c->SetZoomEnabled(false);
		break;
		
	case SDLK_LEFT:
		targetxadd = false;
	break;
	case SDLK_RIGHT:
		targetxsubtract = false;
	break;
	case SDLK_DOWN:
		targetyadd = false;
	break;
	case SDLK_UP:
		targetysubtract = false;
	break;
	case 'i':
		fired=false;
		targetfire=false;
	break;
	}
}

void GameView::OnBlur()
{
	disableAltBehaviour();
	disableCtrlBehaviour();
	disableShiftBehaviour();
	isMouseDown = false;
	drawMode = DrawPoints;
	c->MouseUp(0, 0, 0, 1); // tell lua that mouse is up (even if it really isn't)
	if (GetModifiers())
		c->KeyRelease(0, 0, false, false, false, false);
}

void GameView::OnTick(float dt)
{
	if (selectMode == PlaceSave && !placeSaveThumb)
		selectMode = SelectNone;
	if (zoomEnabled && !zoomCursorFixed)
		c->SetZoomPosition(currentMouse);

	if (skipDraw)
	{
		skipDraw = false;
	}
	else if (selectMode == SelectNone && isMouseDown)
	{
		if (drawMode == DrawPoints)
		{
			c->DrawPoints(toolIndex, lastPoint, currentPoint, true);
			lastPoint = currentPoint;
		}
		else if (drawMode == DrawFill)
		{
			c->DrawFill(toolIndex, c->PointTranslate(currentMouse));
		}
		else if (windTool && drawMode == DrawLine)
		{
			ui::Point drawPoint2 = currentMouse;
			if (altBehaviour)
				drawPoint2 = lineSnapCoords(c->PointTranslate(drawPoint1), currentMouse);
			c->DrawLine(toolIndex, c->PointTranslate(drawPoint1), drawPoint2);
		}
	}

	int foundSignID = c->GetSignAt(mousePosition.X, mousePosition.Y);
	if (foundSignID != -1)
	{
		String str = c->GetSignText(foundSignID);
		String::value_type type = '\0';
		int pos = sign::splitsign(str, &type);
		if (type == 'c' || type == 't' || type == 's')
		{
			String linkSign = str.Substr(3, pos-3);
			StringBuilder tooltip;
			switch (type)
			{
			case 'c':
				tooltip << "Go to save ID:" << linkSign;
				break;
			case 't':
				tooltip << "Open forum thread " << linkSign << " in browser";
				break;
			case 's':
				tooltip << "Search for " << linkSign;
				break;
			}
			ToolTip(ui::Point(0, Size.Y), tooltip.Build());
		}
	}

	if(introText)
	{
		introText -= int(dt)>0?((int)dt < 5? dt:5):1;
		if(introText < 0)
			introText  = 0;
	}
	if(infoTipPresence>0)
	{
		infoTipPresence -= int(dt)>0?int(dt):1;
		if(infoTipPresence<0)
			infoTipPresence = 0;
	}
	if (isButtonTipFadingIn || (selectMode != PlaceSave && selectMode != SelectNone))
	{
		isButtonTipFadingIn = false;
		if(buttonTipShow < 120)
		{
			buttonTipShow += int(dt*2)>0?int(dt*2):1;
			if(buttonTipShow>120)
				buttonTipShow = 120;
		}
	}
	else if(buttonTipShow>0)
	{
		buttonTipShow -= int(dt)>0?int(dt):1;
		if(buttonTipShow<0)
			buttonTipShow = 0;
	}
	if (isToolTipFadingIn)
	{
		isToolTipFadingIn = false;
		if(toolTipPresence < 120)
		{
			toolTipPresence += int(dt*2)>0?int(dt*2):1;
			if(toolTipPresence>120)
				toolTipPresence = 120;
		}
	}
	else if(toolTipPresence>0)
	{
		toolTipPresence -= int(dt)>0?int(dt):1;
		if(toolTipPresence<0)
			toolTipPresence = 0;
	}
	c->Update();
}


void GameView::DoMouseMove(int x, int y, int dx, int dy)
{
	if(c->MouseMove(x, y, dx, dy))
		Window::DoMouseMove(x, y, dx, dy);
}

void GameView::DoMouseDown(int x, int y, unsigned button)
{
	if(introText > 50)
		introText = 50;
	if(c->MouseDown(x, y, button))
		Window::DoMouseDown(x, y, button);
}

void GameView::DoMouseUp(int x, int y, unsigned button)
{
	if(c->MouseUp(x, y, button, 0))
		Window::DoMouseUp(x, y, button);
}

void GameView::DoMouseWheel(int x, int y, int d)
{
	if(c->MouseWheel(x, y, d))
		Window::DoMouseWheel(x, y, d);
}

void GameView::DoKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if (c->KeyPress(key, scan, repeat, shift, ctrl, alt))
		Window::DoKeyPress(key, scan, repeat, shift, ctrl, alt);
}

void GameView::DoKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt)
{
	if(c->KeyRelease(key, scan, repeat, shift, ctrl, alt))
		Window::DoKeyRelease(key, scan, repeat, shift, ctrl, alt);
}

void GameView::DoTick(float dt)
{
	//mouse events trigger every frame when mouse is held down, needs to happen here (before things are drawn) so it can clear the point queue if false is returned from a lua mouse event
	if (!c->MouseTick())
	{
		isMouseDown = false;
		selectMode = SelectNone;
		drawMode = DrawPoints;
	}
	Window::DoTick(dt);
}

void GameView::DoDraw()
{
	Window::DoDraw();
	c->Tick();
}

void GameView::NotifyNotificationsChanged(GameModel * sender)
{
	class NotificationButtonAction : public ui::ButtonAction
	{
		Notification * notification;
	public:
		NotificationButtonAction(Notification * notification) : notification(notification) { }
		void ActionCallback(ui::Button * sender)
		{
			notification->Action();
			//v->c->RemoveNotification(notification);
		}
	};
	class CloseNotificationButtonAction : public ui::ButtonAction
	{
		GameView * v;
		Notification * notification;
	public:
		CloseNotificationButtonAction(GameView * v, Notification * notification) : v(v), notification(notification) { }
		void ActionCallback(ui::Button * sender)
		{
			v->c->RemoveNotification(notification);
		}
		void AltActionCallback(ui::Button * sender)
		{
			v->c->RemoveNotification(notification);
		}
	};

	for(std::vector<ui::Component*>::const_iterator iter = notificationComponents.begin(), end = notificationComponents.end(); iter != end; ++iter) {
		ui::Component * cNotification = *iter;
		RemoveComponent(cNotification);
		delete cNotification;
	}
	notificationComponents.clear();

	std::vector<Notification*> notifications = sender->GetNotifications();

	int currentY = YRES-23;
	for(std::vector<Notification*>::iterator iter = notifications.begin(), end = notifications.end(); iter != end; ++iter)
	{
		int width = (Graphics::textwidth((*iter)->Message))+8;
		ui::Button * tempButton = new ui::Button(ui::Point(XRES-width-22, currentY), ui::Point(width, 15), (*iter)->Message);
		tempButton->SetActionCallback(new NotificationButtonAction(*iter));
		tempButton->Appearance.BorderInactive = style::Colour::WarningTitle;
		tempButton->Appearance.TextInactive = style::Colour::WarningTitle;
		tempButton->Appearance.BorderHover = ui::Colour(255, 175, 0);
		tempButton->Appearance.TextHover = ui::Colour(255, 175, 0);
		AddComponent(tempButton);
		notificationComponents.push_back(tempButton);

		tempButton = new ui::Button(ui::Point(XRES-20, currentY), ui::Point(15, 15), 0xE02A);
		//tempButton->SetIcon(IconClose);
		tempButton->SetActionCallback(new CloseNotificationButtonAction(this, *iter));
		tempButton->Appearance.Margin.Left -= 1;
		tempButton->Appearance.Margin.Top -= 1;
		tempButton->Appearance.BorderInactive = style::Colour::WarningTitle;
		tempButton->Appearance.TextInactive = style::Colour::WarningTitle;
		tempButton->Appearance.BorderHover = ui::Colour(255, 175, 0);
		tempButton->Appearance.TextHover = ui::Colour(255, 175, 0);
		AddComponent(tempButton);
		notificationComponents.push_back(tempButton);

		currentY -= 17;
	}
}

void GameView::NotifyZoomChanged(GameModel * sender)
{
	zoomEnabled = sender->GetZoomEnabled();
}

void GameView::NotifyLogChanged(GameModel * sender, String entry)
{
	logEntries.push_front(std::pair<String, int>(entry, 600));
	if (logEntries.size() > 20)
		logEntries.pop_back();
}

void GameView::NotifyPlaceSaveChanged(GameModel * sender)
{
	delete placeSaveThumb;
	placeSaveOffset = ui::Point(0, 0);
	if(sender->GetPlaceSave())
	{
		placeSaveThumb = SaveRenderer::Ref().Render(sender->GetPlaceSave());
		selectMode = PlaceSave;
		selectPoint2 = mousePosition;
	}
	else
	{
		placeSaveThumb = NULL;
		selectMode = SelectNone;
	}
}

void GameView::enableShiftBehaviour()
{
	if (!shiftBehaviour)
	{
		shiftBehaviour = true;
		if (!isMouseDown || selectMode != SelectNone)
			UpdateDrawMode();
		UpdateToolStrength();
	}
}

void GameView::disableShiftBehaviour()
{
	if (shiftBehaviour)
	{
		shiftBehaviour = false;
		if (!isMouseDown || selectMode != SelectNone)
			UpdateDrawMode();
		UpdateToolStrength();
	}
}

void GameView::enableAltBehaviour()
{
	if (!altBehaviour)
	{
		altBehaviour = true;
		drawSnap = true;
	}
}

void GameView::disableAltBehaviour()
{
	if (altBehaviour)
	{
		altBehaviour = false;
		drawSnap = false;
	}
}

void GameView::enableCtrlBehaviour()
{
	if (!ctrlBehaviour)
	{
		ctrlBehaviour = true;
		if (!isMouseDown || selectMode != SelectNone)
			UpdateDrawMode();
		UpdateToolStrength();

		//Show HDD save & load buttons
		saveSimulationButton->Appearance.BackgroundInactive = saveSimulationButton->Appearance.BackgroundHover = ui::Colour(255, 255, 255);
		saveSimulationButton->Appearance.TextInactive = saveSimulationButton->Appearance.TextHover = ui::Colour(0, 0, 0);

		saveSimulationButton->Enabled = true;
		SetSaveButtonTooltips();

		searchButton->Appearance.BackgroundInactive = searchButton->Appearance.BackgroundHover = ui::Colour(255, 255, 255);
		searchButton->Appearance.TextInactive = searchButton->Appearance.TextHover = ui::Colour(0, 0, 0);

		searchButton->SetToolTip("Open a simulation from your hard drive.");
		if (currentSaveType == 2)
			((SplitButton*)saveSimulationButton)->SetShowSplit(true);
	}
}

void GameView::disableCtrlBehaviour()
{
	if (ctrlBehaviour)
	{
		ctrlBehaviour = false;
		if (!isMouseDown || selectMode != SelectNone)
			UpdateDrawMode();
		UpdateToolStrength();

		//Hide HDD save & load buttons
		saveSimulationButton->Appearance.BackgroundInactive = ui::Colour(0, 0, 0);
		saveSimulationButton->Appearance.BackgroundHover = ui::Colour(20, 20, 20);
		saveSimulationButton->Appearance.TextInactive = saveSimulationButton->Appearance.TextHover = ui::Colour(255, 255, 255);
		saveSimulationButton->Enabled = saveSimulationButtonEnabled && saveReuploadAllowed;
		SetSaveButtonTooltips();
		searchButton->Appearance.BackgroundInactive = ui::Colour(0, 0, 0);
		searchButton->Appearance.BackgroundHover = ui::Colour(20, 20, 20);
		searchButton->Appearance.TextInactive = searchButton->Appearance.TextHover = ui::Colour(255, 255, 255);
		searchButton->SetToolTip("Find & open a simulation. Hold Ctrl to load offline saves.");
		if (currentSaveType == 2)
			((SplitButton*)saveSimulationButton)->SetShowSplit(false);
	}
}

void GameView::UpdateDrawMode()
{
	if (ctrlBehaviour && shiftBehaviour)
	{
		if (toolBrush)
			drawMode = DrawPoints;
		else
			drawMode = DrawFill;
	}
	else if (ctrlBehaviour)
		drawMode = DrawRect;
	else if (shiftBehaviour)
		drawMode = DrawLine;
	else
		drawMode = DrawPoints;
}

void GameView::UpdateToolStrength()
{
	if (shiftBehaviour)
		c->SetToolStrength(10.0f);
	else if (ctrlBehaviour)
		c->SetToolStrength(.1f);
	else
		c->SetToolStrength(1.0f);
}

void GameView::SetSaveButtonTooltips()
{
	if (!Client::Ref().GetAuthUser().UserID)
		((SplitButton*)saveSimulationButton)->SetToolTips("Overwrite the open simulation on your hard drive.", "Save the simulation to your hard drive. Login to save online.");
	else if (ctrlBehaviour)
		((SplitButton*)saveSimulationButton)->SetToolTips("Overwrite the open simulation on your hard drive.", "Save the simulation to your hard drive.");
	else if (((SplitButton*)saveSimulationButton)->GetShowSplit())
		((SplitButton*)saveSimulationButton)->SetToolTips("Re-upload the current simulation", "Modify simulation properties");
	else
		((SplitButton*)saveSimulationButton)->SetToolTips("Re-upload the current simulation", "Upload a new simulation. Hold Ctrl to save offline.");
}

void GameView::OnDraw()
{
	Graphics * g = GetGraphics();
	if (ren)
	{
		ren->clearScreen(1.0f);
		ren->RenderBegin();
		ren->SetSample(c->PointTranslate(currentMouse).X, c->PointTranslate(currentMouse).Y);
		if (selectMode == SelectNone && (!zoomEnabled || zoomCursorFixed) && activeBrush && (isMouseDown || (currentMouse.X >= 0 && currentMouse.X < XRES && currentMouse.Y >= 0 && currentMouse.Y < YRES)))
		{
			ui::Point finalCurrentMouse = c->PointTranslate(currentMouse);
			ui::Point initialDrawPoint = drawPoint1;

			if (wallBrush)
			{
				finalCurrentMouse = c->NormaliseBlockCoord(finalCurrentMouse);
				initialDrawPoint = c->NormaliseBlockCoord(initialDrawPoint);
			}

			if (drawMode == DrawRect && isMouseDown)
			{
				if (drawSnap)
				{
					finalCurrentMouse = rectSnapCoords(c->PointTranslate(initialDrawPoint), finalCurrentMouse);
				}
				if (wallBrush)
				{
					if (finalCurrentMouse.X > initialDrawPoint.X)
						finalCurrentMouse.X += CELL-1;
					else
						initialDrawPoint.X += CELL-1;

					if (finalCurrentMouse.Y > initialDrawPoint.Y)
						finalCurrentMouse.Y += CELL-1;
					else
						initialDrawPoint.Y += CELL-1;
				}
				activeBrush->RenderRect(ren, c->PointTranslate(initialDrawPoint), finalCurrentMouse);
			}
			else if (drawMode == DrawLine && isMouseDown)
			{
				if (drawSnap)
				{
					finalCurrentMouse = lineSnapCoords(c->PointTranslate(initialDrawPoint), finalCurrentMouse);
				}
				activeBrush->RenderLine(ren, c->PointTranslate(initialDrawPoint), finalCurrentMouse);
			}
			else if (drawMode == DrawFill)// || altBehaviour)
			{
				if (!decoBrush)
					activeBrush->RenderFill(ren, finalCurrentMouse);
			}
			if (drawMode == DrawPoints || drawMode==DrawLine || (drawMode == DrawRect && !isMouseDown))
			{
				if (wallBrush)
				{
					ui::Point finalBrushRadius = c->NormaliseBlockCoord(activeBrush->GetRadius());
					ren->xor_line(finalCurrentMouse.X-finalBrushRadius.X, finalCurrentMouse.Y-finalBrushRadius.Y, finalCurrentMouse.X+finalBrushRadius.X+CELL-1, finalCurrentMouse.Y-finalBrushRadius.Y);
					ren->xor_line(finalCurrentMouse.X-finalBrushRadius.X, finalCurrentMouse.Y+finalBrushRadius.Y+CELL-1, finalCurrentMouse.X+finalBrushRadius.X+CELL-1, finalCurrentMouse.Y+finalBrushRadius.Y+CELL-1);

					ren->xor_line(finalCurrentMouse.X-finalBrushRadius.X, finalCurrentMouse.Y-finalBrushRadius.Y+1, finalCurrentMouse.X-finalBrushRadius.X, finalCurrentMouse.Y+finalBrushRadius.Y+CELL-2);
					ren->xor_line(finalCurrentMouse.X+finalBrushRadius.X+CELL-1, finalCurrentMouse.Y-finalBrushRadius.Y+1, finalCurrentMouse.X+finalBrushRadius.X+CELL-1, finalCurrentMouse.Y+finalBrushRadius.Y+CELL-2);
				}
				else
				{
					activeBrush->RenderPoint(ren, finalCurrentMouse);
				}
			}
		}

		if (pmessage==true)
		{
			news=false;
			showHud=false;

			g->fillrect(0, 0, WINDOWW, WINDOWH, 50, 50, 50, 255);

			g->drawtext(10, 10, "Hey Orange. I miss you. I can't beleive that you could leave\nlike that... I gave you everything, opened myself to you. I decided to\nbe vulnerable, but you only took me down to my knees and kicked me. Ich sitz \nhier, allein und traurig. Du bist verlassen, und ich bin in dem Dunkelheit. \nWas soll ich machen meine Liebe? Dein Mutter hasst mich, ich habe keine Wissen \nuber warum. Wie bist du, Helligkeit? Die Sonne tut weh, du bist weg. Ich bin \nallein, ohne alles. Ich Hoffe dass du wirst alles sehen.\n\nDein verlassen Liebe", 200, 200, 200, 255); 
		}

		if(selectMode!=SelectNone)
		{
			if(selectMode==PlaceSave)
			{
				if(placeSaveThumb && selectPoint2.X!=-1)
				{
					int thumbX = selectPoint2.X - ((placeSaveThumb->Width-placeSaveOffset.X)/2) + CELL/2;
					int thumbY = selectPoint2.Y - ((placeSaveThumb->Height-placeSaveOffset.Y)/2) + CELL/2;

					ui::Point thumbPos = c->NormaliseBlockCoord(ui::Point(thumbX, thumbY));

					if(thumbPos.X<0)
						thumbPos.X = 0;
					if(thumbPos.X+(placeSaveThumb->Width)>=XRES)
						thumbPos.X = XRES-placeSaveThumb->Width;

					if(thumbPos.Y<0)
						thumbPos.Y = 0;
					if(thumbPos.Y+(placeSaveThumb->Height)>=YRES)
						thumbPos.Y = YRES-placeSaveThumb->Height;

					ren->draw_image(placeSaveThumb, thumbPos.X, thumbPos.Y, 128);

					ren->xor_rect(thumbPos.X, thumbPos.Y, placeSaveThumb->Width, placeSaveThumb->Height);
				}
			}
			else
			{
				if(selectPoint1.X==-1)
				{
					ren->fillrect(0, 0, XRES, YRES, 0, 0, 0, 100);
				}
				else
				{
					int x2 = (selectPoint1.X>selectPoint2.X)?selectPoint1.X:selectPoint2.X;
					int y2 = (selectPoint1.Y>selectPoint2.Y)?selectPoint1.Y:selectPoint2.Y;
					int x1 = (selectPoint2.X<selectPoint1.X)?selectPoint2.X:selectPoint1.X;
					int y1 = (selectPoint2.Y<selectPoint1.Y)?selectPoint2.Y:selectPoint1.Y;

					if(x2>XRES-1)
						x2 = XRES-1;
					if(y2>YRES-1)
						y2 = YRES-1;

					ren->fillrect(0, 0, XRES, y1, 0, 0, 0, 100);
					ren->fillrect(0, y2+1, XRES, YRES-y2-1, 0, 0, 0, 100);

					ren->fillrect(0, y1, x1, (y2-y1)+1, 0, 0, 0, 100);
					ren->fillrect(x2+1, y1, XRES-x2-1, (y2-y1)+1, 0, 0, 0, 100);

					ren->xor_rect(x1, y1, (x2-x1)+1, (y2-y1)+1);
				}
			}
		}

		ren->RenderEnd();

		if(doScreenshot)
		{
			VideoBuffer screenshot(ren->DumpFrame());
			std::vector<char> data = format::VideoBufferToPNG(screenshot);

			ByteString filename = ByteString::Build("screenshot_", Format::Width(screenshotIndex++, 6), ".png");

			Client::Ref().WriteFile(data, filename);
			doScreenshot = false;
		}

		if(recording)
		{
			VideoBuffer screenshot(ren->DumpFrame());
			std::vector<char> data = format::VideoBufferToPPM(screenshot);

			ByteString filename = ByteString::Build("recordings", PATH_SEP, recordingFolder, PATH_SEP, "frame_", Format::Width(screenshotIndex++, 6), ".ppm");

			Client::Ref().WriteFile(data, filename);
		}

		if (logEntries.size())
		{
			int startX = 20;
			int startY = YRES-20;
			deque<std::pair<String, int> >::iterator iter;
			for(iter = logEntries.begin(); iter != logEntries.end(); iter++)
			{
				String message = (*iter).first;
				int alpha = std::min((*iter).second, 255);
				if (alpha <= 0) //erase this and everything older
				{
					logEntries.erase(iter, logEntries.end());
					break;
				}
				startY -= 14;
				g->fillrect(startX-3, startY-3, Graphics::textwidth(message)+6, 14, 0, 0, 0, 100);
				g->drawtext(startX, startY, message, 255, 255, 255, alpha);
				(*iter).second -= 3;
			}
		}
	}

	if(recording)
	{
		String sampleInfo = String::Build(recordingIndex, ". ", String(0xE00E), " REC");

		int textWidth = Graphics::textwidth(sampleInfo);
		g->fillrect(XRES-20-textWidth, 12, textWidth+8, 15, 0, 0, 0, 255*0.5);
		g->drawtext(XRES-16-textWidth, 16, sampleInfo, 255, 50, 20, 255);
	}
	else if(news==true)
	{
		if (newsPause==true)
			scroll++;

		showHud=false;

		if (scrolling>4332)
			scrolling=4332;
		if (scrolling<0)
			scrolling=0;

		//391 lines available for info. Take line number, and multiply by 12 then add 25 for location

		scroll--;
		if(scroll<=-1846)
			scroll=646;

		g->fillrect(0, 0, WINDOWW, WINDOWH, 50, 50, 50, 255);

		g->drawtext(240, 25-scrolling, "NEWS, HELP, INFO, AND REFERENCE GUIDE", 255, 255, 255, 255);
		g->draw_line(15, 37-scrolling, 612, 37-scrolling, 255, 255, 0, 255);

		g->drawtext(15, 49-scrolling, "LIST OF MOD KEY COMMANDS:\n\n'ctrl+h' opens the entrance screen\n'ctrl+shift+n' turns on Super Debug Mode\n'shift+n' opens this message\n'ctrl+p' opens a parts graph\n'ctrl+m' turns on 'Realistic Mode'\nThere are various others which are not yet listed\n\n"/*Elements Help. Replace with help menu at some point.*/"EXTRA ELEMENT INFORMATION:\n\n"/*NBOT lists*/"NBOT: Uses several modes, set by TMP. Once activated by PSCN and charged, they do what they are set to.\n 0: Retrieve Particle\n 1: Explode\n 2: Charge\n 3: Fight\n 4: Break\n 5: Replicate\n 6: Beacon\n\n"/*Filter Section*/"FILT: This table shows basic ctype values for electronics.\n 1\n 2\n 4\n 8\n 16\n 32\n 64\n 128\n 256\n 512\n 1024\n 2048\n 4096\n 8192\n 16384\n 32768\n 65536\n 131072\n 262144\n 524288\n 1048576\n 2097152\n 4194304\n 8388608\n 16777216\n 33554432\n 67108864\n 134217728\n 268435456\n 536870912", 200, 200, 200, 255); 

		g->drawtext(15, 4692-scrolling, "This is the last line.", 255, 0, 0, 255);

		g->fillrect(0, 0, WINDOWW, 22, 50, 50, 50, 255);

		//g->drawtext(10, 370, "'ctrl+o' for... Myself.", 0, 0, 255, 255);

		g->draw_line(0, 7, WINDOWW, 7, 0, 255, 255, 255);
		g->draw_line(0, 20, WINDOWW, 20, 0, 255, 255, 255);

		g->drawtext(scroll, 10, "Press 'space' to pause news, left and right arrow keys to move in blocks, and up and down to block scroll. NEWS: News feed added! Bottom right number shows location of news feed. COMING SOON: RCServer! Will have auto news feeds, updates, and other cool stuff! New elements have been added, as well as lots of key combos. Adding help menu soon, for now it will just be here. ", 0, 255, 0, 255);
		std::stringstream scrollnum;
		scrollnum << scroll;
		g->drawtext(560, 370, scrollnum.str(), 0, 255, 0, 200);

		scrollbar = 24+(scrolling/12);

		g->draw_line(6, scrollbar, 6, scrollbar+12, 255, 255, 255, 255);
		
	}
	else if(showHud && introText < 51)
	{


int hudx = 612; //Set as the max X value for the screen (Ususally 612)

RMx = 80;
RMy = 29;
RMBx = 77;
RMBy = 27;

//Changes Servers     (Size.X-177, Size.Y-16)
if((serverButton->GetToggleState()==false)&&(localServer==false)){
	SERVER = "powdertoy.co.uk";
	STATICSERVER = "static.powdertoy.co.uk";
	}
if((serverButton->GetToggleState()==true)&&(localServer==false)){
	SERVER = "FortCollinsB17.org:3000";
	STATICSERVER = "FortCollinsB17:3000";
	}
if(localServer==true)
{
SERVER = "192.168.0.64:3000";
	STATICSERVER = "192.168.0.64:3000";
	//SERVER = "localhost:3000";
	//STATICSERVER = "localhost:3000";
	g->drawtext(10, 100, "LocalHost Enabled 192.168.0.64", 255, 255, 255, 255);
}

if(FPSGvar==true)
{
count = count + 1;
std::stringstream counting;
if(count>=ui::Engine::Ref().GetFps())
{
count = 0;
FPS5 = FPS4;
FPS4 = FPS3;
FPS3 = FPS2;
FPS2 = FPS1;
FPS1 = (floor(ui::Engine::Ref().GetFps()*100))/100;
}
g->fillrect(517, 298, 95, 86, 55, 55, 55, 200);
//Draws axis
g->draw_line(528, 371, 528, 301, 0, 255, 255, 255);
g->draw_line(528, 371, 608, 371, 0, 255, 255, 255);
//Draw Ticks
g->addpixel(548, 370, 0, 255, 255, 255);
g->addpixel(568, 370, 0, 255, 255, 255);
g->addpixel(588, 370, 0, 255, 255, 255);
g->addpixel(608, 370, 0, 255, 255, 255);

g->addpixel(529, 301, 0, 255, 255, 255);
g->addpixel(529, 311, 0, 255, 255, 255);
g->addpixel(529, 321, 0, 255, 255, 255);
g->addpixel(529, 331, 0, 255, 255, 255);
g->addpixel(529, 341, 0, 255, 255, 255);
g->addpixel(529, 351, 0, 255, 255, 255);
g->addpixel(529, 361, 0, 255, 255, 255);

g->drawtext(553, 374, "Seconds", 0, 255, 0, 255);

g->drawtext(520, 321, "F", 0, 255, 0, 255);
g->drawtext(520, 331, "P", 0, 255, 0, 255);
g->drawtext(520, 341, "S", 0, 255, 0, 255);

//Draws lines on graph
g->draw_line(528, 371-FPS1, 548, 371-FPS2, 0, 255, 255, 255);
g->draw_line(548, 371-FPS2, 568, 371-FPS3, 0, 255, 255, 255);
g->draw_line(568, 371-FPS3, 588, 371-FPS4, 0, 255, 255, 255);
g->draw_line(588, 371-FPS4, 608, 371-FPS5, 0, 255, 255, 255);
}

if (invertvar==true)
{
	if (accelcount<=10)
		accel = 1;
	if (accelcount>=11 && accelcount<=20)
		accel = 2;
	if (accelcount>=21 && accelcount<=30)
		accel = 3;
	if (accelcount>=31 && accelcount<=40)
		accel = 4;
	if (accelcount>=41)
		accel = 5;
	
	if (acceltimer>=2)
		accelcount = 0;
	
	if (targetxadd==true)
		{
			targetx = targetx-accel;
			accelcount++;
			acceltimer = 0;
		}
	if (targetxsubtract==true)
		{
			targetx = targetx+accel;
			accelcount++;
			acceltimer = 0;
		}
	if (targetyadd==true)
		{
			targety = targety+accel;
			accelcount++;
			acceltimer = 0;
		}
	if (targetysubtract==true)
		{
			targety = targety-accel;
			accelcount++;
			acceltimer = 0;
		}
	acceltimer++;
		
	if (targetx>=612)
		targetx=612;
	if (targetx<=0)
		targetx=0;
	if (targety>=384)
		targety=384;
	if (targety<=0)
		targety=0;
	g->draw_line(targetx+3, targety, targetx+1, targety, 255, 0, 0, 150);
	g->draw_line(targetx-3, targety, targetx-1, targety, 255, 0, 0, 150);
	g->draw_line(targetx, targety-3, targetx, targety-1, 255, 0, 0, 150);
	g->draw_line(targetx, targety+3, targetx, targety+1, 255, 0, 0, 150);
	g->addpixel(targetx, targety+1, 255, 0, 0, 255);
	g->addpixel(targetx, targety-1, 255, 0, 0, 255);
	g->addpixel(targetx+1, targety, 255, 0, 0, 255);
	g->addpixel(targetx-1, targety, 255, 0, 0, 255);
	g->addpixel(targetx, targety+5, 255, 0, 0, 255);
	g->addpixel(targetx, targety-5, 255, 0, 0, 255);
	g->addpixel(targetx+5, targety, 255, 0, 0, 255);
	g->addpixel(targetx-5, targety, 255, 0, 0, 255);
}
frameCount = frameCount + 1;

if (fired==true)
{
	g->draw_line(targetx+5, targety+5, targetx-5, targety+5, 0, 255, 0, 150);
	g->draw_line(targetx-5, targety-5, targetx-5, targety+5, 0, 255, 0, 150);
	g->draw_line(targetx-5, targety-5, targetx+5, targety-5, 0, 255, 0, 150);
	g->draw_line(targetx+5, targety-5, targetx+5, targety+5, 0, 255, 0, 150);	
}

if(INFOvar==true)
{
	if(FPSGvar==true)
	{
		infoX = hudx-158;
		infoY = 298;
	}
	else
	{
		infoX = hudx-60;
		infoY = 298;
	}
g->fillrect(infoX, infoY, 60, 86, 55, 55, 55, 200);

std::stringstream frames;
frames << frameCount;
g->drawtext(infoX+3, infoY+23, "Frames:", 0, 255, 0, 255);
g->drawtext(infoX+3, infoY+33, frames.str(), 0, 255, 255, 255);
g->drawtext(infoX+3, infoY+3, "Element #:", 0, 255, 0, 255);
	if (sample.particle.type)
	{
		std::stringstream infoptype;
		infoptype << sample.particle.type;
		g->drawtext(infoX+3, infoY+13, infoptype.str(), 0, 255, 255, 255);
	}
	else
	{
		g->drawtext(infoX+3, infoY+13, "No Element", 0, 255, 255, 255);
	}
g->drawtext(infoX+3, infoY+43, "Tmp3:", 0, 255, 0, 255);
g->drawtext(infoX+3, infoY+63, "Tmp4:", 0, 255, 0, 255);
std::stringstream tmp3;
tmp3 << sample.particle.tmp3;
std::stringstream tmp4;
tmp4 << sample.particle.tmp4;
	if (sample.particle.type)
	{
		g->drawtext(infoX+3, infoY+53, tmp3.str(), 0, 255, 255, 255);
		g->drawtext(infoX+3, infoY+73, tmp4.str(), 0, 255, 255, 255);
	}
	else
	{
		g->drawtext(infoX+3, infoY+53, "N E", 0, 255, 255, 255);
		g->drawtext(infoX+3, infoY+73, "N E", 0, 255, 255, 255);
	}
}
		//HUD

if (zoomEnabled==true)
{
	superMove = 295;
	zoomMove = 246;
	if (zoomCursorFixed)
	{
		
	}
	else if (sample.PositionX>305)
	{
		xMove = 384;
		superX = -217;
		xx = 412;
	}
	else
	{
		xMove = 0;
		superX = 167;
		xx = 0;
	}
}
else
{
	xMove = 0;
	zoomMove = 0;
	superMove = 0;
	superX = 0;
}

if (elCount==true)
{
	if (sample.PositionX>=306 && zoomEnabled==false)
	{
		xx = 0;
	}
	else if (zoomEnabled==false)
	{
		xx = 412;
	}
	countt++;
	pixAmount = sample.NumParts;
	if (countt>=7)
	{
		countt = 0;
		pa20 = pa19;
		pa19 = pa18;
		pa18 = pa17;
		pa17 = pa16;
		pa16 = pa15;
		pa15 = pa14;
		pa14 = pa13;
		pa13 = pa12;
		pa12 = pa11;
		pa11 = pa10;
		pa10 = pa9;
		pa9 = pa8;
		pa8 = pa7;
		pa7 = pa6;
		pa6 = pa5;
		pa5 = pa4;
		pa4 = pa3;
		pa3 = pa2;
		pa2 = pa1;
		
		factor = 0;
		
		while (370-pixAmount<=0)
		{
			pixAmount = (pixAmount/2)-185;
			factor = factor+1;
		}
		pa1 = 370-pixAmount;
	}
	g->draw_line(10+xx, pa1, 20+xx, pa2, 0, 255, 255, 255);
	g->draw_line(20+xx, pa2, 30+xx, pa3, 0, 255, 255, 255);
	g->draw_line(30+xx, pa3, 40+xx, pa4, 0, 255, 255, 255);
	g->draw_line(40+xx, pa4, 50+xx, pa5, 0, 255, 255, 255);
	g->draw_line(50+xx, pa5, 60+xx, pa6, 0, 255, 255, 255);
	g->draw_line(60+xx, pa6, 70+xx, pa7, 0, 255, 255, 255);
	g->draw_line(70+xx, pa7, 80+xx, pa8, 0, 255, 255, 255);
	g->draw_line(80+xx, pa8, 90+xx, pa9, 0, 255, 255, 255);
	g->draw_line(90+xx, pa9, 100+xx, pa10, 0, 255, 255, 255);
	g->draw_line(100+xx, pa10, 110+xx, pa11, 0, 255, 255, 255);
	g->draw_line(110+xx, pa11, 120+xx, pa12, 0, 255, 255, 255);
	g->draw_line(120+xx, pa12, 130+xx, pa13, 0, 255, 255, 255);
	g->draw_line(130+xx, pa13, 140+xx, pa14, 0, 255, 255, 255);
	g->draw_line(140+xx, pa14, 150+xx, pa15, 0, 255, 255, 255);
	g->draw_line(150+xx, pa15, 160+xx, pa16, 0, 255, 255, 255);
	g->draw_line(160+xx, pa16, 170+xx, pa17, 0, 255, 255, 255);
	g->draw_line(170+xx, pa17, 180+xx, pa18, 0, 255, 255, 255);
	g->draw_line(180+xx, pa18, 190+xx, pa19, 0, 255, 255, 255);
	g->draw_line(190+xx, pa19, 200+xx, pa20, 0, 255, 255, 255);
	g->draw_line(10+xx, 370, 200+xx, 370, 0, 255, 0, 255);

	g->draw_line(10+xx, 0, 10+xx, 369, 255, 0, 0, 100);
	g->draw_line(20+xx, 0, 20+xx, 369, 255, 0, 0, 100);
	g->draw_line(30+xx, 0, 30+xx, 369, 255, 0, 0, 100);
	g->draw_line(40+xx, 0, 40+xx, 369, 255, 0, 0, 100);
	g->draw_line(50+xx, 0, 50+xx, 369, 255, 0, 0, 100);
	g->draw_line(60+xx, 0, 60+xx, 369, 255, 0, 0, 100);
	g->draw_line(70+xx, 0, 70+xx, 369, 255, 0, 0, 100);
	g->draw_line(80+xx, 0, 80+xx, 369, 255, 0, 0, 100);
	g->draw_line(90+xx, 0, 90+xx, 369, 255, 0, 0, 100);
	g->draw_line(100+xx, 0, 100+xx, 369, 255, 0, 0, 100);
	g->draw_line(110+xx, 0, 110+xx, 369, 255, 0, 0, 100);
	g->draw_line(120+xx, 0, 120+xx, 369, 255, 0, 0, 100);
	g->draw_line(130+xx, 0, 130+xx, 369, 255, 0, 0, 100);
	g->draw_line(140+xx, 0, 140+xx, 369, 255, 0, 0, 100);
	g->draw_line(150+xx, 0, 150+xx, 369, 255, 0, 0, 100);
	g->draw_line(160+xx, 0, 160+xx, 369, 255, 0, 0, 100);
	g->draw_line(170+xx, 0, 170+xx, 369, 255, 0, 0, 100);
	g->draw_line(180+xx, 0, 180+xx, 369, 255, 0, 0, 100);
	g->draw_line(190+xx, 0, 190+xx, 369, 255, 0, 0, 100);
	g->draw_line(200+xx, 0, 200+xx, 369, 255, 0, 0, 100);

	std::stringstream factors;
	factors << "Factor: 2^" << factor;
	g->drawtext(12+xx, 373, factors.str(), 255, 0, 0, 255);
}

if (superDebug==true)
{
	showDebug = true;
	g->fillrect(superX+231, superMove+13, 160, 12, 55, 55, 55, 200);
	g->fillrect(superX+231, superMove+27, 160, 12, 55, 55, 55, 200);
	g->fillrect(superX+231, superMove+41, 160, 12, 55, 55, 55, 200);
	
	std::stringstream Grav;
	Grav << "Grav:" << (floor(sample.Gravity*10000))/10000;
	g->drawtext(superX+233, superMove+15, Grav.str(), 0, 255, 255, 255);
	
	std::stringstream Ambient;
	Ambient << "Ambt:" << sample.AirTemperature-273.15;
	g->drawtext(superX+300, superMove+15, Ambient.str(), 0, 255, 255, 255);
	
	std::stringstream GravX;
	GravX << "GravX:" << (floor(sample.GravityVelocityX*1000))/1000;
	g->drawtext(superX+233, superMove+30, GravX.str(), 0, 255, 255, 255);
	
	std::stringstream GravY;
	GravY << "GravY:" << (floor(sample.GravityVelocityY*1000))/1000;
	g->drawtext(superX+300, superMove+30, GravY.str(), 0, 255, 255, 255);
	
	std::stringstream AirX;
	AirX << "AirX:" << (floor(sample.AirVelocityX*1000))/1000;
	g->drawtext(superX+233, superMove+45, AirX.str(), 0, 255, 255, 255);
	
	std::stringstream AirY;
	AirY << "AirY:" << (floor(sample.AirVelocityY*1000))/1000;
	g->drawtext(superX+300, superMove+45, AirY.str(), 0, 255, 255, 255);
}

//Time
time_t rawtime;
struct tm * timeinfo;

time ( &rawtime );
timeinfo = localtime ( &rawtime );

//Behind HUD, so that values are easily visible when particles are behind it.
g->fillrect(hudx-214-xMove, zoomMove+13, 187, 12, 55, 55, 55, 200);
g->fillrect(hudx-214-xMove, zoomMove+27, 200, 12, 55, 55, 55, 200);
g->fillrect(14+xMove, 13, 210, 12, 55, 55, 55, 200);

g->drawtext(16+xMove, 15, asctime(timeinfo), 0, 255, 68, 255);

std::stringstream parts;
parts << "Parts:" << sample.NumParts;
g->drawtext(160+xMove, 15, parts.str(), 0, 255, 255, 255);


/*g->draw_line(196, 200, 204, 200, 255, 0, 0, 255);
g->draw_line(200, 196, 200, 204, 255, 0, 0, 255);
g->draw_line(200, 196, 204, 196, 255, 0, 0, 255);
g->draw_line(204, 200, 204, 204, 255, 0, 0, 255);
g->draw_line(200, 204, 196, 204, 255, 0, 0, 255);
g->draw_line(196, 200, 196, 196, 255, 0, 0, 255);


g->draw_line(anarx-4, anary, anarx+4, anary, 255, 0, 0, 255);
g->draw_line(anarx, anary-4, anarx, anary+4, 255, 0, 0, 255);
g->draw_line(anarx, anary-4, anarx+4, anary-4, 255, 0, 0, 255);
g->draw_line(anarx+4, anary, anarx+4, anary+4, 255, 0, 0, 255);
g->draw_line(anarx, anary+4, anarx-4, anary+4, 255, 0, 0, 255);
g->draw_line(anarx-4, anary, anarx-4, anary-4, 255, 0, 0, 255);*/


/*std::stringstream pavg;
pavg << sample.particle.pavg[0];
g->drawtext(300, 15, pavg.str(), 255, 0, 255, 255);*/

std::stringstream pres;
pres << "Pres:" << (floor (sample.AirPressure*100))/100;
g->drawtext(hudx-77-xMove, zoomMove+15, pres.str(), 0, 255, 255, 255);

std::stringstream life;
life << "Life:" << sample.particle.life;
g->drawtext(hudx-212-xMove, zoomMove+30, life.str(), 0, 255, 255, 255);

std::stringstream x;
x << "X:" << sample.PositionX;
g->drawtext(hudx-71-xMove, zoomMove+30, x.str(), 0, 255, 255, 255);

std::stringstream y;
y << "Y:" << sample.PositionY;
g->drawtext(hudx-42-xMove, zoomMove+30, y.str(), 0, 255, 255, 255);

if (showDebug)
{
	g->fillrect(hudx-214-xMove, zoomMove+41, 180, 12, 55, 55, 55, 200);
	g->fillrect(14+xMove, 27, 61, 12, 55, 55, 55, 200);

	std::stringstream extraInfo;
	if (c->GetReplaceModeFlags()&REPLACE_MODE)
		extraInfo << "[REPLACE MODE] ";
		
	if (c->GetReplaceModeFlags()&SPECIFIC_DELETE)
		extraInfo << "[SPECIFIC DELETE] ";
		
	if (ren->GetGridSize())
		extraInfo << "[GRID: " << ren->GetGridSize() << "/9]";
		
	//if (LinkVar==true)
	//	extraInfo << "[LINK MODE]";
		
	if (ren->findingElement)
		extraInfo << " [FIND]";
		
	g->drawtext(16+xMove, 43, extraInfo.str(), 0, 255, 255, 255);

	std::stringstream fps;
	fps << "FPS:" << (floor(ui::Engine::Ref().GetFps()*100))/100;
	g->drawtext(16+xMove, 29, fps.str(), 0, 255, 255 ,255);

	std::stringstream tmp2;
	if (sample.particle.type==PT_NBOT)
	{
		if (sample.particle.tmp2==0)
			tmp2 << "Bot Inactive";
			
		if (sample.particle.tmp2==1)
			tmp2 << "Bot Active";
			
		if (sample.particle.tmp2==2)
			tmp2 << "Bot Active";
			
		if (sample.particle.tmp2==3)
			tmp2 << "Bot Active";
			
		if (sample.particle.tmp2==4)
			tmp2 << "Needs Charge";
	}
	else
	{
	tmp2 << "Tmp2:" << sample.particle.tmp2;
	}
	
	g->drawtext(hudx-212-xMove, zoomMove+44, tmp2.str(), 0, 255, 255, 255);
	
	if (sample.particle.type==PT_FILT)
	{
		std::stringstream filtVal;
		filtVal << "Value:" << sample.particle.ctype;
		g->drawtext(hudx-147-xMove, zoomMove+44, filtVal.str(), 0, 255, 255, 255);
	}
	else
	{
	std::stringstream vx;
	vx << "VX:" << (floor(sample.particle.vx*100))/100;
	g->drawtext(hudx-147-xMove, zoomMove+44, vx.str(), 0, 255, 255, 255);

	std::stringstream vy;
	vy << "VY:" << (floor(sample.particle.vy*100))/100;
	g->drawtext(hudx-77-xMove, zoomMove+44, vy.str(), 0, 255, 255, 255);
	}
}

		int wavelengthGfx = 0, alpha = 255;
		if (toolTipPosition.Y < 120)
			alpha = 255-toolTipPresence*3;
			
		if (alpha < 50)
			alpha = 50;
		StringBuilder sampleInfo;
		sampleInfo << Format::Precision(2);

		int type = sample.particle.type;
		if (type)
		{
			std::stringstream temp;
			temp << "Temp:" << (floor (sample.particle.temp*100))/100-273.15;
			g->drawtext(hudx-147-xMove, zoomMove+15, temp.str(), 0, 255, 255, 255);

			int ctype = sample.particle.ctype;
//<<<<<<< HEAD
			int st=sample.particle.type;
			
			if (sample.particle.type == PT_PIPE || sample.particle.type == PT_PPIP)
				ctype = sample.particle.tmp&0xFF;

			if (sample.particle.type == PT_PHOT || sample.particle.type == PT_BIZR || sample.particle.type == PT_BIZRG || sample.particle.type == PT_BIZRS || sample.particle.type == PT_FILT || sample.particle.type == PT_BRAY)
				wavelengthGfx = ctype;
				if ((sample.particle.type == PT_PIPE || sample.particle.type == PT_PPIP) && c->IsValidElement(ctype))
					ptype << c->ElementResolve((int)sample.particle.pavg[1], 0);
					
			if (sample.particle.type==PT_LAVA && sample.particle.ctype)
			{
				ptype << "Molten " << c->ElementResolve(sample.particle.ctype, sample.particle.type);
				g->drawtext(hudx-212-xMove, zoomMove+15, ptype.str(), 0, 255, 255, 255);
				tmp << "Tmp:()";
			}
			else if (sample.particle.type == PT_GASS && sample.particle.ctype)
			{
				ptype << "Gasseous " << c->ElementResolve(sample.particle.ctype, sample.particle.type);
				g->drawtext(hudx-212-xMove, zoomMove+15, ptype.str(), 0, 255, 255, 255);
				tmp << "Tmp:()";
			}
			else if ((st==PT_SR) && sample.particle.tmp==1)
			{
				ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype) << " Hydroxide";
				g->drawtext(hudx-212-xMove, zoomMove+15, ptype.str(), 0, 255, 255, 255);
				tmp << "Tmp:()";
			}
			else if (sample.particle.type == PT_LIFE)
				ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype);
			else if (sample.particle.type == PT_FILT)
			{
				ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype);
				const char* filtModes[] = {"set", "AND", "OR", "subt", "red", "blue", "none", "XOR", "NOT", "scat", "var red", "var blue"};
				if (ctype>=1000)
				{
					if (ctype>=1000000)
					{
						ptype << ", " << ctype/1000000 << "m";
					}
/*

			if (type == PT_PHOT || type == PT_BIZR || type == PT_BIZRG || type == PT_BIZRS || type == PT_FILT || type == PT_BRAY || type == PT_C5)
				wavelengthGfx = (ctype&0x3FFFFFFF);

			if (showDebug)
			{
				if (type == PT_LAVA && c->IsValidElement(ctype))
					sampleInfo << "Molten " << c->ElementResolve(ctype, -1).FromAscii();
				else if ((type == PT_PIPE || type == PT_PPIP) && c->IsValidElement(ctype))
					sampleInfo << c->ElementResolve(type, -1).FromAscii() << " with " << c->ElementResolve(ctype, (int)sample.particle.pavg[1]).FromAscii();
				else if (type == PT_LIFE)
					sampleInfo << c->ElementResolve(type, ctype).FromAscii();
				else if (type == PT_FILT)
				{
					sampleInfo << c->ElementResolve(type, ctype).FromAscii();
					String filtModes[] = {"set colour", "AND", "OR", "subtract colour", "red shift", "blue shift", "no effect", "XOR", "NOT", "old QRTZ scattering", "variable red shift", "variable blue shift"};
					if (sample.particle.tmp>=0 && sample.particle.tmp<=11)
						sampleInfo << " (" << filtModes[sample.particle.tmp] << ")";
>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945*/
					else
					{
						ptype << ", " << ctype/1000 << "k";
					}
				}
				else
				{
//<<<<<<< HEAD
				ptype << ", " << ctype;
				}
					
				if (sample.particle.tmp>=0 && sample.particle.tmp<=11)
					tmp << "Tmp:" << " (" << filtModes[sample.particle.tmp] << ")";
				else
					tmp << "Tmp:" << " (unkn)";
/*=======
					sampleInfo << c->ElementResolve(type, ctype).FromAscii();
					if (wavelengthGfx)
						sampleInfo << " (" << ctype << ")";
					// Some elements store extra LIFE info in upper bits of ctype, instead of tmp/tmp2
					else if (type == PT_CRAY || type == PT_DRAY || type == PT_CONV)
						sampleInfo << " (" << c->ElementResolve(TYP(ctype), ID(ctype)).FromAscii() << ")";
					else if (c->IsValidElement(ctype))
						sampleInfo << " (" << c->ElementResolve(ctype, -1).FromAscii() << ")";
					else
						sampleInfo << " ()";
				}
				sampleInfo << ", Temp: " << (sample.particle.temp - 273.15f) << " C";
				sampleInfo << ", Life: " << sample.particle.life;
				if (sample.particle.type != PT_RFRG && sample.particle.type != PT_RFGL)
					sampleInfo << ", Tmp: " << sample.particle.tmp;

				// only elements that use .tmp2 show it in the debug HUD
				if (type == PT_CRAY || type == PT_DRAY || type == PT_EXOT || type == PT_LIGH || type == PT_SOAP || type == PT_TRON || type == PT_VIBR || type == PT_VIRS || type == PT_WARP || type == PT_LCRY || type == PT_CBNW || type == PT_TSNS || type == PT_DTEC || type == PT_LSNS || type == PT_PSTN)
					sampleInfo << ", Tmp2: " << sample.particle.tmp2;

				sampleInfo << ", Pressure: " << sample.AirPressure;
>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945*/
			}
			else if (sample.particle.type==PT_NBOT)
			{
				if (sample.particle.tmp==0)
					tmp << "Retrieve(0)";
				if (sample.particle.tmp==1)
					tmp << "Explode(1)";
				if (sample.particle.tmp==2)
					tmp << "Charge(2)";
				if (sample.particle.tmp==3)
					tmp << "Fight(3)";
				if (sample.particle.tmp==4)
					tmp << "Break(4)";
				if (sample.particle.tmp==5)
					tmp << "Replicate(5)";
				if (sample.particle.tmp==6)
					tmp << "Beacon(6)";
				if (sample.particle.tmp==256)
					tmp << "Part Stored";
				if (sample.particle.tmp==257)
					tmp << "Full(5)";
				if (ctype==0)
				{
					ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype) << ", " << "()";
				}
				else
				{
					ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype) << ", " << c->ElementResolve(sample.particle.ctype, sample.particle.type);
				}
			}
			else
/*=======
				if (type == PT_LAVA && c->IsValidElement(ctype))
					sampleInfo << "Molten " << c->ElementResolve(ctype, -1).FromAscii();
				else if ((type == PT_PIPE || type == PT_PPIP) && c->IsValidElement(ctype))
					sampleInfo << c->ElementResolve(type, -1).FromAscii() << " with " << c->ElementResolve(ctype, (int)sample.particle.pavg[1]).FromAscii();
				else if (type == PT_LIFE)
					sampleInfo << c->ElementResolve(type, ctype).FromAscii();
				else
					sampleInfo << c->ElementResolve(type, ctype).FromAscii();
				sampleInfo << ", Temp: " << sample.particle.temp - 273.15f << " C";
				sampleInfo << ", Pressure: " << sample.AirPressure;
			}
		}
		else if (sample.WallType)
		{
			sampleInfo << c->WallName(sample.WallType);
			sampleInfo << ", Pressure: " << sample.AirPressure;
		}
		else if (sample.isMouseInSim)
		{
			sampleInfo << "Empty, Pressure: " << sample.AirPressure;
		}
		else
		{
			sampleInfo << "Empty";
		}

		int textWidth = Graphics::textwidth(sampleInfo.Build());
		g->fillrect(XRES-20-textWidth, 12, textWidth+8, 15, 0, 0, 0, alpha*0.5f);
		g->drawtext(XRES-16-textWidth, 16, sampleInfo.Build(), 255, 255, 255, alpha*0.75f);

#ifndef OGLI
		if (wavelengthGfx)
		{
			int i, cr, cg, cb, j, h = 3, x = XRES-19-textWidth, y = 10;
			int tmp;
			g->fillrect(x, y, 30, h, 64, 64, 64, alpha); // coords -1 size +1 to work around bug in fillrect - TODO: fix fillrect
			for (i = 0; i < 30; i++)
>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945*/
			{
			tmp << "Tmp:" << sample.particle.tmp;
				if (ctype==0)
					{
					ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype) << ", " << "()";
					}
				else
					{
					ptype << c->ElementResolve(sample.particle.type, sample.particle.ctype) << ", " << c->ElementResolve(sample.particle.ctype, sample.particle.type);
					}
				if (wavelengthGfx & PT_PHOT)
					ptype << ctype/4194304;
			}
//<<<<<<< HEAD
g->drawtext(hudx-212-xMove, zoomMove+15, ptype.str(), 0, 255, 255, 255);
g->drawtext(hudx-147-xMove, zoomMove+30, tmp.str(), 0, 255, 255, 255);
}
else if (sample.WallType)
{
	g->drawtext(400-xMove, zoomMove+15, c->WallName(sample.WallType), 255, 255, 255, 255);
	g->drawtext(465-xMove, zoomMove+15, "Temp:()", 0, 255, 255, 75);
	g->drawtext(hudx-147-xMove, zoomMove+30, "Tmp:()", 0, 255, 255, 255);
}
else
{
	g->drawtext(hudx-212-xMove, zoomMove+15, "Empty, ()", 0, 255, 255, 255);
	g->drawtext(hudx-147-xMove, zoomMove+15, "Temp:()", 0, 255, 255, 255);
	g->drawtext(hudx-147-xMove, zoomMove+30, "Tmp:()", 0, 255, 255, 255);
}
}
if (showHud==false)
	{
	RMx = 3;
	RMy = 2;
	RMBx = 0;
	RMBy = 0;
/*=======
		}
#endif

		if (showDebug)
		{
			StringBuilder sampleInfo;
			sampleInfo << Format::Precision(2);

			if (type)
				sampleInfo << "#" << sample.ParticleID << ", ";

			sampleInfo << "X:" << sample.PositionX << " Y:" << sample.PositionY;

			if (sample.Gravity)
				sampleInfo << ", GX: " << sample.GravityVelocityX << " GY: " << sample.GravityVelocityY;

			if (c->GetAHeatEnable())
				sampleInfo << ", AHeat: " << sample.AirTemperature - 273.15f << " C";

			textWidth = Graphics::textwidth(sampleInfo.Build());
			g->fillrect(XRES-20-textWidth, 27, textWidth+8, 14, 0, 0, 0, alpha*0.5f);
			g->drawtext(XRES-16-textWidth, 30, sampleInfo.Build(), 255, 255, 255, alpha*0.75f);
		}
>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945*/
	}

if(c->GetSimulation()->REALvar==true)
	{
//<<<<<<< HEAD
	g->fillrect(RMBx+xMove, RMBy, 92, 12, 55, 55, 55, 200);
	g->drawtext(RMx+xMove, RMy, "Realistic Mode On", 255, 0, 0, 255);
/*=======
		//FPS and some version info
		StringBuilder fpsInfo;
		fpsInfo << Format::Precision(2) << "FPS: " << ui::Engine::Ref().GetFps();

		if (showDebug)
		{
			if (ren->findingElement)
				fpsInfo << " Parts: " << ren->foundElements << "/" << sample.NumParts;
			else
				fpsInfo << " Parts: " << sample.NumParts;
		}
		if (c->GetReplaceModeFlags()&REPLACE_MODE)
			fpsInfo << " [REPLACE MODE]";
		if (c->GetReplaceModeFlags()&SPECIFIC_DELETE)
			fpsInfo << " [SPECIFIC DELETE]";
		if (ren && ren->GetGridSize())
			fpsInfo << " [GRID: " << ren->GetGridSize() << "]";
		if (ren && ren->findingElement)
			fpsInfo << " [FIND]";

		int textWidth = Graphics::textwidth(fpsInfo.Build());
		int alpha = 255-introText*5;
		g->fillrect(12, 12, textWidth+8, 15, 0, 0, 0, alpha*0.5);
		g->drawtext(16, 16, fpsInfo.Build(), 32, 216, 255, alpha*0.75);
>>>>>>> 27f5ba199d70d8b95bcc33d17fb6ce35894f0945*/
	}

	//Tooltips
	if(infoTipPresence)
	{
		int infoTipAlpha = (infoTipPresence>50?50:infoTipPresence)*5;
		g->drawtext_outline((XRES-Graphics::textwidth(infoTip))/2, (YRES/2)-2, infoTip, 255, 255, 255, infoTipAlpha);
	}

	if(toolTipPresence && toolTipPosition.X!=-1 && toolTipPosition.Y!=-1 && toolTip.length())
	{
		if (toolTipPosition.Y == Size.Y-MENUSIZE-10)
			g->drawtext_outline(toolTipPosition.X, toolTipPosition.Y, toolTip, 255, 255, 255, toolTipPresence>51?255:toolTipPresence*5);
		else
			g->drawtext(toolTipPosition.X, toolTipPosition.Y, toolTip, 255, 255, 255, toolTipPresence>51?255:toolTipPresence*5);
	}

	if(buttonTipShow > 0)
	{
		g->drawtext(16, Size.Y-MENUSIZE-24, buttonTip, 255, 255, 255, buttonTipShow>51?255:buttonTipShow*5);
	}

	//Introduction text
	if(introText)
	{
		g->fillrect(0, 0, WINDOWW, WINDOWH, 0, 0, 0, introText>51?102:introText*2);
		g->drawtext(16, 20, introTextMessage, 255, 255, 255, introText>51?255:introText*5);
	}

	// Clear menu areas, to ensure particle graphics don't overlap
	memset(g->vid+((XRES+BARSIZE)*YRES), 0, (PIXELSIZE*(XRES+BARSIZE))*MENUSIZE);
	g->clearrect(XRES, 1, BARSIZE, YRES-1);
}

ui::Point GameView::lineSnapCoords(ui::Point point1, ui::Point point2)
{
	ui::Point diff = point2 - point1;
	if(abs(diff.X / 2) > abs(diff.Y)) // vertical
		return point1 + ui::Point(diff.X, 0);
	if(abs(diff.X) < abs(diff.Y / 2)) // horizontal
		return point1 + ui::Point(0, diff.Y);
	if(diff.X * diff.Y > 0) // NW-SE
		return point1 + ui::Point((diff.X + diff.Y)/2, (diff.X + diff.Y)/2);
	// SW-NE
	return point1 + ui::Point((diff.X - diff.Y)/2, (diff.Y - diff.X)/2);
}

ui::Point GameView::rectSnapCoords(ui::Point point1, ui::Point point2)
{
	ui::Point diff = point2 - point1;
	if(diff.X * diff.Y > 0) // NW-SE
		return point1 + ui::Point((diff.X + diff.Y)/2, (diff.X + diff.Y)/2);
	// SW-NE
	return point1 + ui::Point((diff.X - diff.Y)/2, (diff.Y - diff.X)/2);
}
