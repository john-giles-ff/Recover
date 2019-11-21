#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

void SettingsScreenView::setupScreen()
{	
	Unicode::snprintf(TxtBuildBuffer, TXTBUILD_SIZE, "%d.%d", Build::Number, LFT::Information.Build);
	Unicode::UnicodeChar utf16Buffer[10];
	Unicode::fromUTF8((const uint8_t *)((String)LFT::Information.SerialNumber).c_str(), utf16Buffer, 10);
	Unicode::snprintf(TxtSerialBuffer, TXTSERIAL_SIZE, "%s", utf16Buffer);
	
	//Load Languages onto screen
	int row = 0;
	int column = 0;
	for (int i = 0; i < LANGUAGE_COUNT; i++)
	{		
		//Setup Click Event
		LanguageTexts[i].SetClickEvent(LanguageClickedCallback);		

		//If current language, show as being selected (green text)
		touchgfx::LanguageId selectedId = Texts::getLanguage();
		if (LanguageTexts[i].GetLanguageID() == selectedId)
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(46, 172, 98));
		else
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		

		//Setup positioning
		int x = column * LANGUAGE_WIDTH;
		int y = row * LANGUAGE_HEIGHT;
		LanguageTexts[i].setXY(x, y);		
		LanguageTexts[i].setWidth(LANGUAGE_WIDTH);
		LanguageTexts[i].setHeight(LANGUAGE_HEIGHT);
		LanguageTexts[i].Text.setWidth(LANGUAGE_WIDTH);
		LanguageTexts[i].Text.setHeight(LANGUAGE_HEIGHT);

		//Increase Column
		column++;
		//If end of row, go to next one
		if (column == 2)
		{
			column = 0;
			row++;
		}		
		LanguageContainer.add(LanguageTexts[i]);		
	}
	
	LanguageContainer.invalidate();

	ConfirmResetFilterMsg.SetText(T_CONFIRMRESETFILTER);
	ConfirmResetFilterMsg.SetReturnedEvent(MsgBoxReturnedCallback);

	//Load Tabs
	TabGeneral.SetText(TypedText(T_GENERAL));
	TabLanguage.SetText(TypedText(T_LANGUAGE));
	TabTime.SetText(TypedText(T_TIME));

	TabGeneral.SetClickedAction(TabClickedCallback);
	TabLanguage.SetClickedAction(TabClickedCallback);
	TabTime.SetClickedAction(TabClickedCallback);
	
	UpdateToCurrentTab();	


	//Setup Idlescreen choices
	BtnDefault.setAction(IdleScreenOptionCallback);
	BtnScreenOff.setAction(IdleScreenOptionCallback);
	BtnClock.setAction(IdleScreenOptionCallback);	

	//Load LFT Idle Screen Setting
	selectedIdleMode = LFT::Settings.GetLockMode();	
	UpdateToSelectedIdleMode();

	//Setup View Diagnostics
	ViewDiagnosticsWindow.setVisible(false);
	add(ViewDiagnosticsWindow);

	//Load other settings
	TglChamberBeep.forceState(LFT::Information.ChamberReadyBeep);			
}

void SettingsScreenView::tearDownScreen()
{

}

void SettingsScreenView::StartAutoClean()
{
	LFT::AutoClean.StartAutoClean();
	application().gotoProcessScreenScreenNoTransition();
}

void SettingsScreenView::ToggleChamberReadyBeep()
{	
	LFT::Auto.SetChamberBeepReady(TglChamberBeep.getState());
	LFT::Settings.SetBeepWhenReadySetting(TglChamberBeep.getState());
}

void SettingsScreenView::ToggleCipherEnabled()
{
	LFT::Information.UserCipherMode = TglCipherEnabled.getState();
}

void SettingsScreenView::OpenViewDiagnostics()
{
	int* ciphers = LFT::Settings.GetCiphers();

	ViewDiagnosticsWindow.SetCiphers(ciphers, CIPHER_COUNT);
	ViewDiagnosticsWindow.setVisible(true);
	ViewDiagnosticsWindow.invalidate();
}


