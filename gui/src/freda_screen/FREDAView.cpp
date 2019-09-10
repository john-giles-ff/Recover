#include <gui/freda_screen/FREDAView.hpp>


void FREDAView::setupScreen()
{	
	//Language should always be english in the engineer mode
	currentLanguage = Texts::getLanguage();
	Texts::setLanguage(GB);

	//Setup Keyboard
	keyboard.setPosition(0, 0, 560, 280);
	keyboard.SetEnterCallback(enterClickedCallback);
	KeyboardContainer.add(keyboard);	
	
	//Setup Response
	TxtResponse.setWideTextAction(WIDE_TEXT_WORDWRAP);		

	//Setup keypad
	ClockKeypad.SetReturnedEvent(ClockKeypadReturnedCallback);

	//Setup Values
	lftDebug.setXY(0, 70);
	lftDebug.setWidth(240);
	lftDebug.setHeight(410);
	lftDebug.SetTextEnabledColor(Color::getColorFrom24BitRGB(0, 0, 0));
	lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
	add(lftDebug);

	//Make sure we start on the preset window
	SoakTestWindow.setVisible(false);
	GotoPresets();		

	/*
	static uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
	CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
#ifdef SIMULATOR
	//CanvasWidgetRenderer::setWriteMemoryUsageReport(true);
#endif
	*/

	//Setup Run Counter
	Unicode::snprintf(TxtRunCountBuffer, TXTRUNCOUNT_SIZE, "%d", LFT::Information.RunCounter);

	//Setup Graphs
	BtnActualVisible.forceState(true);

	upperGraph.setXY(5, 0);	
	lowerGraph.setXY(5, 0);
	actualGraph.setXY(5, 0);
	upperGraph.setup(555, 280, Color::getColorFrom24BitRGB(0x00, 0x00, 0xFF), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	lowerGraph.setup(555, 280, Color::getColorFrom24BitRGB(0x00, 0x00, 0xFF), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	actualGraph.setup(555, 280, Color::getColorFrom24BitRGB(0xFF, 0x00, 0x00), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	upperGraph.setDotsVisible(false);
	lowerGraph.setDotsVisible(false);
	actualGraph.setDotsVisible(false);
	upperGraph.setDotsBackgroundVisible(false);
	lowerGraph.setDotsBackgroundVisible(false);
	actualGraph.setDotsBackgroundVisible(false);	
	upperGraph.setAreaVisible(false);
	lowerGraph.setAreaVisible(false);
	actualGraph.setAreaVisible(false);

	GraphDisplay.add(upperGraph);
	GraphDisplay.add(actualGraph);		
	GraphDisplay.add(lowerGraph);

	UpdateControlStates();
	UpdateGraphs();
}

void FREDAView::tearDownScreen()
{
	Texts::setLanguage(currentLanguage);
}

void FREDAView::ToggleEngineerMode()
{
	LFT::Information.EngineeringMode = BtnToggleEngineerMode.getState();	
}

void FREDAView::SendAndRead()
{
	Unicode::UnicodeChar* keyboardText = keyboard.getBuffer();
	int size = Unicode::strlen(keyboardText);

	if (size == 0)
		return;

	//Convert text from UTF-16 to UTF-8
	unsigned char fixedText[256];	
	Unicode::toUTF8(keyboardText, fixedText, size);


	//Add newline char if it's not at the end
	if (fixedText[size - 1] != 0x0A)
	{
		fixedText[size] = 0x0A;
		size++;
	}

	//Send then read until a newline appears
	int length = 0;
	unsigned char data[256];
	presenter->SendAndRead(fixedText, size, data, length);

	if (length == 0)
		return;

	//Convert text from UTF-8 to UFT-16	
	uint16_t fixedBuffer[256];

	//Increase from array length to word count (Otherwise last character is missed)
	length += 1;

	//Convert bytes to utf-16
	Unicode::fromUTF8(data, fixedBuffer, length);	

	//Double the length since it's utf-16
	Unicode::snprintf(TxtResponseBuffer, length, "%s", fixedBuffer);
	TxtResponse.invalidate();
}

void FREDAView::LidUp()
{	
	if (BtnToggleForceLid.getState())
		LFT::Manual.ForceLidUp();
	else
		LFT::Manual.LidUp();	
}

void FREDAView::LidStop()
{
	if (BtnToggleForceLid.getState())
		LFT::Manual.ForceLidStop();
	else
		LFT::Manual.LidStop();	
}

void FREDAView::LidDown()
{
	if (BtnToggleForceLid.getState())
		LFT::Manual.ForceLidDown();
	else 
		LFT::Manual.LidDown();
	
}

void FREDAView::ToggleInlet()
{
	LFT::Manual.SetInletState(BtnToggleInlet.getState());
}

void FREDAView::TogglePurge()
{
	LFT::Manual.SetPurgeState(BtnTogglePurge.getState());
}

void FREDAView::ToggleBypass()
{
	LFT::Manual.SetBypassState(BtnToggleBypass.getState());
}

void FREDAView::TogglePump()
{
	LFT::Manual.SetPumpState(BtnTogglePump.getState());
}

void FREDAView::ToggleBaseFan()
{
	LFT::Manual.SetBaseFanState(BtnToggleBaseFan.getState());
}

void FREDAView::ToggleStiringFans()
{
	LFT::Manual.SetStiringFansState(BtnToggleStiringFans.getState());
}

void FREDAView::ToggleBaseHeater()
{
	if (BtnToggleBaseHeater.getState())
		LFT::Manual.SetBaseTemperature(LFT::Information.LOWTEMP_ON);
	else
		LFT::Manual.SetBaseTemperature(LFT::Information.LOWTEMP_OFF);
}

void FREDAView::TogglePrecursorHeater()
{
	if (BtnTogglePrecursorHeater.getState())
		LFT::Manual.SetPreTemperature(LFT::Information.HIGHTEMP_ON);
	else
		LFT::Manual.SetPreTemperature(LFT::Information.HIGHTEMP_OFF);

}

void FREDAView::ToggleStandby()
{
	LFT::Manual.SetStandbyState(BtnToggleStandby.getState());	
	LFT::Manual.SetBaseFanState(!BtnToggleStandby.getState());

	BtnToggleBaseFan.forceState(!BtnToggleStandby.getState());
	BtnToggleBaseFan.invalidate();

	UpdateStandbyButtons();
}

void FREDAView::ToggleHideData()
{		
	LFT::Information.SensitiveDataHidden = BtnToggleHideData.getState();
}

void FREDAView::BreakSeal()
{
	LFT::Manual.BreakSeal();
	BtnToggleStiringFans.forceState(true);
	BtnToggleStiringFans.invalidate();
}

void FREDAView::ToggleOpenDoor()
{
	LFT::Manual.SetOpenDoorState(BtnToggleOpenDoor.getState());
}


void FREDAView::Off()
{
	LFT::ProductionTests.AbortProductionTest();
	LFT::Auto.Abort();
}

void FREDAView::Clear()
{		
	LFT::Auto.ClearErrors();
}


void FREDAView::SoakTest()
{
	LFT::Information.ResetTimer();
	LFT::ProductionTests.QueSoakTest();
}

void FREDAView::StopProductionTest()
{
	LFT::ProductionTests.AbortProductionTest();
}

void FREDAView::Format()
{	
	LFT::Information.FormatData();
}

void FREDAView::ResetTimer()
{
	//presenter->Test();	
	LFT::Information.ResetTimer();	
}

void FREDAView::ToggleEMCMode()
{	
	if (BtnToggleEMCMode.getState())
		LFT::ProductionTests.QueEMCMode();
	else
		LFT::ProductionTests.IsEMCModeOn = false;
}

void FREDAView::ShowClockConfig()
{
	//Turn on RTC Clock
	LFT::Manual.RtcOn();

	//Show clock keypad
	ClockKeypad.initialize();
	ClockKeypad.setVisible(true);
	ClockKeypad.invalidate();
}



void FREDAView::GotoCustom()
{
	ProductionTestFeedback.setVisible(false);
	PresetCommands.setVisible(false);
	CustomCommands.setVisible(true);
	AutoCommands.setVisible(false);
	Tabs.setVisible(true);
	GraphDisplay.setVisible(false);

	CustomCommands.invalidate();
}

void FREDAView::GotoPresets()
{
	UpdateControlStates();
	ProductionTestFeedback.setVisible(false);
	PresetCommands.setVisible(true);
	CustomCommands.setVisible(false);
	AutoCommands.setVisible(false);
	Tabs.setVisible(true);
	GraphDisplay.setVisible(false);

	PresetCommands.invalidate();
	UpdateStandbyButtons();
}

void FREDAView::GotoAuto()
{
	UpdateControlStates();
	ProductionTestFeedback.setVisible(false);
	CustomCommands.setVisible(false);
	PresetCommands.setVisible(false);
	AutoCommands.setVisible(true);
	Tabs.setVisible(true);
	GraphDisplay.setVisible(false);

	AutoCommands.invalidate();
}

void FREDAView::GotoGraphs()
{
	ProductionTestFeedback.setVisible(false);
	CustomCommands.setVisible(false);
	PresetCommands.setVisible(false);
	AutoCommands.setVisible(false);
	Tabs.setVisible(false);
	GraphDisplay.setVisible(true);	

	GraphDisplay.invalidate();
}

void FREDAView::handleTickEvent()
{	
	GetValues();			
	StepsScroll.invalidate();	

	if (CntBigClock.isVisible())
	{
		if (!IsBigClockVisible)
		{
			LFT::Information.AlwaysUpdateRTC = true;
			IsBigClockVisible = true;
		}


		//This is used for EMC testing, because of this it uses the RTC clock from firmware instead of the board
		STime time = STime(((DateTime)LFT::Information.RtcTime).getRaw());
		int hour = time.GetHour();
		int minute = time.GetMinute();
		int second = time.GetSecond();		

		touchgfx::Unicode::snprintf(TxtBigClockBuffer, TXTBIGCLOCK_SIZE, "%02d:%02d:%02d", hour, minute, second);
		TxtBigClock.invalidate();
	}
	else if (IsBigClockVisible)
	{
		LFT::Information.AlwaysUpdateRTC = false;
		IsBigClockVisible = false;
	}



	//Update Graph Values
	if (LFT::ProductionTests.IsPumpdownTestQued)
	{
		//Only update if there are less points on the graph than in the data
		if (onScreenActualValues < LFT::ProductionTests.LFT_ACTUAL_LENGTH)
		{
			int x = (int)(LFT::ProductionTests.LFTActual[onScreenActualValues].Time / 10);
			int y = LFT::ProductionTests.GetStatusValue(currentGraphType, LFT::ProductionTests.LFTActual[onScreenActualValues]);

			onScreenActualValues++;

			actualGraph.addValue(x, y);
			actualGraph.invalidate();
		}

		//Update Elapsed Time
		STime time = LFT::ProductionTests.GraphPumpDownTime;
		int hour = time.GetHour();
		int minute = time.GetMinute();
		int second = time.GetSecond();
		touchgfx::Unicode::snprintf(TxtGraphElapsedBuffer, TXTGRAPHELAPSED_SIZE, "%02d:%02d:%02d", hour, minute, second);
		TxtGraphElapsed.invalidate();
	}

	//Update Max Voltage
	float baseCurrent = LFT::ProductionTests.maxBaseCurrent;
	float preCurrent = LFT::ProductionTests.maxPreCurrent;
	
	if (currentGraphType == Graph_BasePower && onScreenMaxBaseCurrent != baseCurrent)
	{
		Unicode::snprintfFloat(TxtGraphMaxCurrentBuffer, TXTGRAPHMAXCURRENT_SIZE, "%f", baseCurrent);
		
		if (baseCurrent > BaseCurrentLowerBound && baseCurrent < BaseCurrentUpperBound)
			TxtGraphMaxCurrent.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 255, 0));
		else
			TxtGraphMaxCurrent.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));

		//Update On screen value and invalidate control
		onScreenMaxBaseCurrent = baseCurrent;
		TxtGraphMaxCurrent.invalidate();
	}		

	if (currentGraphType == Graph_PrePower && onScreenMaxPreCurrent != preCurrent)
	{
		Unicode::snprintfFloat(TxtGraphMaxCurrentBuffer, TXTGRAPHMAXCURRENT_SIZE, "%f", preCurrent);
		
		if (preCurrent > PrecursorCurrentLowerBound && preCurrent < PrecursorCurrentUpperBound)
			TxtGraphMaxCurrent.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 255, 0));
		else
			TxtGraphMaxCurrent.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));

		//Update On screen value and invalidate control
		onScreenMaxPreCurrent = preCurrent;
		TxtGraphMaxCurrent.invalidate();
	}

	//Update SoakTest Values
	if (LFT::ProductionTests.IsSoakTestQued)
	{
		if (LFT::ProductionTests.GetProductionTestAborted())
		{
			Unicode::snprintf(TxtSoakTestStatusBuffer, TXTSOAKTESTSTATUS_SIZE, "Aborting");
		}
		else
		{
			Unicode::snprintf(TxtSoakTestCountBuffer, TXTSOAKTESTCOUNT_SIZE, "%d", LFT::ProductionTests.SoakTestCount);
			Unicode::snprintf(TxtSoakTestTotalBuffer, TXTSOAKTESTTOTAL_SIZE, "%d", LFT::Information.RunCounter);
			Unicode::snprintf(TxtSoakTestStatusBuffer, TXTSOAKTESTSTATUS_SIZE, "%d Running", LFT::ProductionTests.SoakTestStatus);
		}

		TxtSoakTestCount.invalidate();
		TxtSoakTestStatus.invalidate();
	}	
	else if (SoakTestWindow.isVisible())
	{
		Unicode::snprintf(TxtSoakTestStatusBuffer, TXTSOAKTESTCOUNT_SIZE, "Stopped");
		TxtSoakTestStatus.invalidate();
	}

}

