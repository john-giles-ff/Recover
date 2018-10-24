#include <gui/idlescreen_screen/IdleScreenView.hpp>


void IdleScreenView::setupScreen()
{
	//Don't allow screensaver over screensaver
	presenter->DisallowScreensaver();		

	//Secret Slider to enter engineer window
	unlockSlider.SetSize(186, 273, 434, 71);	
	unlockSlider.SetClickEvent(UnlockClickedCallback);
	unlockSlider.SetSwipeEvent(UnlockSwipedCallback);	

	//Engineer window password modal window
	patternModalWindow.setXY(0, 0);
	patternModalWindow.setBackground(BITMAP_PATTERNBACKGROUND_ID);
	patternModalWindow.setVisible(false);
	
	//Engineer pattern code 
	patternCode.setXY(0, 0);		
	patternCode.SetExitAction(PatternExitCallback);
	patternCode.SetPatternEnteredAction(PatternSubmittedCallback);

	//Initialisation bar	
	initialisingBar.setXY(125, 360);
	initialisingBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255,255,255));
	initialisingBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	initialisingBar.setWidth(550);
	initialisingBar.setHeight(20);
	initialisingBar.SetMaximum(presenter->GetRefreshMaximum());		
	initialisingBar.SetTextVisible(false);
#ifndef SIMULATOR
	initialisingBar.setVisible(presenter->GetRefreshValuesRequired());
#else
	//This process doesn't happen on the simulator
	initialisingBar.setVisible(true);	
	initialisingBar.SetMaximum(10);

	initialisingBar.SetValue(6);
#endif // SIMULATOR	
	add(initialisingBar);
	
	//Allow the background to pulse
	allowPulse = true;		
	
	patternModalWindow.add(patternCode);
	add(unlockSlider);	
	add(patternModalWindow);	

	if (application().isEngineerModeOn)
	{		
		lftDebug.setXY(0, 70);
		lftDebug.setWidth(240);
		lftDebug.setHeight(410);		
		lftDebug.SetTextEnabledColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
		add(lftDebug);		
	}
	
}

void IdleScreenView::tearDownScreen()
{
	//Set backlight to 100% when window is exited
	presenter->SetBacklight(100);
}

void IdleScreenView::handleTickEvent()
{		

	if (presenter->GetRefreshValuesRequired())
	{
		initialisingBar.handleTickEvent();
#ifndef SIMULATOR
		initialisingBar.SetValue(presenter->GetRefreshProgress());
#endif
	}
	else if (initialisingBar.isVisible())
	{
		initialisingBar.setVisible(false);
		initialisingBar.invalidate();
	}		


	if (application().isEngineerModeOn)
	{
		presenter->UpdateLFTDebug(&lftDebug);
		lftDebug.invalidate();
		return;
	}

	if (!allowPulse)
		return;

	tickCounter++;	

	//Stop light from pulsing until the set time has passed
	if (tickCounter / 60 < TimeBeforePulse)
		return;		

	//Pulses backlight - .......''''........
	//Readable Format: 0 + 10^(sin(x / 60)) 
	float value = MinBrightness + pow(MaxBrightness, sin((float)tickCounter / Dampening));

	//Throbs backlight - .....''''''''......
	//Readble Format: sin(x / 60) * 5 + (5 + 0)
	//float halfMax = MaxBrightness / 2;
	//float value = sin((float)tickCounter / Dampening) * halfMax + (halfMax + MinBrightness);	

	//Set led backlight
	presenter->SetBacklight(value);
}

void IdleScreenView::gotoMainScreen()
{
#ifndef SIMULATOR
	//This process doesn't happen on the simulator
	if (presenter->GetRefreshValuesRequired())
		return;
#endif // !SIMULATOR

	application().gotoMainScreenScreenSlideTransitionNorth();
}


void IdleScreenView::PatternExit(const touchgfx::AbstractButton & src)
{
	//Allow the window to pulse again
	allowPulse = true;	
	tickCounter = 0;

	//Hide the pattern screen
	patternModalWindow.setVisible(false);
	patternModalWindow.invalidate();
}

void IdleScreenView::PatternEntered(const PatternCode & pattern)
{
	//Get Path
	int pathLength = patternCode.GetPathSize();
	int *path = patternCode.GetPath();	

	//Check if it was the pattern for the FREDA Screen
	if ((pathLength == FREDA_BUFFER_SIZE && compareArrays(path, FredaCode, pathLength)) || (pathLength == SIMPLE_BUFFER_SIZE && compareArrays(path, SimpleCode, pathLength)))
		application().gotoFREDAScreenNoTransition();

	//Check if it was the pattern for the Demo Mode
	if (pathLength == DEMO_BUFFER_SIZE && compareArrays(path, DemoCode, pathLength))
	{
		application().isDemoModeOn = true;
		application().gotoProcessScreenScreenNoTransition();
	}


	//Check if it was the patter for the easter egg
	if (pathLength == EGG_BUFFER_SIZE && compareArrays(path, EggCode, pathLength))
		application().gotoEggNoTransition();
}

bool IdleScreenView::compareArrays(int * array1, const int * array2, int size)
{
	for (int i = 0; i < size; i++)	
		if (array1[i] != array2[i])					
			return false;		
			
	return true;
}

void IdleScreenView::UnlockClicked(const Unlock & u)
{		
	//Open App Normally if clicked
	gotoMainScreen();
}

void IdleScreenView::UnlockSwiped(const Unlock & u)
{
	//Don't allow backlight pulsing, it if was pulsing bright it back up to max brightness
	allowPulse = false;
	presenter->SetBacklight(100);

	//Open the swipe password for the engineering window (or the secret)	
	patternModalWindow.setVisible(true);
	patternModalWindow.invalidate();
}
