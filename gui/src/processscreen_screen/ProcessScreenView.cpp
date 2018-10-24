#include <gui/processscreen_screen/ProcessScreenView.hpp>

void ProcessScreenView::setupScreen()
{	
	//Stop screensaver
	presenter->SetScreensaverState(false);

	//Word Wrap on Modal Windows
	TxtAbortMessage.setWideTextAction(WIDE_TEXT_WORDWRAP);
	TxtStopFumingMessage.setWideTextAction(WIDE_TEXT_WORDWRAP);	

	//Setup Lid Control
	lvrLidControl.setXY(225, 175);	
	lvrLidControl.SetPositionChangedEvent(LidControlValueChangedCallback);

	//Setup Progress bar
	progressBar.setXY(100, 375);
	progressBar.setWidth(600);
	progressBar.setHeight(40);
	progressBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255, 255, 255));
	progressBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	progressBar.SetTextVisible(true);
	progressBar.SetMaximum(600);	

	//Setup Error Box
	ErrorWindow.setXY(0, 0);
	ErrorWindow.setVisible(false);	

	//Setup Animation Defaults
	Animation.SetAnimationXY(63, 9);
	Animation.SetUpdateTicksInterval(3);
	Animation.SetImages(Bitmap(BITMAP_ANIMATIONBACKGROUND_ID));

	//Update to be correct stage
	stage = 0;
	UpdateStage();		
	updateLid();

	if (application().isDemoModeOn)
	{
		pointerImage.setBitmap(Bitmap(BITMAP_FINGER_VERTICAL_SWIPE_ID));
		pointerImage.setAlpha(0);
		add(pointerImage);

		tapImage.setBitmap(Bitmap(BITMAP_FINGER_TAP_ID));
		tapImage.setAlpha(0);
		add(tapImage);

		txtDemoMode.setColor(Color::getColorFrom24BitRGB(255, 255, 255));
		txtDemoMode.setTypedText(TypedText(T_DEMOMODE));
		txtDemoMode.setHeight(50);
		txtDemoMode.setWidth(200);
		txtDemoMode.setXY(500, 20);
		add(txtDemoMode);
	}	
	
	add(lvrLidControl);
	add(progressBar);
	add(ErrorWindow);

	//Show the Debug Values Depending on if engineering mode is on
	//Must be last so it's above all other controls
	if (application().isEngineerModeOn)
	{
		lftDebug.setXY(0, 70);
		lftDebug.setWidth(240);
		lftDebug.setHeight(410);
		lftDebug.SetTextEnabledColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
		add(lftDebug);
	}

	_isSetupFinished = true;
}

void ProcessScreenView::tearDownScreen()
{	
	//Allow screen saver
	presenter->SetScreensaverState(true);
}

void ProcessScreenView::handleTickEvent()
{
	//Used for progress bars and auto-progressing whilst we don't have actual values to use
	tickCounter++;

	//Animate Progress bar if it's shown
	if (progressBar.isVisible())
		progressBar.handleTickEvent();

	//Animate Text if it's a waiting stage
	if (stage == LFT_STAGE_INITIALISING || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_PURGING || stage == LFT_STAGE_FUMING)
		pulseStageText();

	//If demo mode is on, do the demo mode actions and only pretend to do the actions
	if (application().isDemoModeOn)
	{
		DemoProcess();
		return;
	}
	
	//Check for any Errors and Update on screen values if engineer mode is on
	if (tickCounter % 60 == 0)
		checkLFTValues();						

	//Stage Specific Actions
	switch (stage)
	{
	case LFT_STAGE_LID_CONTROL:	
	{
		//Animate Lid Control
		lvrLidControl.handleTickEvent();			

		//Check + Update Start button for if it should be enabled
		updateLid();
		break;
	}	
	case LFT_STAGE_INITIALISING:
	case LFT_STAGE_CHAMBER_CONDITIONING:
		//Update UI to match LFT State
#ifdef REAL
		updateToProgress();
#else
		progressBar.SetValue(tickCounter);		
#endif				
		break;			
	case LFT_STAGE_PURGING:
		//Update UI to match LFT State
		progressBar.SetValue(tickCounter);							
		break;	
	default:
		break;
	}
#ifndef REAL
	if (tickCounter != 0 && tickCounter % DUMMY_WAIT_TIME == 0)
	{				
		//Only progress if it's a stage that auto progresses

		if (stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_PURGING || stage == LFT_STAGE_INITIALISING)
		{
			tickCounter = 0;
			NextStage();
		}
	}
#else
	if (tickCounter != 0 && tickCounter % DUMMY_WAIT_TIME == 0)
	{
		//Only progress if it's a stage that auto progresses

		if (stage == LFT_STAGE_PURGING)
		{
			tickCounter = 0;
			NextStage();
		}
	}
#endif
}

