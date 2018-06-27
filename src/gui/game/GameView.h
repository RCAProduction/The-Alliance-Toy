#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <vector>
#include <queue>
#include <deque>
#include "common/String.h"
#include "GameController.h"
#include "GameModel.h"
#include "gui/interface/Window.h"
#include "gui/interface/Point.h"
#include "gui/interface/Button.h"
#include "gui/interface/Slider.h"
#include "gui/interface/Textbox.h"
#include "ToolButton.h"
#include "Brush.h"
#include "simulation/Sample.h"

using namespace std;

enum DrawMode
{
	DrawPoints, DrawLine, DrawRect, DrawFill
};

enum SelectMode
{
	SelectNone, SelectStamp, SelectCopy, SelectCut, PlaceSave
};

class GameController;
class GameModel;
class GameView: public ui::Window
{
private:
	bool isMouseDown;
	bool skipDraw;
	bool zoomEnabled;
	bool zoomCursorFixed;
	bool mouseInZoom;
	bool drawSnap;
	bool shiftBehaviour;
	bool ctrlBehaviour;
	bool altBehaviour;
	bool showHud;
	bool showDebug;
	int delayedActiveMenu;
	bool wallBrush;
	bool toolBrush;
	bool decoBrush;
	bool windTool;
	int toolIndex;
	int currentSaveType;
	int lastMenu;

	int toolTipPresence;
	String toolTip;
	bool isToolTipFadingIn;
	ui::Point toolTipPosition;
	int infoTipPresence;
	String infoTip;
	int buttonTipShow;
	String buttonTip;
	bool isButtonTipFadingIn;
	int introText;
	String introTextMessage;

	bool doScreenshot;
	int screenshotIndex;
	bool recording;
	int recordingFolder;
	int recordingIndex;

	ui::Point currentPoint, lastPoint;
	GameController * c;
	Renderer * ren;
	Brush * activeBrush;
	//UI Elements
	vector<ui::Button*> quickOptionButtons;
	vector<ui::Button*> menuButtons;
	vector<ToolButton*> toolButtons;
	vector<ui::Component*> notificationComponents;
	deque<std::pair<String, int> > logEntries;
	ui::Button * scrollBar;
	ui::Button * searchButton;
	ui::Button * reloadButton;
	ui::Button * saveSimulationButton;
	bool saveSimulationButtonEnabled;
	bool saveReuploadAllowed;
	ui::Button * downVoteButton;
	ui::Button * upVoteButton;
	ui::Button * tagSimulationButton;
	ui::Button * clearSimButton;
	ui::Button * loginButton;
	ui::Button * simulationOptionButton;
	ui::Button * displayModeButton;
	ui::Button * pauseButton;
	ui::Button * serverButton;

	ui::Button * colourPicker;
	vector<ToolButton*> colourPresets;

	DrawMode drawMode;
	ui::Point drawPoint1;
	ui::Point drawPoint2;

	SelectMode selectMode;
	ui::Point selectPoint1;
	ui::Point selectPoint2;
	
	ui::Point currentMouse;
	ui::Point mousePosition;

	VideoBuffer * placeSaveThumb;
	ui::Point placeSaveOffset;

	SimulationSample sample;

	void updateToolButtonScroll();

	void SetSaveButtonTooltips();

	void screenshot();

	void enableShiftBehaviour();
	void disableShiftBehaviour();
	void enableCtrlBehaviour();
	void disableCtrlBehaviour();
	void enableAltBehaviour();
	void disableAltBehaviour();
	void UpdateDrawMode();
	void UpdateToolStrength();

	bool menuClick;
public:
	GameView();
	virtual ~GameView();

	//Breaks MVC, but any other way is going to be more of a mess.
	ui::Point GetMousePosition();
	void SetSample(SimulationSample sample);
	void SetHudEnable(bool hudState);
	bool GetHudEnable();
	void SetDebugHUD(bool mode);
	bool GetDebugHUD();
	bool GetPlacingSave();
	bool GetPlacingZoom();
	void SetActiveMenuDelayed(int activeMenu) { delayedActiveMenu = activeMenu; }
	bool CtrlBehaviour(){ return ctrlBehaviour; }
	bool ShiftBehaviour(){ return shiftBehaviour; }
	bool AltBehaviour(){ return altBehaviour; }

	bool pmessage;
	bool newsPause;
	bool SetMenu;

