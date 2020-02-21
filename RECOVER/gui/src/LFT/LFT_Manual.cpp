#include <gui/LFT/LFT_Manual.hpp>

LFT_Manual::LFT_Manual(LFT_Information* information) :
	_information(information),
	_model(0)
{	
}


void LFT_Manual::SetStandbyState(bool state)
{	
	_information->StandbyMode = state;
	_model->SendBool("STANDBY", state);	
}

void LFT_Manual::SetInletState(bool state)
{
	_information->InletState = state;
	_model->SendBool("INLET", state);
}

void LFT_Manual::SetPurgeState(bool state)
{
	_information->PurgeState = state;
	_model->SendBool("PURGE", state);
}

void LFT_Manual::SetBypassState(bool state)
{
	_information->BypassState = state;
	_model->SendBool("BYPASS", state);
}

void LFT_Manual::SetPumpState(bool state)
{
	_information->PumpState = state;
	_model->SendBool("PUMP", state);
}

void LFT_Manual::SetBaseFanState(bool state)
{
	_information->BaseFanState = state;
	_model->SendBool("FAN", state);
}

void LFT_Manual::SetStiringFansState(bool state)
{
	_information->StiringFansState = state;
	_model->SendBool("STIRFAN", state);
}

void LFT_Manual::SetBaseTemperature(int temperature)
{
	_information->BaseHeaterSetpoint = temperature;
	_model->SendInt("SETBASE", temperature);
}

void LFT_Manual::SetPreTemperature(int temperature)
{
	_information->PrecursorHeaterSetpoint = temperature;
	_model->SendInt("SETPRE", temperature);
}

void LFT_Manual::BreakSeal(int time)
{	
	_model->SendInt("BREAKSEAL", time);
}

void LFT_Manual::SetOpenDoorState(bool state)	
{
	_information->DoorOpenState = state;
	_model->SendBool("OPENDOOR", state);
}

void LFT_Manual::RtcOn()
{
	_model->SendCommand("RTCON");
}

void LFT_Manual::Beep(int pitch, int length, int repeat)
{
	String command("BEEP ");
	command += String(pitch) + " " + String(length);

	if (repeat != 0)
		command += " " + String(repeat);

	int timeout = length + 100;

	if (repeat != 0)
		timeout = timeout * repeat;

	_model->SendCommand(command, timeout);
}

void LFT_Manual::Beep(int pitch)
{
	_model->SendInt("BEEP", pitch);
}

void LFT_Manual::LidDown()
{
	_model->SendCommand("LIDDOWN");
}

void LFT_Manual::LidUp()
{
	_model->SendCommand("LIDUP");
}

void LFT_Manual::LidStop()
{
	_information->ReadLidState();
	_model->SendCommand("LIDSTOP");
}

void LFT_Manual::ForceLidDown()
{
	_model->SendCommand("M-");
}

void LFT_Manual::ForceLidUp()
{
	_model->SendCommand("M+");
}

void LFT_Manual::ForceLidStop()
{
	_model->SendCommand("M0");
}

void LFT_Manual::SetModel(Model * model)
{
	_model = model;
}
