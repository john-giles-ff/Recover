#ifndef FREDA_VIEW_HPP
#define FREDA_VIEW_HPP

#include <gui_generated/freda_screen/FREDAViewBase.hpp>
#include <gui/freda_screen/FREDAPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>
#include <inttypes.h>
#include <gui\LFT\LFT_TestValues.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui\common\graph_widget\Graph.hpp>
#include <cstring>

constexpr int STANDBYBUTTON_COUNT = 7;

class FREDAView : public FREDAViewBase
{
public:
	FREDAView() : 		
		currentLanguage(GB),
		onScreenActualValues(0), 
		onScreenMaxBaseCurrent(0),
		onScreenMaxPreCurrent(0),
		enterClickedCallback(this, &FREDAView::enterPressedHandler), 
		ClockKeypadReturnedCallback(this, &FREDAView::ClockKeypadReturned)
	{

	};
    virtual ~FREDAView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

	//Settings
	virtual void ToggleEngineerMode();

	//Custom Commands
	virtual void SendAndRead();					
	CustomKeyboard keyboard;

	//Preset Commands
	//Lid Commands
	virtual void LidUp();
	virtual void LidStop();
	virtual void LidDown();	

	//Manual Commands
	virtual void ToggleInlet();
	virtual void TogglePurge();
	virtual void ToggleBypass();
	virtual void TogglePump();
	virtual void ToggleBaseFan();
	virtual void ToggleStiringFans();
	virtual void ToggleBaseHeater();
	virtual void TogglePrecursorHeater();	
	virtual void ToggleStandby();	
	virtual void ToggleHideData();
	virtual void BreakSeal();
	virtual void ToggleOpenDoor();

	//Auto Commands	
	virtual void Off();	
	virtual void Clear();		

	//Production Tests	
	virtual void SoakTest();
	virtual void StopProductionTest();

	//Misc Commands	
	virtual void Format();
	virtual void ResetTimer();
	virtual void ToggleEMCMode();
	virtual void ShowClockConfig();

	//Switch Modes
	virtual void GotoCustom();
	virtual void GotoPresets();
	virtual void GotoAuto();
	virtual void GotoGraphs();
	
	//Update LFT Information
	virtual void handleTickEvent();
	virtual void GetValues();					

	//Graph Controls
	virtual void ToggleActualVisible();
	virtual void AbortGraphTest();
	virtual void SwitchGraphMode();
	virtual void StartGraphTest();

	//Callbacks and Handlers
	touchgfx::Callback<FREDAView, const CustomKeyboard&, void> enterClickedCallback;
	void enterPressedHandler(const CustomKeyboard& u);	

	void ClockKeypadReturned(const Keypad& u, bool state);
	touchgfx::Callback<FREDAView, const Keypad&, bool>ClockKeypadReturnedCallback;

protected:

private:
	void UpdateControlStates();
	void UpdateStandbyButtons();
	
	Button *standbyButtons[STANDBYBUTTON_COUNT]
	{
		&BtnToggleInlet,
		&BtnTogglePurge,
		&BtnToggleBypass,
		&BtnTogglePump,
		&BtnToggleBaseHeater,
		&BtnTogglePrecursorHeater,
		&BtnToggleOpenDoor
	};


	LFTDebug lftDebug;	
	Graph upperGraph;
	Graph actualGraph;
	Graph lowerGraph;
	GraphType currentGraphType = Graph_Pressure;
	void UpdateGraphs();

	int onScreenActualValues;
	float onScreenMaxBaseCurrent;
	float onScreenMaxPreCurrent;

	LanguageId currentLanguage;

	bool IsBigClockVisible = false;
};

#endif // FREDA_VIEW_HPP
