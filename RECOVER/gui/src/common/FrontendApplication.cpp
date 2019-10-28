#include <gui/common/FrontendApplication.hpp>
#include <gui/idlescreen_screen/IdleScreenView.hpp>
#include <gui/idlescreen_screen/IdleScreenPresenter.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Texts.hpp>
#include <BitmapDatabase.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{	
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{								
	if (evt.getType() == ClickEvent::PRESSED)
	{		
		clickDown = LFT::Information.GetCurrentTime();
		isIdle = false;
	}

	if (isDemoModeOn)
	{							
		if (evt.getType() == ClickEvent::PRESSED)
		{					
			isDemoModePaused = !isDemoModePaused;			
		}

		if (evt.getType() == ClickEvent::RELEASED)
		{
			//Most be more than 5 seconds since click
			STime differenceTimeData(LFT::Information.GetCurrentTime().getRaw() - clickDown.getRaw());
			if (differenceTimeData.get() <= 5)
				return;

			isDemoModeOn = false;			
			gotoIdleScreenScreenNoTransition();
		}		

		return;
	}		

	MVPApplication::handleClickEvent(evt);
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{		
	//Don't allow drag events if demo mode is on
	if (!isDemoModeOn)
		MVPApplication::handleDragEvent(evt);	
}

void FrontendApplication::handleTickEvent()
{
	model.tick();
	FrontendApplicationBase::handleTickEvent();

	//If the click is valid, isn't already idle and idle is allowed
	if (clickDown.isValid() && !isIdle && AllowIdle)
	{
		long long difference = LFT::Information.GetCurrentTime().getRaw() - clickDown.getRaw();

		//If 15 minutes since click	
		if (difference >= 900)
		{
			if (LFT::Auto.GetStage() == LFT_STAGE_LID_CONTROL)
			{				
				isIdle = true;
				gotoIdleScreenScreenNoTransition();
			}
		}					
	}
}

void FrontendApplication::gotoFREDAScreenNoTransition()
{		
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoFREDAScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoEggNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoEggNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainScreenNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoMainScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoMainScreenScreenSlideTransitionNorth()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoMainScreenScreenSlideTransitionNorthImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoUsbModeScreenNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoUsbModeScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGraphScreenNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoGraphScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoAuditScreenNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoAuditScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoPowerlossRecoveryScreenNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoPowerlossRecoveryScreenNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoTouchscreenTestNoTransition()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoTouchscreenTestNoTransitionImpl);
	pendingScreenTransitionCallback = &transitionCallback;
}


void FrontendApplication::gotoFREDAScreenNoTransitionImpl()
{
	makeTransition<FREDAView, FREDAPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoEggNoTransitionImpl()
{
	makeTransition<EggScreenView, EggScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoMainScreenNoTransitionImpl()
{
	makeTransition<MainScreenView, MainScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoMainScreenScreenSlideTransitionNorthImpl()
{
	makeTransition<MainScreenView, MainScreenPresenter, touchgfx::SlideTransition<NORTH>, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoUsbModeScreenNoTransitionImpl()
{
	makeTransition<UsbModeView, UsbModePresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoGraphScreenNoTransitionImpl()
{
	makeTransition<GraphScreenView, GraphScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoAuditScreenNoTransitionImpl()
{
	makeTransition<AuditScreenView, AuditScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoPowerlossRecoveryScreenNoTransitionImpl()
{
	makeTransition<PowerlossRecoveryScreenView, PowerlossRecoveryScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}

void FrontendApplication::gotoTouchscreenTestNoTransitionImpl()
{
	makeTransition<TouchTestScreenView, TouchTestScreenPresenter, touchgfx::NoTransition, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}



