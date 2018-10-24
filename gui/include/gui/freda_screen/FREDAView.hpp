#ifndef FREDA_VIEW_HPP
#define FREDA_VIEW_HPP

#include <gui_generated/freda_screen/FREDAViewBase.hpp>
#include <gui/freda_screen/FREDAPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>
#include <inttypes.h>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <cstring>


class FREDAView : public FREDAViewBase
{
public:
	FREDAView() : 
		enterClickedCallback(this, &FREDAView::enterPressedHandler)
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

	//Auto Commands
	virtual void SetHeaters();
	virtual void Prechecks();
	virtual void PumpDown();
	virtual void Heat();
	virtual void Cool();
	virtual void Off();	

	//Misc Commands	
	virtual void ResetTimer();

	//Switch Modes
	virtual void GotoCustom();
	virtual void GotoPresets();
	virtual void GotoAuto();
	
	//Update LFT Information
	virtual void handleTickEvent();
	virtual void GetValues();				

	touchgfx::Callback<FREDAView, const CustomKeyboard&, void> enterClickedCallback;
	void enterPressedHandler(const CustomKeyboard& u);
protected:

private:
	LFTDebug lftDebug;
	int tickCounter = 0;
	
};

#endif // FREDA_VIEW_HPP
