#include <gui/processscreen_screen/ProcessScreenView.hpp>

void ProcessScreenView::setupScreen()
{	
	//Stop screensaver
	presenter->SetScreensaverState(false);	

	//Word Wrap
	TxtAbortMessage.setWideTextAction(WIDE_TEXT_WORDWRAP);
	TxtStopFumingMessage.setWideTextAction(WIDE_TEXT_WORDWRAP);	
	TxtConfirmFumehood.setWideTextAction(WIDE_TEXT_WORDWRAP);

	//Setup Lid Control
	lvrLidControl.setXY(LVR_POS_START_X, LVR_POS_START_Y);	
	lvrLidControl.SetPositionChangedEvent(LidControlValueChangedCallback);

	//Setup Progress bar
	progressBar.setXY(100, 375);
	progressBar.setWidth(600);
	progressBar.setHeight(40);
	progressBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255, 255, 255));
	progressBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	progressBar.SetTextVisible(true);
	progressBar.SetMaximum(600);	

	HintInstructions.setXY(0, 0);
	HintInstructions.setVisible(false);
	
	AbortInProcessWindow.setXY(0, 0);
	AbortInProcessWindow.setVisible(false);	

	InternalSwitchErrorWindow.setXY(0, 0);
	InternalSwitchErrorWindow.setVisible(false);

	ExtSwitchErrorWindow.setXY(0, 0);
	ExtSwitchErrorWindow.setVisible(false);

	TxtConfirmFumehood.setVisible(false);

	BtnExternal.setVisible(false);
	BtnSystemUnderperforming.setVisible(false);

	//Setup Error Box
	ErrorWindow.setXY(0, 0);
	ErrorWindow.setVisible(false);	

	//Setup Filter Window Box
	FilterStateMessageWindow.setXY(0, 0);
	FilterStateMessageWindow.setVisible(false);

	//Setup moisture Window
	MoistureMessage.setXY(0, 0);
	MoistureMessage.setVisible(false);

	//Setup Powerloss Window
	PowerLossWindow.setXY(0, 0);
	PowerLossWindow.setVisible(false);
	PowerLossWindow.SetMessage(T_POWERLOSSDETECTED);

	//Setup Log Overwrite messagebox
	ConfirmLogOverwrite.setXY(0, 0);
	ConfirmLogOverwrite.setVisible(false);
	ConfirmLogOverwrite.SetText(T_CONFIRMLOGOVERWRITE);
	ConfirmLogOverwrite.SetReturnedEvent(LogOverwriteMsgBoxReturnedCallback);

	//Setup Animation Defaults
	Animation.SetAnimationXY(63, 9);
	Animation.SetUpdateTicksInterval(3);
	Animation.SetImages(Bitmap(BITMAP_ANIMATIONBACKGROUND_ID));
	
	//Setup user cipher
	TxtUserCipher.setVisible(LFT::Information.UserCipherMode);

	//Setup Selection	
	lidSubStage = 0;	

	//If the stage is finished, reset the process
	if (LFT::Auto.GetStage() == LFT_STAGE_FINISHED)
		LFT::Auto.SetStage(LFT_STAGE_LID_CONTROL);

	//Update to be current stage	
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
		txtDemoMode.setXY(520, 10);
		txtDemoMode.setWidth(125);
		txtDemoMode.setHeight(100);
		add(txtDemoMode);
	}	
		
	add(lvrLidControl);
	add(progressBar);	
	add(HintInstructions);
	add(ConfirmLogOverwrite);
	add(FilterStateMessageWindow);
	add(MoistureMessage);
	add(PowerLossWindow);
	add(ExtSwitchErrorWindow);
	add(InternalSwitchErrorWindow);
	add(AbortInProcessWindow);
	add(ErrorWindow);

	//Show the Debug Values Depending on if engineering mode is on
	//Must be last so it's above all other controls
	if (LFT::Information.EngineeringMode)
	{
		lftDebug.setXY(0, 70);
		lftDebug.setWidth(240);
		lftDebug.setHeight(410);
		lftDebug.SetTextEnabledColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
		add(lftDebug);
	}		

	_isSetupFinished = true;
	
	int extState = LFT::Information.ExternalSwitchValue;
	UpdateExternalSwitch(extState);
	_shownExtValue = extState;

	if (LFT::Information.PowerlossDetected)
	{
		LFT::Information.PowerlossDetected = false;
		ShowPowerLossDetected();
	}
	
}

void ProcessScreenView::tearDownScreen()
{	
	//Allow screen saver
	presenter->SetScreensaverState(true);
}

