#include <gui/idlescreen_screen/IdleScreenView.hpp>


void IdleScreenView::setupScreen()
{
	//Don't allow screensaver over screensaver
	presenter->DisallowScreensaver();		

	//Secret Slider to enter engineer window
	unlockSlider.SetSize(186, 273, 434, 71);	
	unlockSlider.SetClickEvent(UnlockClickedCallback);
	unlockSlider.SetSwipeEvent(UnlockSwipedCallback);	
	
	
	//Engineer pattern code 
	patternCode.setXY(225, 40);		
	patternCode.SetExitAction(PatternExitCallback);
	patternCode.SetPatternEnteredAction(PatternSubmittedCallback);
	patternCode.setVisible(false);

	//Initialisation bar	
	initialisingBar.setXY(125, 360);
	initialisingBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255,255,255));
	initialisingBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	initialisingBar.setWidth(550);
	initialisingBar.setHeight(20);	
	initialisingBar.SetMaximum(LFT::Information.RefreshValuesMaxmimum);
	initialisingBar.SetTextVisible(false);	
#ifndef SIMULATOR
	initialisingBar.setVisible(LFT::Information.RefreshValuesRequired);
#else
	//This process doesn't happen on the simulator
	initialisingBar.setVisible(true);	
	initialisingBar.SetMaximum(10);

	initialisingBar.SetValue(6);
#endif // SIMULATOR	

	if (LFT::Information.RefreshValuesRequired)
	{	
		//When starting up, show main screen for 60 seconds longer!
		extraTimeBeforeFadeOut = 60 * 60;		
	}
	else
	{
		//Show set lock type
		lockType = (LOCK_MODE_TYPE)LFT::Settings.GetLockMode();
	}


	add(initialisingBar);		
	add(unlockSlider);		
	add(patternCode);
	
	if (LFT::Information.EngineeringMode)
	{		
		lftDebug.setXY(0, 70);
		lftDebug.setWidth(240);
		lftDebug.setHeight(410);		
		lftDebug.SetTextEnabledColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
		add(lftDebug);				
	}
			
	UpdateLockMode();
	UpdateClock();
}

void IdleScreenView::tearDownScreen()
{
	//Set backlight to 100% when window is exited
	presenter->SetBacklight(100);
}

void IdleScreenView::handleTickEvent()
{	
	//Update clock
	if (DateClock.isVisible())				
		UpdateClock();
	

	//Update Initialise Progress bar
	if (LFT::Information.RefreshValuesRequired)
	{
		initialisingBar.handleTickEvent();		
#ifndef SIMULATOR
		initialisingBar.SetValue(LFT::Information.RefreshValuesProgress);
#endif
	}
	else if (initialisingBar.isVisible())
	{
		//Allow pulse now		
		allowPulse = true;

		//Since initialisation is finished, use the settings we just loaded to setup the software		
		

		initialisingBar.setVisible(false);
		initialisingBar.invalidate();
	}		

	//Show engineering values on screen
	if (LFT::Information.EngineeringMode)
	{
		LFT::Information.UpdateLFTDebug(&lftDebug);
		lftDebug.invalidate();
		return;
	}

	//Backlight Animation
	if (allowPulse && lockType != LOCK_MODE_TIME && !queFadeUp)
	{
		tickCounter++;

		//Stop light from pulsing until the set time has passed
		if (tickCounter < ((TimeBeforePulse * 60) + extraTimeBeforeFadeOut))
			return;


		//If pattern code is visible, this isn't appropriate
		if (patternCode.isVisible())
			return;


		if (lockType != LFT::Settings.GetLockMode())
		{
			lockType = (LOCK_MODE_TYPE)LFT::Settings.GetLockMode();
			UpdateLockMode();
			return;
		}

		int pulseCounter = tickCounter - TimeBeforePulse * 60;
		float value = 0;
		if (!faded)
		{
			//Fade to 0!		
			value = EquationFadeOut(pulseCounter, 15.0f);

			//If faded
			if (pulseCounter >= 47)
				faded = true;

		}
		else if (lockType == LOCK_MODE_PULSE)
		{
			//Pulse backlight
			value = EquationPulse(pulseCounter + 229);

			//Reset at 120Pi
			if (pulseCounter % 376 == 0)
				tickCounter = TimeBeforePulse * 60;
		}							

		//Set led backlight
		presenter->SetBacklight(value);
	}
	if (queFadeUp)
	{
		//Set endValue to work back from
		if (faded)
		{
			int endValue = 47;
			if (lockType == LOCK_MODE_TIME)
				endValue = 32;


			//Fade Value Up
			fadeUpCounter++;
			float value = 100;
			if (fadeUpCounter < endValue)
				value = EquationFadeOut(endValue - fadeUpCounter, 15);
			else if (fadeUpCounter < endValue + 18)
				value = 100;
			else
				faded = false;

			presenter->SetBacklight(value);		
		}
		else		
			application().gotoMainScreenNoTransition();
		

	}


}

void IdleScreenView::handleClickEvent(const ClickEvent & evt)
{			
	if (patternCode.isVisible())
	{	
		ClickEvent patternEvent(
			evt.getType(),			
			evt.getX() - patternCode.getX(),
			evt.getY() - patternCode.getY()
		);

		patternCode.handleClickEvent(patternEvent);
		return;
	}
	
	unlockSlider.handleClickEvent(evt);


	if (initialisingBar.isVisible())
		return;

	if (evt.getType() == ClickEvent::RELEASED)
	{
		if (LFT::Information.EngineeringMode)
		{
			application().gotoMainScreenNoTransition();
			return;
		}


		if (!queFadeUp)
		{
			fadeUpCounter = 0;
			queFadeUp = true;
		}
	}
}