void SettingsScreenView::LanguageClicked(const TextRadioButton & u)
{													
	//Cycle through language Texts, if it's the one that was clicked make it green and change the language to the corresponding language
	//Otherwise change it to white
	for (int i = 0; i < LANGUAGE_COUNT; i++)
	{
		if (&u == &LanguageTexts[i])
		{
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(46, 172, 98));			
			Texts::setLanguage(LanguageTexts[i].GetLanguageID());
			LFT::Settings.SetLanguage(LanguageTexts[i].GetLanguageID());
		}
		else
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));					
	}	

	//Invalidate whole screen
	Backdrop.invalidate();	
}

void SettingsScreenView::MsgBoxReturned(const MsgBox & u, bool state)
{
	if (state)	
		LFT::Information.ResetFilterCounter();

	ConfirmResetFilterMsg.setVisible(false);
	ConfirmResetFilterMsg.invalidate();
}

void SettingsScreenView::IdleScreenOptionClicked(const touchgfx::AbstractButtonContainer & src)
{
	//Figure out which idle mode is selected
	if (&src == &BtnDefault)
		selectedIdleMode = 0;
	else if (&src == &BtnScreenOff)
		selectedIdleMode = 1;
	else if (&src == &BtnClock)
		selectedIdleMode = 2;

	//Set that as a setting
	LFT::Settings.SetLockMode(selectedIdleMode);

	//Update UI
	UpdateToSelectedIdleMode();
}

void SettingsScreenView::TabClicked(const Tab & src)
{	
	if (&src == &TabGeneral)
		currentTab = TAB_GENERAL;
	else if (&src == &TabLanguage)
		currentTab = TAB_LANGUAGE;
	else if (&src == &TabTime)
		currentTab = TAB_TIME;

	UpdateToCurrentTab();
}

void SettingsScreenView::UpdateToCurrentTab()
{
	TabGeneral.SetSelected(currentTab == TAB_GENERAL);
	TabLanguage.SetSelected(currentTab == TAB_LANGUAGE);
	TabTime.SetSelected(currentTab == TAB_TIME);

	ContentGeneral.setVisible(currentTab == TAB_GENERAL);
	ContentLanguage.setVisible(currentTab == TAB_LANGUAGE);
	ContentTime.setVisible(currentTab == TAB_TIME);

	//Initialise Date Time picker on tab select
	if (currentTab == TAB_TIME)			
		DateTimePicker.initialize();
	


	TabGeneral.invalidate();
	TabLanguage.invalidate();
	TabTime.invalidate();

	ContentGeneral.invalidate();
}

void SettingsScreenView::UpdateToSelectedIdleMode()
{
	colortype selected = touchgfx::Color::getColorFrom24BitRGB(46, 172, 98);
	colortype grey = touchgfx::Color::getColorFrom24BitRGB(0x69, 0x69, 0x69);
	colortype black = touchgfx::Color::getColorFrom24BitRGB(0x00, 0x00, 0x00);
	int defaultBorderSize = 2;
	int selectedBorderSize = 4;

	//Declare all possible options, order must link to number of selected option
	touchgfx::TextButtonStyle<touchgfx::IconButtonStyle<touchgfx::BoxWithBorderButtonStyle<touchgfx::ClickButtonTrigger>>>* options[3]
	{
		&BtnDefault,
		&BtnScreenOff,
		&BtnClock
	};


	//Loop through the options either selecting them or deselecting them
	for (int i = 0; i < 3; i++)
	{
		if (i == selectedIdleMode)
		{
			options[i]->setBorderSize(selectedBorderSize);
			options[i]->setBoxWithBorderColors(black, black, selected, selected);
		}
		else
		{
			options[i]->setBorderSize(defaultBorderSize);
			options[i]->setBoxWithBorderColors(black, black, grey, grey);
		}

		options[i]->invalidate();
	}

}