void ProcessScreenView::LidControlValueChanged(const Lever & u)
{	
	if (application().isDemoModeOn)
		return;

	switch (lvrLidControl.GetPosition())
	{
	case -1:
		presenter->LidDown();
		break;
	case 0:
		presenter->LidStop();
		break;
	case 1:
		presenter->LidUp();
		break;
	}
}

void ProcessScreenView::pulseStageText()
{
	stageTextCounter++;
	// (sin(x / 25)
	// ------------ * 100 + 205 = y
	//       2
	float level = ((sin(stageTextCounter / 25) / 2) * stageTextDeviation) + (stageTextMaximum - (stageTextDeviation / 2));
	
	TxtStageInformation.setAlpha(level);
	TxtStageInformation.invalidate();
}

void ProcessScreenView::updateLid()
{			
	bool currentState = presenter->GetLidClosedState() || (application().isDemoModeOn && _isDemoModeLidClosed);

	//Return if the state hasn't changed and it's not setting up
	if (currentState == _guiLidState && _isSetupFinished)
		return;
	
	//Only allow user to start process if lid state is closed
	if (currentState)
	{					
		BtnStartProcess.setTouchable(true);
		BtnStartProcess.setBitmaps(Bitmap(BITMAP_START_ID), Bitmap(BITMAP_START_ID));
		BtnStartProcess.invalidate();				
	}	
	else
	{
		BtnStartProcess.setTouchable(false);
		BtnStartProcess.setBitmaps(Bitmap(BITMAP_START_DISABLED_ID), Bitmap(BITMAP_START_DISABLED_ID));
		BtnStartProcess.invalidate();
	}	

	_guiLidState = currentState;
}

void ProcessScreenView::checkLFTValues()
{
	int status = presenter->GetStatus();	

	//If there is an error has happened, but the processing screens are still showing, take it to the initialise screen
	if (status != 0 && (stage != LFT_STAGE_LID_CONTROL || stage != LFT_STAGE_FINISHED))
	{	
		//Return to lid control stage
		stage = LFT_STAGE_LID_CONTROL;
		UpdateStage();
	}

	if (status != errorNumber)
	{	
		if (status == 0)		
			errorNumber = 0;		
		else if (!ErrorWindow.isVisible())
		{						
			if (errorNumber < status)
				errorNumber++;
			else
				errorNumber = status;

			showError(Utils::TranslateError(presenter->GetError(errorNumber)));
		}
	}	

	//If engineer mode is on, show the values on the screen
	if (application().isEngineerModeOn)
	{		
		presenter->UpdateLFTDebug(&lftDebug);
		lftDebug.invalidate();
	}

	//Get the values from the presenter
	/* COMMENTED OUT DUE TO NOT USED ERRORS
	int progress = presenter->GetProgress();
	int baseTemp = presenter->ReadBaseTemperature();
	int preTemp = presenter->ReadPreTemperature();
	int pressure = presenter->ReadPressure();*/

	//TODO: Do something with these values	
}

void ProcessScreenView::updateToProgress()
{
	int progress = presenter->GetProgress();

	//If read failed, don't update bar
	if (progress == -1)
		return;	

	progressBar.SetValue(progress);

	//If 100 percent is reached, an error window isn't open and there's no more errors to show, go to the next screen.
	if (progress == 100 && !ErrorWindow.isVisible() && errorNumber == presenter->GetStatus())
	{
		presenter->QueRefreshValues();
		NextStage();
	}
}

void ProcessScreenView::showError(TEXTS errorMessage)
{		
	ErrorWindow.SetText(TypedText(errorMessage));
	ErrorWindow.setVisible(true);
	ErrorWindow.invalidate();
}

void ProcessScreenView::NextStage()
{
	stage++;
	if (stage >= STAGE_COUNT)
		stage = 0;

	if (application().isDemoModeOn)
		demoCounter = 0;
	
	tickCounter = 0;

	UpdateStage();
}