void FREDAView::GetValues()
{		
	LFT::Information.UpdateLFTDebug(&lftDebug);	
	lftDebug.invalidate();
}

void FREDAView::ToggleActualVisible()
{
	actualGraph.setVisible(BtnActualVisible.getState());
	actualGraph.invalidate();
}

void FREDAView::AbortGraphTest()
{	
	LFT::ProductionTests.AbortProductionTest();
}

void FREDAView::SwitchGraphMode()
{
	switch (currentGraphType)
	{
	case Graph_Pressure:
		currentGraphType = Graph_BaseTemp;
		TxtDataType.setTypedText(TypedText(T_GRAPHTYPE_BASEHEAT));
		break;
	case Graph_BaseTemp:
		currentGraphType = Graph_BasePower;
		TxtDataType.setTypedText(TypedText(T_GRAPHTYPE_BASEPOWER));
		break;
	case Graph_BasePower:
		currentGraphType = Graph_PreTemp;
		TxtDataType.setTypedText(TypedText(T_GRAPHTYPE_PREHEAT));
		break;
	case Graph_PreTemp:
		currentGraphType = Graph_PrePower;
		TxtDataType.setTypedText(TypedText(T_GRAPHTYPE_PREPOWER));
		break;
	case Graph_PrePower:
		currentGraphType = Graph_Pressure;
		TxtDataType.setTypedText(TypedText(T_GRAPHTYPE_PRESSURE));
		break;
	default:
		break;
	}

	UpdateGraphs();
}

