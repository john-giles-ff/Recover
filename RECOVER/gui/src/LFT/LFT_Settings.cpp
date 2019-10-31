#include <gui/LFT/LFT_Settings.hpp>

LFT_Settings::LFT_Settings(LFT_Information * information)
{
	_information = information;

	_languageSetting.SetSemaphore(_information->xSemaphore);
	_beepWhenReadySetting.SetSemaphore(_information->xSemaphore);
	_runCounterSetting.SetSemaphore(_information->xSemaphore);
	_recoveryState.SetSemaphore(_information->xSemaphore);
	_usePurgeFans.SetSemaphore(_information->xSemaphore);
	_pcMode.SetSemaphore(_information->xSemaphore);
	_lockMode.SetSemaphore(_information->xSemaphore);
	_logOverwriteWarning.SetSemaphore(_information->xSemaphore);
}

void LFT_Settings::SetModel(Model * model)
{
	_model = model;
}

void LFT_Settings::SetLanguage(touchgfx::LanguageId languageID)
{
	_languageSetting = languageID;
	WriteInt(LANGUAGE_SETTING_LOCATION, languageID);
}

touchgfx::LanguageId LFT_Settings::ReadLanguage()
{
	_languageSetting = ReadInt(LANGUAGE_SETTING_LOCATION, DEFAULT_LANGUAGE);
	return _languageSetting;	
}

touchgfx::LanguageId LFT_Settings::GetLanguage()
{
	return _languageSetting;
}

void LFT_Settings::SetBeepWhenReadySetting(bool state)
{
	_beepWhenReadySetting = state;
	WriteInt(BEEP_WHEN_READY_SETTING_LOCATION, (int)state);
}

bool LFT_Settings::ReadBeepWhenReadySetting()
{
	_beepWhenReadySetting = (bool)ReadInt(BEEP_WHEN_READY_SETTING_LOCATION, DEFAULT_BEEP_WHEN_READY);
	return _beepWhenReadySetting;
}

bool LFT_Settings::GetBeepWhenReadySetting()
{
	return _beepWhenReadySetting;
}


void LFT_Settings::SetRecoveryState(int value)
{
	_recoveryState = value;
	WriteInt(RECOVERY_STATE, value);
}

int LFT_Settings::ReadRecoveryState()
{
	_recoveryState = ReadInt(RECOVERY_STATE, DEFAULT_RECOVERY_STATE);
	return _recoveryState;
}

int LFT_Settings::GetRecoveryState()
{
	return _recoveryState;
}

void LFT_Settings::SetUsePurgeFans(bool state)
{
	_usePurgeFans = state;
	WriteInt(USE_PURGE_FANS, (bool)state);
}

bool LFT_Settings::ReadUsePurgeFans()
{
	_usePurgeFans = (bool)ReadInt(USE_PURGE_FANS, DEFAULT_USE_PURGE_FANS);
	return _usePurgeFans;
}

bool LFT_Settings::GetUsePurgeFans()
{
	return _usePurgeFans;
}

void LFT_Settings::SetPCMode(int value)
{
	_pcMode = value;
	WriteInt(PC_MODE, value);
}

int LFT_Settings::ReadPCMode()
{
	_pcMode = ReadInt(PC_MODE, DEFAULT_PC_MODE);
	return _pcMode;
}

int LFT_Settings::GetPCMode()
{
	return _pcMode;
}

void LFT_Settings::SetLockMode(int value)
{
	_lockMode = value;
	WriteInt(LOCK_MODE, value);
}

int LFT_Settings::ReadLockMode()
{
	_lockMode = ReadInt(LOCK_MODE, DEFAULT_LOCK_MODE);
	return _lockMode;
}

int LFT_Settings::GetLockMode()
{
	return _lockMode;;
}


void LFT_Settings::WriteInt(int location, int value)
{
	//Make certain values are within range
	if (location < 0 || location > 99)
		return;
	if (value < -32767 || value > 32767)
		return;
	

	String params[3]{ "EWRITE", String(location), String(value) };
	String command = String::combine(' ', params, 3);
	_model->SendCommand(command);	
}

int LFT_Settings::ReadInt(int location, int defaultValue)
{
	int result = ReadInt(location);
	if (result >= 0)
		return result;
	else
		return defaultValue;
}

int LFT_Settings::ReadInt(int location)
{
	if (location < 0 || location > 99)
		return 0;	

	String params[2] { String("EREAD"), String(location) };	
	String command = String::combine(' ', params, 2);
	return _model->ReadInt(command);
}

void LFT_Settings::WriteString(int location, String value)
{
	if (location < 0 || location > 89)
		return;

	if (value.len() > 8)
		return;

	String params[3]{ String("WR"), String(location), value };
	String command = String::combine(' ', params, 3);
	_model->SendCommand(command);
}

String LFT_Settings::ReadString(int location)
{
	if (location < 0 || location > 99)
		return String();

	String params[2]{ String("RD"), String(location) };
	String command = String::combine(' ', params, 2);

	return _model->ReadString(command);
}