void ProcessScreenView::handleTickEvent()
{	
	//Ticker
	tickCounter++;		

	int stage = LFT::Auto.GetStage();				

	//Animate Progress bar if it's shown and not paused
	if (progressBar.isVisible() && !_isDemoModePaused)
		progressBar.handleTickEvent();		

	//If demo mode is on, do the demo mode actions and only pretend to do the actions
	if (application().isDemoModeOn)
	{
		stage = demoStage;
		DemoProcess();
		return;
	}
	
#ifdef SIMULATOR
	//In simulator, show abort window for a little while then finish
	if (AbortInProcessWindow.isVisible())
	{
		simAbortCounter++;

		if (simAbortCounter > 500)
		{
			simAbortCounter = 0;

			AbortInProcessWindow.setVisible(false);
			AbortInProcessWindow.invalidate();

			LFT::Auto.SetStage(LFT_STAGE_FINISHED);
			LFT::AutoClean.SetStage(AUTOCLEAN_STAGE_FINISHED);
			UpdateStage();
		}
	}
#else
	//If Aborting Window is open, and process has finished aborting, close the window
	if (AbortInProcessWindow.isVisible() && !LFT::Information.AbortRequired)
	{
		AbortInProcessWindow.setVisible(false);
		AbortInProcessWindow.invalidate();
		UpdateStage();
	}
#endif

	
	if (tickCounter % 60)
		checkLFTValues();
				
	//Stage Specific Actions
	switch (stage)
	{
	case LFT_STAGE_LID_CONTROL:	
	case LFT_STAGE_FINISHED:		
	{
		//Animate Lid Control
		lvrLidControl.handleTickEvent();			

		//Check + Update Start button for if it should be enabled
		if (stage == LFT_STAGE_LID_CONTROL)
		updateLid();
		break;
	}	
	case LFT_STAGE_PRECHECKS:
	case LFT_STAGE_CHAMBER_CONDITIONING:
	case LFT_STAGE_COOLDOWN:						
	case LFT_STAGE_FUMING:
	case LFT_STAGE_TUNING:
		//Animate Text if it's a waiting stage		
		pulseStageText();
		break;			
	default:
		break;	
	}	

#ifdef SIMULATOR
	//Simulator, goto next stage on count, normally uses value from LFT
	if (tickCounter != 0 && tickCounter % DUMMY_WAIT_TIME == 0)
	{				
		//Only progress if it's a stage that auto progresses
		if (stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_COOLDOWN || stage == LFT_STAGE_PRECHECKS)
		{			
			tickCounter = 0;
			NextStage();		
		}
		if (stage == LFT_STAGE_TUNING)
		{			
			LFT::AutoClean.SetStage(AUTOCLEAN_STAGE_FINISHED);
			UpdateStage();
		}
	}	
#endif

	//Reset Counter when it's opportune
	if (tickCounter == DUMMY_WAIT_TIME)
		tickCounter = 0;


	if (_requiresInvalidate)
	{
		lftDebug.invalidate();
		progressBar.invalidate();
	}
}

void ProcessScreenView::ShowInstructions()
{		
	HintInstructions.SetInstructions(instructions, INSTRUCTION_LENGTH);
	HintInstructions.setVisible(true);
	HintInstructions.invalidate();		
}

void ProcessScreenView::LidControlValueChanged(const Lever & u)
{		
	if (application().isDemoModeOn)
		return;

	switch (lvrLidControl.GetPosition())
	{
	case -1:
		LFT::Manual.LidDown();		
		break;
	case 0:
		LFT::Manual.LidStop();		
		break;
	case 1:
		LFT::Manual.LidUp();		
		break;
	}
}

void ProcessScreenView::LogOverwriteMsgBoxReturned(const MsgBox & u, bool state)
{
	ConfirmLogOverwrite.setVisible(false);
	ConfirmLogOverwrite.invalidate();

	if (state)
		StartProcess(true);
}

void ProcessScreenView::pulseStageText()
{
	stageTextCounter++;
	// (cos(x / 25)
	// ------------ * 100 + 205 = y
	//       2
	float level = ((cos((float)stageTextCounter / 25) / 2) * stageTextDeviation) + (stageTextMaximum - (stageTextDeviation / 2));		

	//Reset when counter at 50Pi
	if (stageTextCounter % 157 == 0)
		stageTextCounter = 0;


	TxtStageInformation.setAlpha((uint8_t)level);
	TxtStageInformation.invalidate();
}