	int count;
	int FPS1;
	int FPS2;
	int FPS3;
	int FPS4;
	int FPS5;
	int TESTx;
	int RMx;
	int RMy;
	int RMBx;
	int RMBy;
	int infoX;
	int infoY;
	int frameCount;
	int cctype;
	int zoomMove;
	int xMove;
	int superMove;
	int superX;
	int countt;
	int pa20;
	int pa19;
	int pa18;
	int pa17;
	int pa16;
	int pa15;
	int pa14;
	int pa13;
	int pa12;
	int pa11;
	int pa10;
	int pa9;
	int pa8;
	int pa7;
	int pa6;
	int pa5;
	int pa4;
	int pa3;
	int pa2;
	int pa1;
	int pixAmount;
	int xx;
	int factor;
	int targetx;
	int targety;
	int accelcount;
	int accel;
	int acceltimer;
	int scroll;
	int scrolling;
	int scrollbar;
	int targetfire;

	bool INFOvar;
	bool FPSGvar;
	bool REALvar;
	bool LinkVar;
	bool superDebug;
	bool elCount;
	bool targetxadd;
	bool targetxsubtract;
	bool targetyadd;
	bool targetysubtract;
	bool invertvar;
	bool fired;
	bool news;
	bool localServer;

	void ExitPrompt();

	SelectMode GetSelectMode() { return selectMode; }
	void BeginStampSelection();
	ui::Point GetPlaceSaveOffset() { return placeSaveOffset; }
	void SetPlaceSaveOffset(ui::Point offset) { placeSaveOffset = offset; }
	int Record(bool record);

	//all of these are only here for one debug lines
	bool GetMouseDown() { return isMouseDown; }
	bool GetDrawingLine() { return drawMode == DrawLine && isMouseDown; }
	bool GetDrawSnap() { return drawSnap; }
	ui::Point GetLineStartCoords() { return drawPoint1; }
	ui::Point GetLineFinishCoords() { return currentMouse; }
	ui::Point lineSnapCoords(ui::Point point1, ui::Point point2);
	ui::Point rectSnapCoords(ui::Point point1, ui::Point point2);

	void AttachController(GameController * _c){ c = _c; }
	void NotifyRendererChanged(GameModel * sender);
	void NotifySimulationChanged(GameModel * sender);
	void NotifyPausedChanged(GameModel * sender);
	void NotifySaveChanged(GameModel * sender);
	void NotifyBrushChanged(GameModel * sender);
	void NotifyMenuListChanged(GameModel * sender);
	void NotifyToolListChanged(GameModel * sender);
	void NotifyActiveToolsChanged(GameModel * sender);
	void NotifyUserChanged(GameModel * sender);
	void NotifyZoomChanged(GameModel * sender);
	void NotifyColourSelectorVisibilityChanged(GameModel * sender);
	void NotifyColourSelectorColourChanged(GameModel * sender);
	void NotifyColourPresetsChanged(GameModel * sender);
	void NotifyColourActivePresetChanged(GameModel * sender);
	void NotifyPlaceSaveChanged(GameModel * sender);
	void NotifyNotificationsChanged(GameModel * sender);
	void NotifyLogChanged(GameModel * sender, String entry);
	void NotifyToolTipChanged(GameModel * sender);
	void NotifyInfoTipChanged(GameModel * sender);
	void NotifyQuickOptionsChanged(GameModel * sender);
	void NotifyLastToolChanged(GameModel * sender);


	virtual void ToolTip(ui::Point senderPosition, String toolTip);

	virtual void OnMouseMove(int x, int y, int dx, int dy);
	virtual void OnMouseDown(int x, int y, unsigned button);
	virtual void OnMouseUp(int x, int y, unsigned button);
	virtual void OnMouseWheel(int x, int y, int d);
	virtual void OnKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
	virtual void OnKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
	virtual void OnTick(float dt);
	virtual void OnDraw();
	virtual void OnBlur();

	//Top-level handlers, for Lua interface
	virtual void DoTick(float dt);
	virtual void DoDraw();
	virtual void DoMouseMove(int x, int y, int dx, int dy);
	virtual void DoMouseDown(int x, int y, unsigned button);
	virtual void DoMouseUp(int x, int y, unsigned button);
	virtual void DoMouseWheel(int x, int y, int d);
	virtual void DoKeyPress(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);
	virtual void DoKeyRelease(int key, int scan, bool repeat, bool shift, bool ctrl, bool alt);

	class MenuAction;
	class ToolAction;
	class OptionAction;
	class OptionListener;
};

#endif // GAMEVIEW_H