void IdleScreenView::handleDragEvent(const DragEvent & evt)
{
	//If pattern code is visible, send event to patterncode for processing
	if (patternCode.isVisible())
	{		
		DragEvent patternEvent(
			evt.getType(),
			evt.getOldX() - patternCode.getX(),
			evt.getOldY() - patternCode.getY(),
			evt.getNewX() - patternCode.getX(),
			evt.getNewY() - patternCode.getY()
		);

		patternCode.handleDragEvent(patternEvent);
	}

	//If unlock slider is visible and event is within the patternCode areas, send event to pattern code for processing
	if (unlockSlider.isVisible() && Utils::IsPointWithinRectagle(unlockSlider.getX(), unlockSlider.getY(), unlockSlider.getWidth(), unlockSlider.getHeight(), evt.getOldX(), evt.getOldY()))
	{		
		DragEvent unlockEvent(
			evt.getType(),
			evt.getOldX() - unlockSlider.getX(),
			evt.getOldY() - unlockSlider.getY(),
			evt.getNewX() - unlockSlider.getX(),
			evt.getNewY() - unlockSlider.getY());


		unlockSlider.handleDragEvent(unlockEvent);
	}
}


void IdleScreenView::PatternExit(const touchgfx::AbstractButton & src)
{
	//Allow the window to pulse again
	allowPulse = true;	
	tickCounter = 0;

	//Hide the pattern screen
	patternCode.ClearLine();
	patternCode.setVisible(false);
	
	//Show full splash screen
	UpdateLockMode();
}

void IdleScreenView::PatternEntered(const PatternCode & pattern)
{
	//Get Path
	int pathLength = patternCode.GetPathSize();
	int *path = patternCode.GetPath();	

	//Check if it was the pattern for the FREDA Screen
#ifdef ALLOW_SIMPLE_ENGINEER_ENTRY
	if (pathLength == SIMPLE_BUFFER_SIZE && Utils::CompareArrays(path, SimpleCode, pathLength))	
		application().gotoFREDAScreenNoTransition();	
#endif

	if (pathLength == FREDA_BUFFER_SIZE && Utils::CompareArrays(path, FredaCode, pathLength))
		application().gotoFREDAScreenNoTransition();

	//Check if it was the pattern for the Demo Mode
	if (pathLength == DEMO_BUFFER_SIZE && Utils::CompareArrays(path, DemoCode, pathLength))
	{
		application().isDemoModeOn = true;
		application().gotoProcessScreenScreenNoTransition();
	}

	//Check if it was the pattern for the easter egg
	if (pathLength == EGG_BUFFER_SIZE && Utils::CompareArrays(path, EggCode, pathLength))
		application().gotoEggNoTransition();

	//Check if it was the pattern for the touchscreen test
	if (pathLength == TOUCH_TEST_BUFFER_SIZE && Utils::CompareArrays(path, TouchTestCode, pathLength))
		application().gotoTouchscreenTestNoTransition();
}
	


void IdleScreenView::UnlockClicked(const Unlock & u)
{		

}

void IdleScreenView::UnlockSwiped(const Unlock & u)
{		
	//Don't allow backlight pulsing, it if was pulsing bright it back up to max brightness
	allowPulse = false;
	faded = false;
	presenter->SetBacklight(100);

	DateClock.setVisible(false);
	DateClock.invalidate();

	
	//Open the swipe password for the engineering window (or the secret)		
	patternCode.setVisible(true);

	//Show empty splash screen
	SwitchSplash(false);
}

void IdleScreenView::UpdateLockMode()
{
	switch (lockType)
	{
	case LOCK_MODE_NO_PULSE:
	case LOCK_MODE_PULSE:
		DateClock.setVisible(false);		
		SwitchSplash(true);				
		break;

	case LOCK_MODE_TIME:
		DateClock.setVisible(true);		
		SwitchSplash(false);
		presenter->SetBacklight(15);
		faded = true;		
		break;
	}	

	unlockSlider.invalidate();
	DateClock.invalidate();
	RecoverSplash.invalidate();
	EmptySplash.invalidate();
}

void IdleScreenView::UpdateClock()
{
	STime current = STime(LFT::Information.GetCurrentTime().getRaw());
	Unicode::snprintf(txtClockBuffer, TXTCLOCK_SIZE, "%02d:%02d:%02d", current.GetHour(), current.GetMinute(), current.GetSecond());
	txtClock.invalidate();


	const uint16_t* month = Utils::GetMonthString(current.GetMonth(), true);
	Unicode::snprintf(txtDateBuffer, TXTDATE_SIZE, "%02d %s %04d",
		current.GetDay(),
		month,
		current.GetYear());

	txtDate.invalidate();
}

void IdleScreenView::SwitchSplash(bool value)
{
	RecoverSplash.setVisible(value);
	EmptySplash.setVisible(!value);

	RecoverSplash.invalidate();
}

float IdleScreenView::EquationFadeOut(float x, float dampening)
{
	/*
		XOffset = 30pi
		YOffset = 50 (1/2 of 100)
		Dampening = 60
		Heightening = 50 (1/2 of 100)

			   x + 30pi
		50 + ( sin(----------) * 50 )
			     60		
	*/

	float xOffset = dampening / 2;

	return 50.0f + (sin((x + (xOffset * 3.14f)) / dampening) * 50.0f);

}

float IdleScreenView::EquationPulse(float x, float dampening)
{
	/*
		XOffset = 94.257
		YOffset = 0
		Dampening = 60
		Heightening = 15		


					x + 94.257
			   sin(------------)
		0 + 15		   60
		

	*/


	return 0.0f + pow(15.0f, sin((x + 94.257) / dampening));
}