void ProcessScreenView::updateLid()
{				
	bool currentState = LFT::Information.LidClosedState || (application().isDemoModeOn && _isDemoModeLidClosed);

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
	int status = LFT::Information.Status;
	int stage = LFT::Auto.GetStage();
	

	//Uncomment this to show fake errors (helpful for debug and screenshots)
	//status = 1;


	//If Status > 0 then an error has occured.
	//Status up to date is false if an error has been deleted since the status was last polled.	
	if (status > 0 && LFT::Information.StatusUpToDate)
	{
		_isUpdateCipherAllowed = false;

		//Read All Errors		
		ErrorMessage errorBuffer[32];
		for (int i = 0; i <= status; i++)
			errorBuffer[i] = ErrorMessage(LFT::Information.ReadError(i + 1));

		//Assign fake errors here
		//errorBuffer[0] = ErrorMessage(04);		

		//Clear all errors
		_processSuccess = false;
		LFT::Auto.ClearErrors();

		//Show the error(s)
		showError(errorBuffer, status);

		//Start Aborting
		if (stage != LFT_STAGE_LID_CONTROL && stage != LFT_STAGE_FINISHED)
			AbortProcess();
	}

	//If engineer mode is on, show the values on the screen
	if (LFT::Information.EngineeringMode)
	{
		LFT::Information.UpdateLFTDebug(&lftDebug);
		_requiresInvalidate = true;	
	}

	//If User Cipher mode is on then update the values
	if (_isUpdateCipherAllowed)
	{				
		_cipher = Cipher().GetCipher(LFT::Information.Pressure, LFT::Information.BaseTemp, LFT::Information.PreTemp);

		if (LFT::Information.UserCipherMode)
		{
			Unicode::snprintf(TxtUserCipherBuffer, TXTUSERCIPHER_SIZE, "%X", _cipher);
			TxtUserCipher.invalidate();
		}
	}


	//If stage does not equal shown stage, update
	if (stage != shownStage && !application().isDemoModeOn)
		UpdateStage();

	//Check External Switch State	
	int extState = LFT::Information.ExternalSwitchValue;
	if (extState != _shownExtValue && extState >= 0)
	{
		UpdateExternalSwitch(extState);
		_shownExtValue = extState;
	}

	//Check Internal Switch
	int internalSwitch2 = LFT::Information.InternalSwitchValue2;
	if (stage == LFT_STAGE_FUMING && !InternalSwitchErrorWindow.isVisible())
	{
		if (internalSwitch2 > 0)
		{
			InternalSwitchErrorWindow.SetState(true);
			InternalSwitchErrorWindow.setVisible(true);
			InternalSwitchErrorWindow.invalidate();
		}
	}

	//System underperforming
	if (stage == LFT_STAGE_CHAMBER_CONDITIONING && LFT::Information.Pressure < 10.0f)
	{
		LFT::Information.CheckPerformance();
		bool performance = LFT::Information.Performance();
		if (!performance && !BtnSystemUnderperforming.isVisible())
		{
			BtnSystemUnderperforming.setVisible(true);
			BtnSystemUnderperforming.invalidate();
			RepositionErrors();


			//TODO: Show message about how it's underperforming and give user option to dry
		}
	}


	/* DEPRECIATED!
	* This is kept here incase it needs to come back in the future. The error code is also kept but should never be shown
	*
	* This will trigger when the delta (mTorr of pressure reduced per 10 seconds) is 0, it will then do different things
	* depending on where the pressure is currently.
	*
	* Science department didn't like this as they saw it as too inacurate and believed that certain scientific bodies 
	* would have issues with the wording used. This is replaced by the timeout allowing progress if under 0.9Torr.

	//Check Delta Level in pumpdown (only works below 15Torr)
	int pressure = (int)LFT::Information.Pressure;
	if (LFT::Information.Delta >= 0 && LFT::Information.Delta < LFT::Information.DELTA_MIN)
	{
		if (stage == LFT_STAGE_CHAMBER_CONDITIONING && pressure < 15.0f)
		{			
			if (pressure < LFT::Information.DELTA_STOPPED_ACCEPTABLE_MAX) //If pressure is in warning area
			{
				LowPressureWindow.setVisible(true);
				BtnLowPressure.setVisible(true);
				BtnLowPressure.invalidate();
				RepositionErrors();
				LFT::Auto.QueStage(LFT_STAGE_READY_TO_FUME);
			}
			else //If pressure is not good enough
			{
				InternalSwitchErrorWindow.SetState(false);
				InternalSwitchErrorWindow.setVisible(true);
				InternalSwitchErrorWindow.invalidate();
			}

		}
	}*/

	//Update Fuming Time
	if (stage == LFT_STAGE_FUMING)
	{
		if (((DateTime)LFT::Information.FumingStartTime).isValid())
		{
			DateTime current = (DateTime)LFT::Information.GetCurrentTime();
			DateTime startTime = (DateTime)LFT::Information.FumingStartTime;

			STime time(current.getRaw() - startTime.getRaw());
			int minutes = time.GetTotalMinutes();
			if (current.getRaw() == 0 || startTime.getRaw() == 0)
				minutes = 0;

#ifdef SIMULATOR
			minutes = 13;
#endif		

			Unicode::snprintf(TxtFumeTimerBuffer, TXTFUMETIMER_SIZE, "%d", minutes);
			TxtFumeTimer.invalidate();
	    }		
	}
	if (stage == LFT_STAGE_CHAMBER_CONDITIONING)
	{
		if (((DateTime)LFT::Information.ConditioningStartTime).isValid())
		{
			DateTime current = (DateTime)LFT::Information.GetCurrentTime();
			DateTime startTime = (DateTime)LFT::Information.ConditioningStartTime;

			STime time(current.getRaw() - startTime.getRaw());
			int minutes = time.GetTotalMinutes();

			if (current.getRaw() == 0 || startTime.getRaw() == 0)
				minutes = 0;

#ifdef SIMULATOR
			minutes = 10;
#endif		

			Unicode::snprintf(TxtFumeTimerBuffer, TXTFUMETIMER_SIZE, "%d", minutes);
			TxtFumeTimer.invalidate();
		}
	}



	//Update Progress bar to match LFT value	
	if (stage == LFT_STAGE_PRECHECKS || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_COOLDOWN || stage == LFT_STAGE_TUNING)
		updateToProgress();
}

