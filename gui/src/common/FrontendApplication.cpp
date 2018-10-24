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
	isEngineerModeOn = false;
}

void FrontendApplication::handleTickEvent()
{		
	FrontendApplicationBase::handleTickEvent();
	
	tickCounter++;				

	/*
	if (tickCounter >= SCREEN_SAVER_TIMEOUT)
	{	
		if (!screenSaverTriggered && AllowIdle)
		{
			screenSaverTriggered = true;
			gotoIdleScreenScreenSlideTransitionSouth();						
		}
	}
	*/
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{	
	//MVPApplication::handleClickEvent(evt);	
	//Don't allow click events if demo mode is on
	if (!isDemoModeOn)
		MVPApplication::handleClickEvent(evt);	
	else
	{								
		if (evt.getType() == ClickEvent::RELEASED)
		{
			//If less than 10 seconds			
			if (tickCounter < 600)
				return;

			isDemoModeOn = false;
			gotoIdleScreenScreenNoTransition();
		}
		
	}	

	// A click event has been registered so reset last click time
	tickCounter = 0;
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{		
	//Don't allow drag events if demo mode is on
	if (!isDemoModeOn)
		MVPApplication::handleDragEvent(evt);
	
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

void FrontendApplication::gotoMainScreenScreenSlideTransitionNorth()
{
	transitionCallback = touchgfx::Callback<FrontendApplication>(this, &FrontendApplication::gotoMainScreenScreenSlideTransitionNorthImpl);
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

void FrontendApplication::gotoMainScreenScreenSlideTransitionNorthImpl()
{
	makeTransition<MainScreenView, MainScreenPresenter, touchgfx::SlideTransition<NORTH>, Model>(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}