void FREDAView::StartGraphTest()
{
	if (LFT::ProductionTests.IsPumpdownTestQued)
		return;

	LFT::ProductionTests.QueGraphPumpdown();
	LFT::ProductionTests.LFT_ACTUAL_LENGTH = 0;
	onScreenActualValues = 0;

	UpdateGraphs();
}


void FREDAView::enterPressedHandler(const CustomKeyboard& u)
{
	SendAndRead();
}

void FREDAView::ClockKeypadReturned(const Keypad & u, bool state)
{
	ClockKeypad.setVisible(false);
	ClockKeypad.invalidate();

	if (state)	
		LFT::Information.SetClockFrequency(ClockKeypad.GetNumber());
	


}

void FREDAView::UpdateControlStates()
{
	//Load States from Static Object
	BtnToggleEngineerMode.forceState(LFT::Information.EngineeringMode);
	BtnToggleBaseFan.forceState(LFT::Information.BaseFanState);
	BtnToggleStiringFans.forceState(LFT::Information.StiringFansState);
	BtnToggleBaseHeater.forceState(LFT::Information.BaseHeaterSetpoint > LFT::Information.LOWTEMP_OFF);
	BtnToggleBypass.forceState(LFT::Information.BypassState);
	BtnToggleInlet.forceState(LFT::Information.InletState);
	BtnTogglePrecursorHeater.forceState(LFT::Information.PrecursorHeaterSetpoint > LFT::Information.HIGHTEMP_OFF);
	BtnTogglePump.forceState(LFT::Information.PumpState);
	BtnTogglePurge.forceState(LFT::Information.PurgeState);
	BtnToggleStandby.forceState(LFT::Information.StandbyMode);
	BtnToggleHideData.forceState(LFT::Information.SensitiveDataHidden);
	BtnToggleOpenDoor.forceState(LFT::Information.DoorOpenState);
}