void ProcessScreenView::updateToProgress()
{		
	int progress = 0;

#ifdef REAL
	progress = LFT::Information.Progress;

	//If currently doing a tuning cycle
	if (LFT::AutoClean.GetState() != AUTOCLEAN_STAGE_NONE)
		progress = LFT::AutoClean.GetProgress();

	if (LFT::Auto.GetStage() == LFT_STAGE_CHAMBER_CONDITIONING && progress >= 10)
	{
		progress = ProgressStages().TranslatePressureToPercentage((double)LFT::Information.Pressure, previousChamberConditioningPercentage);
		previousChamberConditioningPercentage = progress;
	}
	
#else		
	progress = tickCounter;
	if (AbortInProcessWindow.isVisible())
		return;
#endif					
	//Happens when screen hasn't been updated to current stage
	if (progress < localProgress)
		return;

	//If read failed, don't update bar
	if (progress <= 1)
		return;		

	progressBar.SetValue(progress);					

	_requiresInvalidate = true;

}

void ProcessScreenView::checkDemoLidState(bool direction)
{
	if (_isDemoLidPaused != _isDemoModePaused)
	{
		if (_isDemoModePaused)
			LFT::Manual.LidStop();
		else if (direction)
			LFT::Manual.LidUp();
		else
			LFT::Manual.LidDown();

		_isDemoLidPaused = _isDemoModePaused;
	}
}

void ProcessScreenView::showError(ErrorMessage * errors, int count)
{		
	//If any of the errors is ignored, stop showing error
	for (int i = 0; i < count; i++)
		if (errors[i].isIgnored)
			return;


	ErrorWindow.SetMessages(errors, count);	
	ErrorWindow.setVisible(true);
	ErrorWindow.invalidate();
}

void ProcessScreenView::RepositionErrors()
{
	const int lowValue = 560;
	const int medValue = 620;	
	const int highValue = 680;

	int binVal = 0;
	if (BtnFilter.isVisible())
		binVal += 4;
	if (BtnExternal.isVisible())
		binVal += 2;
	if (BtnSystemUnderperforming.isVisible())
		binVal += 1;	

	//Organise soft errors
	switch (binVal)
	{
	case 1:
		BtnSystemUnderperforming.setX(highValue);
		break;
	case 2:
		BtnExternal.setX(highValue);
		break;
	case 3:
		BtnSystemUnderperforming.setX(medValue);
		BtnExternal.setX(highValue);
		break;
	case 4:
		BtnFilter.setX(highValue);
		break;
	case 5:
		BtnSystemUnderperforming.setX(medValue);
		BtnFilter.setX(highValue);
		break;
	case 6:
		BtnExternal.setX(medValue);
		BtnFilter.setX(highValue);
		break;
	case 7:
		BtnSystemUnderperforming.setX(lowValue);
		BtnExternal.setX(medValue);
		BtnFilter.setX(highValue);
		break;

	}

	BtnFilter.invalidate();
	BtnExternal.invalidate();	
	BtnSystemUnderperforming.invalidate();
}

void ProcessScreenView::UpdateExternalSwitch(int state)
{	
	BtnExternal.setVisible(state > 0);
	
	int stage = LFT::Auto.GetStage();
	if (stage == LFT_STAGE_READY_TO_FUME)
	{
		//Disable start button on ready to fume stage if switch is incorrect
		BtnStartFuming.setTouchable(state == 0);
		if (state == 0)		
			BtnStartFuming.setBitmaps(Bitmap(BITMAP_TRANSPARENT_250X250_ID), Bitmap(BITMAP_TRANSPARENT_250X250_ID), Bitmap(BITMAP_START_ID), Bitmap(BITMAP_START_ID));
		else					
			BtnStartFuming.setBitmaps(Bitmap(BITMAP_TRANSPARENT_250X250_ID), Bitmap(BITMAP_TRANSPARENT_250X250_ID), Bitmap(BITMAP_START_DISABLED_ID), Bitmap(BITMAP_START_DISABLED_ID));		

		BtnStartFuming.invalidate();
	}
	else if (stage == LFT_STAGE_FUMING || stage == LFT_STAGE_COOLDOWN)
	{
		//Show fuming error message
		ExtSwitchErrorWindow.setVisible(state > 0);
		ExtSwitchErrorWindow.AllowCancel(stage == LFT_STAGE_FUMING);
		ExtSwitchErrorWindow.invalidate();
	}
	else
	{
		ExtSwitchErrorWindow.setVisible(false);		
		ExtSwitchErrorWindow.invalidate();
	}

	//If external switch is an override instead of an actual fume hood, display message confirming fume hood is working							
	TxtConfirmFumehood.setVisible(LFT::Information.TachoValue == 0 && stage == LFT_STAGE_READY_TO_FUME);
	TxtConfirmFumehood.invalidate();

	
	BtnExternal.invalidate();
	RepositionErrors();
}

void ProcessScreenView::UpdateProcessSelected()
{	
	if (_chamberSelected)
	{
		BtnChamberType.setIconBitmaps(Bitmap(BITMAP_LARGECHAMBER_ID), Bitmap(BITMAP_LARGECHAMBER_ID));
		BtnChamberType.setIconXY(8, 20);
	}
	else
	{
		BtnChamberType.setIconBitmaps(Bitmap(BITMAP_SMALLCHAMBER_ID), Bitmap(BITMAP_SMALLCHAMBER_ID));
		BtnChamberType.setIconXY(8, 6);
	}

	if (_typeSelected)
		BtnMetalType.setIconBitmaps(Bitmap(BITMAP_SILVER_ID), Bitmap(BITMAP_SILVER_ID));
	else
		BtnMetalType.setIconBitmaps(Bitmap(BITMAP_COPPER_ID), Bitmap(BITMAP_COPPER_ID));


	TxtPrecursor.setTypedText(TypedText(Utils::GetDevelopRString(_chamberSelected, _typeSelected, false)));
	

	BtnChamberType.invalidate();
	BtnMetalType.invalidate();
	TxtPrecursor.invalidate();
}

