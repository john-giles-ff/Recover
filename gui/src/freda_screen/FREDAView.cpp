#include <gui/freda_screen/FREDAView.hpp>


void FREDAView::setupScreen()
{	
	keyboard.setPosition(0, 0, 560, 280);
	keyboard.SetEnterCallback(enterClickedCallback);
	KeyboardContainer.add(keyboard);	
	
	TxtResponse.setWideTextAction(WIDE_TEXT_WORDWRAP);		

	lftDebug.setXY(0, 70);
	lftDebug.setWidth(240);
	lftDebug.setHeight(410);
	lftDebug.SetTextEnabledColor(Color::getColorFrom24BitRGB(0, 0, 0));
	lftDebug.SetTextDisabledColor(Color::getColorFrom24BitRGB(46, 172, 98));
	add(lftDebug);


	BtnToggleEngineerMode.forceState(application().isEngineerModeOn);

	GotoPresets();

	//Load States from model (via presenter)
	BtnToggleBaseFan.forceState(presenter->GetBaseFanState());	
	BtnToggleStiringFans.forceState(presenter->GetStiringFanState());
	BtnToggleBaseHeater.forceState(presenter->GetBaseHeaterSetPoint() > presenter->LOWTEMP_OFF());
	BtnToggleBypass.forceState(presenter->GetBypassState());
	BtnToggleInlet.forceState(presenter->GetInletState());
	BtnTogglePrecursorHeater.forceState(presenter->GetPrecursorHeaterSetPoint() > presenter->HIGHTEMP_OFF());
	BtnTogglePump.forceState(presenter->GetPumpState());
	BtnTogglePurge.forceState(presenter->GetPurgeState());
	BtnToggleStandby.forceState(presenter->GetStandbyState());		
	BtnToggleHideData.forceState(presenter->GetSensitiveDataHidden());
	
}

void FREDAView::tearDownScreen()
{

}

void FREDAView::ToggleEngineerMode()
{
	application().isEngineerModeOn = BtnToggleEngineerMode.getState();
	presenter->SetEngineeringMode(BtnToggleEngineerMode.getState());
}

void FREDAView::SendAndRead()
{
	Unicode::UnicodeChar* keyboardText = keyboard.getBuffer();
	int size = Unicode::strlen(keyboardText);

	if (size == 0)
		return;


	unsigned char* text = (unsigned char*)keyboardText;

	//Convert text from UTF-16 to UTF-8
	unsigned char fixedText[256];
	Utils::Utf16ToUtf8(text, fixedText, size);


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
	unsigned char fixedBuffer[256];

	//Increase from array length to word count (Otherwise last character is missed)
	length += 1;

	//Convert bytes to utf-16
	Utils::Utf8ToUtf16(data, fixedBuffer, length);
	for (int i = 0; i < length * 2; i++)
		data[i] = fixedBuffer[i];	

	//Double the length since it's utf-16
	Unicode::snprintf(TxtResponseBuffer, length, "%s", data);
	TxtResponse.invalidate();
}

void FREDAView::LidUp()
{
	presenter->LidUp();
}

void FREDAView::LidStop()
{
	presenter->LidStop();
}

void FREDAView::LidDown()
{
	presenter->LidDown();
}

void FREDAView::ToggleInlet()
{
	presenter->SetInletState(BtnToggleInlet.getState());
}

void FREDAView::TogglePurge()
{
	presenter->SetPurgeState(BtnTogglePurge.getState());
}

void FREDAView::ToggleBypass()
{
	presenter->SetBypassState(BtnToggleBypass.getState());
}

void FREDAView::TogglePump()
{
	presenter->SetPumpState(BtnTogglePump.getState());
}

void FREDAView::ToggleBaseFan()
{
	presenter->SetBaseFanState(BtnToggleBaseFan.getState());
}

void FREDAView::ToggleStiringFans()
{
	presenter->SetStiringFansState(BtnToggleStiringFans.getState());
}

void FREDAView::ToggleBaseHeater()
{
	if (BtnToggleBaseHeater.getState())
		presenter->SetBaseTemperature(presenter->LOWTEMP_ON());
	else
		presenter->SetBaseTemperature(presenter->LOWTEMP_OFF());
}

void FREDAView::TogglePrecursorHeater()
{
	if (BtnTogglePrecursorHeater.getState())
		presenter->SetPreTemperature(presenter->HIGHTEMP_ON());
	else
		presenter->SetPreTemperature(presenter->HIGHTEMP_OFF());

}

void FREDAView::ToggleStandby()
{
	presenter->SetStandbyState(BtnToggleStandby.getState());
	presenter->SetBaseFanState(!BtnToggleStandby.getState());

	BtnToggleBaseFan.forceState(!BtnToggleStandby.getState());
	BtnToggleBaseFan.invalidate();
}

void FREDAView::ToggleHideData()
{
	presenter->SetSensitiveDataHidden(BtnToggleHideData.getState());	
}

void FREDAView::BreakSeal()
{
	presenter->BreakSeal(3000);
	BtnToggleStiringFans.forceState(true);
	BtnToggleStiringFans.invalidate();
}

void FREDAView::SetHeaters()
{
	presenter->SetBaseTemperatureSetting();
	presenter->SetPreTemperatureSetting();
}

void FREDAView::Prechecks()
{
	presenter->QuePrechecks();
}

void FREDAView::PumpDown()
{
	presenter->QuePumpDown();
}

void FREDAView::Heat()
{
	presenter->QueHeat();
}

void FREDAView::Cool()
{
	presenter->QueCool();
}

void FREDAView::Off()
{
	presenter->Abort();
}

void FREDAView::ResetTimer()
{
	presenter->ResetTimer();
}

void FREDAView::GotoCustom()
{
	PresetCommands.setVisible(false);
	CustomCommands.setVisible(true);
	AutoCommands.setVisible(false);

	CustomCommands.invalidate();
}

void FREDAView::GotoPresets()
{
	PresetCommands.setVisible(true);
	CustomCommands.setVisible(false);
	AutoCommands.setVisible(false);

	PresetCommands.invalidate();
}

void FREDAView::GotoAuto()
{
	CustomCommands.setVisible(false);
	PresetCommands.setVisible(false);
	AutoCommands.setVisible(true);

	PresetCommands.invalidate();
}

void FREDAView::handleTickEvent()
{	
	GetValues();		
}

void FREDAView::GetValues()
{		
	presenter->UpdateLFTDebug(&lftDebug);
	lftDebug.invalidate();	
}

void FREDAView::enterPressedHandler(const CustomKeyboard& u)
{
	SendAndRead();
}



