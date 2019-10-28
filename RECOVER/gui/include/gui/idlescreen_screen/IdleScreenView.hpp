#ifndef IDLESCREEN_VIEW_HPP
#define IDLESCREEN_VIEW_HPP

#include <gui_generated/idlescreen_screen/IdleScreenViewBase.hpp>
#include <gui/idlescreen_screen/IdleScreenPresenter.hpp>
#include <gui/common/Unlock.hpp>
#include <gui/common/PatternCode.hpp>
#include <gui/common/ProgressBar.hpp>
#include <touchgfx\containers\Slider.hpp>
#include <touchgfx\Callback.hpp>
#include <touchgfx\widgets\AbstractButton.hpp>
#include <touchgfx\events\ClickEvent.hpp>
#include <ctgmath>
#include <gui\common\LFTDebug.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>

//#define ALLOW_SIMPLE_ENGINEER_ENTRY

enum LOCK_MODE_TYPE
{
	LOCK_MODE_PULSE = 0x00,
	LOCK_MODE_NO_PULSE,
	LOCK_MODE_TIME
};

class IdleScreenView : public IdleScreenViewBase
{
public:
	IdleScreenView() :
		PatternExitCallback(this, &IdleScreenView::PatternExit),
		PatternSubmittedCallback(this, &IdleScreenView::PatternEntered),
		UnlockClickedCallback(this, &IdleScreenView::UnlockClicked),
		UnlockSwipedCallback(this, &IdleScreenView::UnlockSwiped)
	{
		allowPulse = false;		
	}	

    virtual ~IdleScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

	virtual void handleTickEvent();	


	virtual void handleClickEvent(const ClickEvent& evt);	
	virtual void handleDragEvent(const DragEvent& evt);

protected:	
	void PatternExit(const touchgfx::AbstractButton& src);
	void PatternEntered(const PatternCode& pattern);
	touchgfx::Callback<IdleScreenView, const touchgfx::AbstractButton&> PatternExitCallback;	
	touchgfx::Callback<IdleScreenView, const PatternCode&> PatternSubmittedCallback;

	LOCK_MODE_TYPE lockType = LOCK_MODE_PULSE;
		
	bool allowPulse;
	
	int tickCounter = 0;

	ProgressBar initialisingBar;		
	PatternCode patternCode;
	LFTDebug lftDebug;

	Unlock unlockSlider;			
	void UnlockClicked(const Unlock& u);
	void UnlockSwiped(const Unlock& u);
	touchgfx::Callback<IdleScreenView, const Unlock&, void> UnlockClickedCallback;
	touchgfx::Callback<IdleScreenView, const Unlock&, void> UnlockSwipedCallback;		

#ifdef ALLOW_SIMPLE_ENGINEER_ENTRY
	static const int SIMPLE_BUFFER_SIZE = 2;
#endif
	static const int FREDA_BUFFER_SIZE = 8;
	static const int DEMO_BUFFER_SIZE = 6;
	static const int EGG_BUFFER_SIZE = 7;	
	static const int TOUCH_TEST_BUFFER_SIZE = 6;

#ifdef ALLOW_SIMPLE_ENGINEER_ENTRY 
	const int SimpleCode[SIMPLE_BUFFER_SIZE]{ 0, 1 };
#endif
	const int FredaCode[FREDA_BUFFER_SIZE]{ 4,5,2,1,0,3,6,7 };
	const int DemoCode[DEMO_BUFFER_SIZE]{ 0,1,5,7,6,3 };
	const int EggCode[EGG_BUFFER_SIZE]{ 2,1,0,3,6,7,8 };
	const int TouchTestCode[TOUCH_TEST_BUFFER_SIZE]{ 0, 1, 3, 4, 6, 7 };

	
	//const int TimeBeforePulse = 10;
	const int TimeBeforePulse = 1; //1 second before the screen starts to pulsate	
	int fadeUpCounter = 0;
	bool queFadeUp = false;
	bool faded = false;
	int extraTimeBeforeFadeOut = 0;
	
	void UpdateLockMode();
	void UpdateClock();
	void SwitchSplash(bool value);

	float EquationFadeOut(float x, float dampening = 60);
	float EquationPulse(float x, float dampening = 60);
};

#endif // IDLESCREEN_VIEW_HPP