void ProcessScreenView::NextSubStage()
{
	if (selectSingleStage)
		lidSubStage = LIDCONTROL_MAIN;
	else
		lidSubStage++;

	UpdateStage();
}

void ProcessScreenView::NextStage()
{
	if (application().isDemoModeOn)
	{
		demoStage++;
		if (demoStage >= STAGE_COUNT)
			demoStage = 0;
		demoCounter = 0;
	}
	else
	{
		LFT::Auto.SetStage(LFT::Auto.GetStage() + 1);
		if (LFT::Auto.GetStage() >= STAGE_COUNT)
			demoStage = 0;		
	}

	tickCounter = 0;
	
	UpdateStage();
}

void ProcessScreenView::UpdateStage()
{
	int stage = LFT::Auto.GetStage();	

	if (application().isDemoModeOn)
		stage = demoStage;

	//Show/Hide Buttons
	BtnAbort.setVisible(stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_READY_TO_FUME || stage == LFT_STAGE_PRECHECKS || stage == LFT_STAGE_TUNING);
	BtnBack.setVisible(stage == LFT_STAGE_LID_CONTROL);
	BtnHome.setVisible(stage == LFT_STAGE_FINISHED);
	BtnStopFuming.setVisible(stage == LFT_STAGE_FUMING);
	BtnStartFuming.setVisible(stage == LFT_STAGE_READY_TO_FUME);
	progressBar.setVisible(stage == LFT_STAGE_PRECHECKS || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_COOLDOWN || stage == LFT_STAGE_TUNING);
	BtnHint.setVisible(stage == LFT_STAGE_LID_CONTROL);
	TxtFumeTimer.setVisible(stage == LFT_STAGE_FUMING || stage == LFT_STAGE_CHAMBER_CONDITIONING);

	ImgFinished.setVisible(stage == LFT_STAGE_FINISHED);
	if (stage == LFT_STAGE_FINISHED)
	{
		if (_processSuccess)
			ImgFinished.setBitmap(Bitmap(BITMAP_COMPLETE_ID));
		else
			ImgFinished.setBitmap(Bitmap(BITMAP_FAILED_ID));

		ImgFinished.invalidate();
	}
	

	if (stage == LFT_STAGE_LID_CONTROL)
	{
		ChamberSelector.setVisible(lidSubStage == LIDCONTROL_SETCHAMBER);
		TypeSelector.setVisible(lidSubStage == LIDCONTROL_SELECTTYPE);
		

		txtFinalLidControl.setVisible(lidSubStage != LIDCONTROL_MAIN);
		lvrLidControl.setVisible(true);
		
		BtnStartProcess.setVisible(lidSubStage == LIDCONTROL_MAIN);
		PrecursorElement.setVisible(lidSubStage == LIDCONTROL_MAIN);

		switch (lidSubStage)
		{
		case LIDCONTROL_MAIN:
			TxtStageInformation.setTypedText(TypedText(T_LIDCONTROL));			
			lvrLidControl.setXY(LVR_POS_START_X, LVR_POS_START_Y);			
			UpdateProcessSelected();
			break;
		case LIDCONTROL_SETCHAMBER:
			TxtStageInformation.setTypedText(TypedText(T_SELECTCHAMBER));
			lvrLidControl.setXY(LVR_POS_START_R_X, LVR_POS_START_R_Y);			
			txtFinalLidControl.setY(LVR_POS_START_R_Y);
			break;
		case LIDCONTROL_SELECTTYPE:
			TxtStageInformation.setTypedText(TypedText(T_SELECTTYPE));
			lvrLidControl.setXY(LVR_POS_START_R_X, LVR_POS_START_R_Y);
			txtFinalLidControl.setY(LVR_POS_START_R_Y);
			break;
		}
	}
	else if (stage == LFT_STAGE_FINISHED)
	{
		//Update Ciphers
		int* ciphers = LFT::Settings.GetCiphers();
		for (int i = CIPHER_COUNT - 2; i >= 0; i--)
		{
			ciphers[i + 1] = ciphers[i];

			if (i == 0)
				ciphers[i] = _cipher;
		}
		LFT::Settings.SetCiphers(ciphers);


		//Reposition UI
		lvrLidControl.setXY(LVR_POS_END_X, LVR_POS_END_Y);
		txtFinalLidControl.setY(LVR_POS_END_Y);
		lvrLidControl.setVisible(true);
		txtFinalLidControl.setVisible(true);
	}
	else
	{
		txtFinalLidControl.setVisible(false);
		ChamberSelector.setVisible(false);
		TypeSelector.setVisible(false);
		lvrLidControl.setVisible(false);
		BtnStartProcess.setVisible(false);
		PrecursorElement.setVisible(false);
	}	

	progressBar.SetValue(0);

	//Hide Stop fuming confirm window
	StopFumingWindow.setVisible(false);
	StopFumingWindow.invalidate();

	//Stop Animation and hide depending on stage	
	Animation.StopAnimation();
	Animation.setVisible(stage == LFT_STAGE_PRECHECKS || stage == LFT_STAGE_CHAMBER_CONDITIONING || stage == LFT_STAGE_FUMING || stage == LFT_STAGE_COOLDOWN || stage == LFT_STAGE_TUNING);

	//Set Progress bar to default maximum

#ifdef REAL		
	if (application().isDemoModeOn)
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
		_isDemoModeLidClosed = false;
		updateLid();
		break;
	case LFT_STAGE_PRECHECKS:
		TxtStageInformation.setTypedText(TypedText(T_INITIALISING));
		Animation.SetImages(BITMAP_INITIALISE0000_ID, BITMAP_INITIALISE0019_ID);
		Animation.StartAnimation(false, true, true);
		break;
	case LFT_STAGE_CHAMBER_CONDITIONING:
		TxtStageInformation.setTypedText(TypedText(T_CHAMBERCONDITIONING));
		Animation.SetImages(BITMAP_PUMP0000_ID, BITMAP_PUMP0019_ID);
		Animation.StartAnimation(false, true, true);
		break;
	case LFT_STAGE_READY_TO_FUME:
		TxtStageInformation.setTypedText(TypedText(T_READYTOFUME));
		break;
	case LFT_STAGE_FUMING:
		TxtStageInformation.setTypedText(TypedText(T_FUMINGINPROGRESS));
		Animation.SetImages(BITMAP_FUME0000_ID, BITMAP_FUME0103_ID);
		Animation.StartAnimation(false, true, true);
		break;
	case LFT_STAGE_COOLDOWN:
		TxtStageInformation.setTypedText(TypedText(T_PURGINGCHAMBER));
		Animation.SetImages(BITMAP_PUMP0000_ID, BITMAP_PUMP0019_ID);
		Animation.StartAnimation(true, true, true);
		break;
	case LFT_STAGE_FINISHED:
		LFT::Information.LidClosedState = false;
		TxtStageInformation.setTypedText(TypedText(T_REMOVEEVIDENCE));
		break;
	case LFT_STAGE_TUNING:
		TxtStageInformation.setTypedText(TypedText(T_TUNING));
		Animation.SetImages(BITMAP_TUNING0000_ID, BITMAP_TUNING0019_ID);
		Animation.StartAnimation(false, true, true);

	}
	

	//Reset/Update Local variables
	shownStage = stage;
	localProgress = 0;

	//Update Filter Control
	switch (LFT::Information.GetFilterValue())
	{
	case 0:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_GOOD_ID), Bitmap(BITMAP_FILTER_GOOD_ID));
		break;
	case 1:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_MEDIUM_ID), Bitmap(BITMAP_FILTER_MEDIUM_ID));
		break;
	case 2:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_BAD_ID), Bitmap(BITMAP_FILTER_BAD_ID));
		break; 	

	}

	//Update External Switch error for this state
	UpdateExternalSwitch(LFT::Information.ExternalSwitchValue);

	//Reset elapsed time to 0
	Unicode::snprintf(TxtFumeTimerBuffer, TXTFUMETIMER_SIZE, "%d", 0);
	TxtFumeTimer.invalidate();

	//Invalidate controls so that they update
	Backdrop.invalidate();
	TxtStageInformation.invalidate();
}

