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

class IdleScreenView : public IdleScreenViewBase
{
public:
	IdleScreenView() :
		PatternExitCallback(this, &IdleScreenView::PatternExit),
		PatternSubmittedCallback(this, &IdleScreenView::PatternEntered),
		UnlockClickedCallback(this, &IdleScreenView::UnlockClicked),
		UnlockSwipedCallback(this, &IdleScreenView::UnlockSwiped)
	{}	

    virtual ~IdleScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

	virtual void handleTickEvent();
	virtual void gotoMainScreen();
	
protected:	
	void PatternExit(const touchgfx::AbstractButton& src);
	void PatternEntered(const PatternCode& pattern);
	touchgfx::Callback<IdleScreenView, const touchgfx::AbstractButton&> PatternExitCallback;	
	touchgfx::Callback<IdleScreenView, const PatternCode&> PatternSubmittedCallback;
	
	bool compareArrays(int* array1, const int* array2, int size);
	bool allowPulse;
	
	int tickCounter = 0;

	ProgressBar initialisingBar;	
	ModalWindow patternModalWindow;
	PatternCode patternCode;
	LFTDebug lftDebug;

	Unlock unlockSlider;			
	void UnlockClicked(const Unlock& u);
	void UnlockSwiped(const Unlock& u);
	touchgfx::Callback<IdleScreenView, const Unlock&, void> UnlockClickedCallback;
	touchgfx::Callback<IdleScreenView, const Unlock&, void> UnlockSwipedCallback;		

	static const int SIMPLE_BUFFER_SIZE = 2;
	static const int FREDA_BUFFER_SIZE = 8;
	static const int DEMO_BUFFER_SIZE = 6;
	static const int EGG_BUFFER_SIZE = 7;	

	const int SimpleCode[SIMPLE_BUFFER_SIZE]{ 0, 1 };
	const int FredaCode[FREDA_BUFFER_SIZE]{ 4,5,2,1,0,3,6,7 };
	const int DemoCode[DEMO_BUFFER_SIZE]{ 0,1,5,7,6,3 };
	const int EggCode[EGG_BUFFER_SIZE]{ 2,1,0,3,6,7,8 };

	const int TimeBeforePulse = 120; //120 seconds before the screen starts to pulsate
	const int MaxBrightness = 10;
	const int MinBrightness = 0;	
	const int Duration = 20;
	const int Dampening = 60;	
};

#endif // IDLESCREEN_VIEW_HPP