void FREDAView::UpdateStandbyButtons()
{
	bool state = BtnToggleStandby.getState();
	for (int i = 0; i < STANDBYBUTTON_COUNT; i++)
	{
		if (state)
		{
			standbyButtons[i]->setAlpha(50);
			standbyButtons[i]->setTouchable(false);
			standbyButtons[i]->invalidate();
		}
		else
		{
			standbyButtons[i]->setAlpha(255);
			standbyButtons[i]->setTouchable(true);
			standbyButtons[i]->invalidate();
		}
	}


}

void FREDAView::UpdateGraphs()
{
	upperGraph.clear();
	lowerGraph.clear();
	actualGraph.clear();
	TxtGraphMaxCurrent.setVisible(currentGraphType == Graph_BasePower  || currentGraphType == Graph_PrePower);
	onScreenMaxBaseCurrent = 0;
	onScreenMaxPreCurrent = 0;


	


	TxtGraphMaxCurrent.invalidate();

	for (int i = 0; i < LFT::ProductionTests.LFT_ACTUAL_LENGTH; i++)
	{
		int x = (int)(LFT::ProductionTests.LFTActual[i].Time / 10);		
		int y = LFT::ProductionTests.GetStatusValue(currentGraphType, LFT::ProductionTests.LFTActual[i]);
		
		actualGraph.addValue(x, y);
	}


	switch (currentGraphType)
	{
	case Graph_Pressure:
		upperGraph.setRange(0, 360, 250, 0);
		lowerGraph.setRange(0, 360, 250, 0);
		actualGraph.setRange(0, 360, 250, 0);
		break;
	case Graph_BaseTemp:
		upperGraph.setRange(0, 360, 60, 0);
		lowerGraph.setRange(0, 360, 60, 0);
		actualGraph.setRange(0, 360, 60, 0);
		break;
	case Graph_BasePower:
		upperGraph.setRange(0, 360, 100, 0);
		lowerGraph.setRange(0, 360, 100, 0);
		actualGraph.setRange(0, 360, 100, 0);
		break;
	case Graph_PreTemp:
		upperGraph.setRange(0, 360, 200, 0);
		lowerGraph.setRange(0, 360, 200, 0);
		actualGraph.setRange(0, 360, 200, 0);
		break;
	case Graph_PrePower:
		upperGraph.setRange(0, 360, 100, 0);
		lowerGraph.setRange(0, 360, 100, 0);
		actualGraph.setRange(0, 360, 100, 0);
		break;	
	}

	actualGraph.invalidate();
}