void ProcessScreenView::ConfirmStopFuming()
{
	//Make sure box is in correct place
	StopFumingWindow.setXY(400 - (StopFumingWindow.getWidth() / 2), 240 - (StopFumingWindow.getHeight() / 2));
	StopFumingWindow.setVisible(true);
	StopFumingWindow.invalidate();
}

void ProcessScreenView::ConfirmAbort()
{
	//Make sure box is in correct place
	AbortConfirmWindow.setXY(400 - (AbortConfirmWindow.getWidth() / 2), 240 - (AbortConfirmWindow.getHeight() / 2));
	AbortConfirmWindow.setVisible(true);
	AbortConfirmWindow.invalidate();
}

void ProcessScreenView::DemoProcess()
{
	bool paused = application().isDemoModePaused;
	if (paused)
		tickCounter--;
	else
		demoCounter++;

	//Update Pause State
	if (_isDemoModePaused != paused)
	{
		if (paused)
			txtDemoMode.setTypedText(TypedText(T_DEMOMODEPAUSED));
		else
			txtDemoMode.setTypedText(TypedText(T_DEMOMODE));
		
		txtDemoMode.invalidate();
		_isDemoModePaused = paused;
	}



	//Animations
	switch (demoStage)
	{
	case LFT_STAGE_LID_CONTROL:
	{
		switch (lidSubStage)
		{
		case LIDCONTROL_SELECTTYPE:
			if (demoCounter == 1)
			{
				int x = BtnType1.getX() + (BtnType1.getWidth() / 2) - (tapImage.getWidth() / 2);
				int y = BtnType1.getY() + BtnType1.getHeight() + 10;
				
				tapImage.setXY(x, y);
								
			}
			else if (demoCounter < 60)
			{
			}
			else if (demoCounter < 120)
			{ 
				float percentage = (float)(demoCounter - 60) / 60.0f;
				tapImage.setAlpha((uint8_t)(percentage * 255));
				tapImage.invalidate();
			}
			else if (demoCounter > 200)
			{
				lidSubStage = LIDCONTROL_SETCHAMBER;
				_typeSelected = false;
				tickCounter = 1;
				demoCounter = 0;
				tapImage.setAlpha(0);
				tapImage.invalidate();
				UpdateStage();				
			}
			break;
		case LIDCONTROL_SETCHAMBER:		
			if (demoCounter == 1)
			{
				int x = BtnLargeChamber.getX() + (BtnLargeChamber.getWidth() / 2) - (tapImage.getWidth() / 2);
				int y = BtnLargeChamber.getY() + BtnLargeChamber.getHeight() + 10;

				tapImage.setXY(x, y);
			}
			else if (demoCounter < 60)
			{ }
			else if (demoCounter < 120)
			{
				float percentage = (float)(demoCounter - 60) / 60.0f;
				tapImage.setAlpha((uint8_t)(percentage * 255));
				tapImage.invalidate();
			}
			else if (demoCounter > 200)
			{
				lidSubStage = LIDCONTROL_MAIN;
				_chamberSelected = true;
				tickCounter = 1;
				demoCounter = 0;
				tapImage.setAlpha(0);
				tapImage.invalidate();
				UpdateStage();
			}
			break;			

		case LIDCONTROL_MAIN:
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
			else if (demoCounter < 60) {}
			//Fade In Finger
			else if (demoCounter < 120)
			{
				float percentage = (float)(demoCounter - 60) / 60.0f;
				//touchgfx_printf("percentage = %f\n", percentage);
				pointerImage.setAlpha((uint8_t)(percentage * 255));
				pointerImage.invalidate();
			}
			//Make sure finger is on max, disable lever animation
			else if (demoCounter == 120)
			{
				pointerImage.setAlpha(255);
				pointerImage.invalidate();
				lvrLidControl.SetAnimationAllowed(false);
				LFT::Manual.LidDown();				
			}
			//Finger Down
			else if (demoCounter < 180)
			{
				int minimumY = lvrLidControl.getY() + (lvrLidControl.getHeight() / 2) - (pointerImage.getHeight() / 2);
				int maximumY = lvrLidControl.getY() + lvrLidControl.getHeight() - (pointerImage.getHeight() / 2) - 25;
				int distance = maximumY - minimumY;
				float percentage = (float)(demoCounter - 120) / 60;
				pointerImage.setY((uint16_t)(minimumY + (percentage * distance)));
				lvrLidControl.SetYPos((int)((lvrLidControl.getHeight() / 2) + (percentage * distance) - 25));
				pointerImage.invalidate();

				checkDemoLidState(false);

			}
			else if (demoCounter < 480) { checkDemoLidState(false); }
			//Finger Up
			else if (demoCounter == 539)
				_isDemoModeLidClosed = true;
			else if (demoCounter < 540)
			{
				int minimumY = lvrLidControl.getY() + (lvrLidControl.getHeight() / 2) - (pointerImage.getHeight() / 2);
				int maximumY = lvrLidControl.getY() + lvrLidControl.getHeight() - (pointerImage.getHeight() / 2) - 25;
				int distance = maximumY - minimumY;
				float percentage = (float)(demoCounter - 480) / 60;
				pointerImage.setY((int16_t)(maximumY - (percentage * distance)));
				lvrLidControl.SetYPos((int)(lvrLidControl.getHeight() - (percentage * distance) - 50));
				pointerImage.invalidate();
			}
			//Allow Animation again
			else if (demoCounter == 600)
			{
				lvrLidControl.SetAnimationAllowed(true);
			}
			//Fade Finger Out
			else if (demoCounter < 600)
			{
				float percentage = (float)(demoCounter - 540) / 60.0f;
				//touchgfx_printf("percentage = %f\n", percentage);
				pointerImage.setAlpha((uint8_t)(255 - (percentage * 255)));
				pointerImage.invalidate();
			}
			//Fade Tap Finger in
			else if (demoCounter < 660)
			{
				float percentage = (float)(demoCounter - 600) / 60.0f;
				tapImage.setAlpha((uint8_t)(percentage * 255));
				tapImage.invalidate();
			}
			//Move to next
			else if (demoCounter == 720)
			{
				pointerImage.setAlpha(0);
				tapImage.setAlpha(0);
				pointerImage.invalidate();
				tapImage.invalidate();

				lidSubStage = 0;
				demoStage++;
				tickCounter = 1;
				demoCounter = 0;
				UpdateStage();
			}
			break;
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
			tapImage.setAlpha((uint8_t)(percentage * 255));
			tapImage.invalidate();
		}		
		//Goto next stage
		else if (demoCounter == 240)
		{
			tapImage.setAlpha(0);
			tapImage.invalidate();

			demoStage = LFT_STAGE_FUMING;
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
			tapImage.setAlpha((uint8_t)(percentage * 255));
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
			tapImage.setAlpha((uint8_t)(percentage * 255));
			tapImage.invalidate();
			
		}
		//Goto next screen
		else if (demoCounter == 480)
		{							
			demoStage = LFT_STAGE_COOLDOWN;
			tapImage.setAlpha(0);						
			tickCounter = 1;
			demoCounter = 0;
			UpdateStage();
		}

		break;
	}
	case LFT_STAGE_FINISHED:
	{
		//Put lid back up
		if (demoCounter == 1)
			LFT::Manual.LidUp();
		else if (demoCounter < 400)		
			checkDemoLidState(true);		
		else if (demoCounter == 400)
			LFT::Manual.LidStop();

		//Wait a while then reset
		else if (demoCounter == 600)
		{			
			updateLid();

			demoStage = LFT_STAGE_LID_CONTROL;
			tickCounter = 1;
			demoCounter = 0;
			UpdateStage();			
		}
		break;
			
	}
	}


	//Stage Specific Actions
	switch (demoStage)
	{
	case LFT_STAGE_LID_CONTROL:
	{
		//Update the percentage bar with fake lid position		
		lvrLidControl.handleTickEvent();		
		updateLid();
		break;
	}	
	case LFT_STAGE_PRECHECKS:
	case LFT_STAGE_CHAMBER_CONDITIONING:
	case LFT_STAGE_COOLDOWN:
		progressBar.SetValue(tickCounter);		
#ifndef _MSC_VER 
		[[fallthrough]];
#endif
	case LFT_STAGE_FUMING:
		pulseStageText();
		break;	
	default:
		break;
	}

	if (tickCounter != 0 && tickCounter % DUMMY_WAIT_TIME == 0)
	{
		//Only progress if it's a stage that auto progresses
		if (demoStage == LFT_STAGE_CHAMBER_CONDITIONING || demoStage == LFT_STAGE_COOLDOWN || demoStage == LFT_STAGE_PRECHECKS)
		{			
			tickCounter = 0;

			NextStage();
		}
	}
}