void ProcessScreenView::UpdateStage()
{		
	//Show/Hide Buttons
	BtnAbort.setVisible(stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_READY_TO_FUME || stage == LFT_STAGE_INITIALISING);		
	BtnBack.setVisible(stage == LFT_STAGE_LID_CONTROL);
	BtnHome.setVisible(stage == LFT_STAGE_FINISHED);
	ImgFinished.setVisible(stage == LFT_STAGE_FINISHED);
	BtnStopFuming.setVisible(stage == LFT_STAGE_FUMING);
	BtnStartFuming.setVisible(stage == LFT_STAGE_READY_TO_FUME);
	BtnStartProcess.setVisible(stage == LFT_STAGE_LID_CONTROL);
	CtrlChamberSelection.setVisible(stage == LFT_STAGE_LID_CONTROL);
	lvrLidControl.setVisible(stage == LFT_STAGE_LID_CONTROL);	
	progressBar.setVisible(stage == LFT_STAGE_INITIALISING || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_PURGING);
	progressBar.SetValue(0);	

	//Stop Animation and hide depending on stage	
	Animation.StopAnimation();
	Animation.setVisible(stage == LFT_STAGE_INITIALISING || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_FUMING || stage == LFT_STAGE_PURGING);
	
	//Set Progress bar to default maximum
#ifdef REAL
	if (application().isDemoModeOn || (stage != LFT_STAGE_INITIALISING && stage != LFT_STAGE_CHAMBER_CONDITIONING))
		progressBar.SetMaximum(DUMMY_WAIT_TIME);
	else
		progressBar.SetMaximum(100);
#else
	progressBar.SetMaximum(DUMMY_WAIT_TIME);
#endif

	//Update Text Alpha
	TxtStageInformation.setAlpha(255);
	TxtStageInformation.invalidate();
	
	//Show correct stage information 
	//Start correct animation
	//Send command to device
	switch (stage)
	{
		case LFT_STAGE_LID_CONTROL:					
			presenter->ClearErrors();
			errorNumber = 0;			
			presenter->SetLidClosedState(false);
			_isDemoModeLidClosed = false;
			updateLid();		
			TxtStageInformation.setTypedText(TypedText(T_LIDCONTROL));
			break;
		case LFT_STAGE_INITIALISING:
			TxtStageInformation.setTypedText(TypedText(T_INITIALISING));			
			Animation.SetImages(BITMAP_INITIALISE0000_ID, BITMAP_INITIALISE0020_ID);
			Animation.StartAnimation(false, true, true);		
			if (!application().isDemoModeOn)
			{
				presenter->SetBaseTemperatureSetting();
				presenter->SetPreTemperatureSetting();
				presenter->QuePrechecks();
			}
			break;
		case LFT_STAGE_CHAMBER_CONDITIONING:
			TxtStageInformation.setTypedText(TypedText(T_CHAMBERCONDITIONING));			
			Animation.SetImages(BITMAP_PUMP0000_ID, BITMAP_PUMP0020_ID);
			Animation.StartAnimation(false, true, true);
			if (!application().isDemoModeOn)
				presenter->QuePumpDown();
			break;
		case LFT_STAGE_READY_TO_FUME:
			TxtStageInformation.setTypedText(TypedText(T_READYTOFUME));
			break;
		case LFT_STAGE_FUMING:
			TxtStageInformation.setTypedText(TypedText(T_FUMINGINPROGRESS));			
			Animation.SetImages(BITMAP_FUME0000_ID, BITMAP_FUME0103_ID);
			Animation.StartAnimation(false, true, true);
			if (!application().isDemoModeOn)
				presenter->QueHeat();
			break;
		case LFT_STAGE_PURGING:
			TxtStageInformation.setTypedText(TypedText(T_PURGINGCHAMBER));			
			Animation.SetImages(BITMAP_PUMP0000_ID, BITMAP_PUMP0020_ID);
			Animation.StartAnimation(true, true, true);
			if (!application().isDemoModeOn)
				presenter->QueCool();
			break;
		case LFT_STAGE_FINISHED:
			TxtStageInformation.setTypedText(TypedText(T_REMOVEEVIDENCE));			
			break;
	}

	//Invalidate controls so that they update
	Backdrop.invalidate();
	TxtStageInformation.invalidate();	
}

void ProcessScreenView::ConfirmStopFuming()
{
	//Make sure box is in correct place
	StopFumingWindow.setXY(400 - (StopFumingWindow.getWidth() / 2), 240 - (AbortWindow.getHeight() / 2));
	StopFumingWindow.setVisible(true);
	StopFumingWindow.invalidate();
}