void ProcessScreenView::Type1Selected()
{
	_typeSelected = false;
	NextSubStage();
}

void ProcessScreenView::Type2Selected()
{
	_typeSelected = true;
	NextSubStage();
}

void ProcessScreenView::SmallChamberSelected()
{
	_chamberSelected = false;
	NextSubStage();
}

void ProcessScreenView::LargeChamberSelected()
{
	_chamberSelected = true;
	NextSubStage();
}

void ProcessScreenView::Back()
{
	if (lidSubStage == 0)
	{
		application().gotoMainScreenNoTransition();
	}
	else
	{
		selectSingleStage = false;
		lidSubStage--;
		UpdateStage();
	}
}



void ProcessScreenView::ShowPowerLossDetected()
{
	PowerLossWindow.setVisible(true);
	PowerLossWindow.invalidate();
}

void ProcessScreenView::ShowRunsRemaining()
{
	FilterStateMessageWindow.setVisible(true);
	FilterStateMessageWindow.invalidate();
}

void ProcessScreenView::ShowMoistureDetectedWindow()
{
	MoistureMessage.setVisible(true);
	MoistureMessage.invalidate();
}

void ProcessScreenView::ShowProcessSelectorChamber()
{
	lidSubStage = LIDCONTROL_SETCHAMBER;
	selectSingleStage = true;
	UpdateStage();
}

void ProcessScreenView::ShowProcessSelectorType()
{
	lidSubStage = LIDCONTROL_SELECTTYPE;
	selectSingleStage = true;
	UpdateStage();
}

void ProcessScreenView::StartProcess()
{
	StartProcess(false);
}


void ProcessScreenView::StartProcess(bool skipOverwriteCheck)
{	
#ifdef SIMULATOR
	LFT::Auto.SetSettings();
#endif


	_processSuccess = true;
	_isUpdateCipherAllowed = true;

	if (!application().isDemoModeOn)
	{
		//Check if Warning should be given about overwriting log before continuing
		if (!skipOverwriteCheck)
		{						
				bool willOverwrite = LFT::Logs.IsNextLogPresent();
				if (willOverwrite)
				{
					
					ConfirmLogOverwrite.setVisible(true);
					ConfirmLogOverwrite.invalidate();
					return;
				}
			
		}

		//Set Parameters then start
		LFT::Auto.SetChamberSize(_chamberSelected);
		LFT::Auto.SetMetalType(_typeSelected);
		LFT::Auto.QuePrechecks();
		UpdateStage();
		tickCounter = 0;
		previousChamberConditioningPercentage = 0;
	}
}

void ProcessScreenView::StartFume()
{
	if (!application().isDemoModeOn)
	{
		LFT::Auto.QueFuming();
		UpdateStage();
		tickCounter = 0;
	}
}

void ProcessScreenView::StartCool()
{
	if (!application().isDemoModeOn)
	{
		LFT::Auto.QueCool();
		UpdateStage();
		tickCounter = 0;
	}
}

void ProcessScreenView::AbortProcess()
{	
	_processSuccess = false;
	_isUpdateCipherAllowed = false;

	//Hide Abort Confirmation
	AbortConfirmWindow.setVisible(false);
	AbortConfirmWindow.invalidate();
	
	//Reset the tick counter
	tickCounter = 0;

	//Abort will change current stage
	if (!application().isDemoModeOn)
	{
		LFT::Auto.QueAbort();		
	}

	//Show Abort Happening Window
	AbortInProcessWindow.setVisible(true);
	AbortInProcessWindow.invalidate();	
}

void ProcessScreenView::GotoHome()
{



	LFT::Auto.SetStage(LFT_STAGE_LID_CONTROL);
	LFT::AutoClean.SetStage(AUTOCLEAN_STAGE_NONE);
	LFT::Manual.SetBaseFanState(false);
	application().gotoMainScreenNoTransition();
}