void ProcessScreenView::ConfirmAbort()
{
	//Make sure box is in correct place
	AbortWindow.setXY(400 - (AbortWindow.getWidth() / 2), 240 - (AbortWindow.getHeight() / 2));
AbortWindow.setVisible(true);
AbortWindow.invalidate();
}

void ProcessScreenView::DemoProcess()
{
	demoCounter++;

	//Animations
	switch (stage)
	{
	case LFT_STAGE_LID_CONTROL:
	{

		//Setup Posititons
		if (demoCounter == 1)
		{
			//Setup Pointer Image
			pointerImage.setXY(lvrLidControl.getX() + 50,
				lvrLidControl.getY() + (lvrLidControl.getHeight() / 2) - (pointerImage.getHeight() / 2));			

			//Setup tap Image
			int x = BtnStartProcess.getX() + (BtnStartProcess.getWidth() / 2) - (tapImage.getWidth() / 2);
			int y = BtnStartProcess.getY() + BtnStartProcess.getHeight() - 50;
			tapImage.setXY(x, y);			
			lvrLidControl.SetYPos((lvrLidControl.getHeight() / 2) - 25);			
		}
		//Fade In Finger
		else if (demoCounter < 60)
		{
			float percentage = (float)demoCounter / 60.0f;
			//touchgfx_printf("percentage = %f\n", percentage);
			pointerImage.setAlpha(percentage * 255);
			pointerImage.invalidate();
		}
		//Make sure finger is on max, disable lever animation
		else if (demoCounter == 60)
		{
			pointerImage.setAlpha(255);
			pointerImage.invalidate();
			lvrLidControl.SetAnimationAllowed(false);
		}
		//Finger Down
		else if (demoCounter < 120)
		{
			int minimumY = lvrLidControl.getY() + (lvrLidControl.getHeight() / 2) - (pointerImage.getHeight() / 2);
			int maximumY = lvrLidControl.getY() + lvrLidControl.getHeight() - (pointerImage.getHeight() / 2) - 25;
			int distance = maximumY - minimumY;
			float percentage = (float)(demoCounter - 60) / 60;
			pointerImage.setY(minimumY + (percentage * distance));
			lvrLidControl.SetYPos((lvrLidControl.getHeight() / 2) + (percentage * distance) - 25);
			pointerImage.invalidate();
		}
		else if (demoCounter < 420) {}
		//Finger Up
		else if (demoCounter == 479)		
			_isDemoModeLidClosed = true;		
		else if (demoCounter < 480)
		{
			int minimumY = lvrLidControl.getY() + (lvrLidControl.getHeight() / 2) - (pointerImage.getHeight() / 2);
			int maximumY = lvrLidControl.getY() + lvrLidControl.getHeight() - (pointerImage.getHeight() / 2) - 25;
			int distance = maximumY - minimumY;
			float percentage = (float)(demoCounter - 420) / 60;
			pointerImage.setY(maximumY - (percentage * distance));
			lvrLidControl.SetYPos(lvrLidControl.getHeight() - (percentage * distance) - 50);
			pointerImage.invalidate();
		}
		//Allow Animation again
		else if (demoCounter == 540)
		{
			lvrLidControl.SetAnimationAllowed(true);
		}
		//Fade Finger Out
		else if (demoCounter < 540)
		{
			float percentage = (float)(demoCounter - 480) / 60.0f;
			//touchgfx_printf("percentage = %f\n", percentage);
			pointerImage.setAlpha(255 - (percentage * 255));
			pointerImage.invalidate();
		}
		//Fade Tap Finger in
		else if (demoCounter < 600)
		{
			float percentage = (float)(demoCounter - 540) / 60.0f;
			tapImage.setAlpha(percentage * 255);
			tapImage.invalidate();
		}
		//Move to next
		else if (demoCounter == 660)
		{
			pointerImage.setAlpha(0);
			tapImage.setAlpha(0);
			pointerImage.invalidate();
			tapImage.invalidate();

			stage++;
			tickCounter = 1;
			demoCounter = 0;
			UpdateStage();
		}
		break;
	}
	case LFT_STAGE_READY_TO_FUME:
	{
		//Position Image
		if (demoCounter == 1)
		{
			int x = BtnStartFuming.getX() + (BtnStartFuming.getWidth() / 2) - (tapImage.getWidth() / 2);
			int y = BtnStartFuming.getY() + BtnStartFuming.getHeight() - 60;
			tapImage.setXY(x, y);
		}
		else if (demoCounter < 120) {}
		//Fade Image in
		else if (demoCounter < 180)
		{
			float percentage = (float)(demoCounter - 120) / 60.0f;
			//touchgfx_printf("percentage = %f\n", percentage);
			tapImage.setAlpha(percentage * 255);
			tapImage.invalidate();
		}		
		//Goto next stage
		else if (demoCounter == 240)
		{
			tapImage.setAlpha(0);
			tapImage.invalidate();

			stage = LFT_STAGE_FUMING;
			tickCounter = 1;
			demoCounter = 0;
			UpdateStage();
		}

		break;
	}
	case LFT_STAGE_FUMING:
	{
		//Setup Image Position
		if (demoCounter == 1) {
			int x = BtnStopFuming.getX() + (BtnStopFuming.getWidth() / 2) - (tapImage.getWidth() / 2) - 55;
			int y = BtnStopFuming.getY() + BtnStopFuming.getHeight() - 45;
			tapImage.setXY(x, y);
		}
		else if (demoCounter < 180) {}
		//Fade in image
		else if (demoCounter < 240)
		{
			float percentage = (float)(demoCounter - 180) / 60.0f;			
			tapImage.setAlpha(percentage * 255);
			tapImage.invalidate();
			
		}		
		//Goto confirm window
		else if (demoCounter == 300)
		{
			int x = 406;
			int y = 275;
			//int x = StopFumingWindow.getX() + BtnStopFumingYes.getX() + (BtnStopFumingYes.getWidth() / 2) - (tapImage.getWidth() / 2) - 30;
			//int y = StopFumingWindow.getY() + BtnStopFumingYes.getY() + BtnStopFumingYes.getHeight() - 50;
			tapImage.setAlpha(0);
			tapImage.setXY(x, y);
			tapImage.invalidate();


			ConfirmStopFuming();			
			tickCounter = 1;			
			
		}
		else if (demoCounter < 360) {}
		//Fade in image
		else if (demoCounter < 420)
		{
			float percentage = (float)(demoCounter - 360) / 60.0f;
			tapImage.setAlpha(percentage * 255);
			tapImage.invalidate();
			
		}
		//Goto next screen
		else if (demoCounter == 480)
		{							
			tapImage.setAlpha(0);
			AbortProcess();
			demoCounter = 0;
		}

		break;
	}
	case LFT_STAGE_FINISHED:
	{
		//Wait a while then reset
		if (demoCounter == 600)
		{			
			updateLid();

			stage = LFT_STAGE_LID_CONTROL;
			tickCounter = 1;
			demoCounter = 0;
			UpdateStage();			
		}
		break;
			
	}
	}


	//Stage Specific Actions
	switch (stage)
	{
	case LFT_STAGE_LID_CONTROL:
	{
		//Update the percentage bar with fake lid position		
		lvrLidControl.handleTickEvent();		
		updateLid();
		break;
	}	
	case LFT_STAGE_INITIALISING:
		progressBar.SetValue(tickCounter);
		pulseStageText();
		break;
	case LFT_STAGE_CHAMBER_CONDITIONING:
		//Update Percentage bar, pulse the text
		progressBar.SetValue(tickCounter);
		pulseStageText();
		break;
	case LFT_STAGE_PURGING:
		//Update Percentage bar, pulse the text
		progressBar.SetValue(tickCounter);
		pulseStageText();
		break;
	case LFT_STAGE_FUMING:
		//Update Percentage bar, pulse the text
		pulseStageText();
		break;
	default:
		break;
	}

	if (tickCounter != 0 && tickCounter % DUMMY_WAIT_TIME == 0)
	{
		//Only progress if it's a stage that auto progresses
		if (stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_PURGING || stage == LFT_STAGE_INITIALISING)
		{
			tickCounter = 0;
			NextStage();
		}
	}
}

void ProcessScreenView::AbortProcess()
{	
	//Hide Abort Confirmation
	AbortWindow.setVisible(false);
	AbortWindow.invalidate();

	//Hide Stop fuming confirm window
	StopFumingWindow.setVisible(false);
	StopFumingWindow.invalidate();

	//Reset the tick counter
	tickCounter = 0;

	if (!application().isDemoModeOn)
		presenter->Abort();

	//Error Code from process being aborted will return stage to Lid Control
	
#ifndef REAL
	//Set Stage to purging or lid control depending on where it was.
	if (stage == LFT_STAGE_FUMING)
		stage = LFT_STAGE_PURGING;
	else	
		stage = LFT_STAGE_LID_CONTROL;
	UpdateStage();
#endif
}

