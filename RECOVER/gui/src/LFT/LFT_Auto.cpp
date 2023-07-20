#include <gui/LFT/LFT_Auto.hpp>

LFT_Auto::LFT_Auto(LFT_Information* information, LFT_Manual* manual, LFT_Settings* settings, LFT_AutoClean* autoClean) :
	_model(0),
	_information(information),
	_manual(manual),
	_settings(settings),
	_autoClean(autoClean)
{	
	CurrentStage.SetSemaphore(_information->xSemaphore);
}

void LFT_Auto::SetModel(Model * model)
{
	_model = model;
}

void LFT_Auto::SetBaseTemperatureSetting(int temperature)
{
	if (temperature == -1)
		temperature = _information->LOWTEMP_ON;

	_model->SendInt("BASETEMP", temperature);
}

void LFT_Auto::SetPreTemperatureSetting(int temperature)
{
	if (temperature == -1)
		temperature = _information->HIGHTEMP_ON;

	_model->SendInt("PRETEMP", temperature);
}

void LFT_Auto::SetVac(int mTorr)
{
	if (mTorr == -1 || mTorr == 0)
		mTorr = _information->VAC_VALUE;

	_model->SendInt("SETVAC", mTorr);
}

void LFT_Auto::SetVacMax(int mTorr)
{
	if (mTorr == -1 || mTorr == 0)
		mTorr = _information->VAC_MAX;
	
	_model->SendInt("SETMAX", mTorr);
}

void LFT_Auto::SetLeakMax(int mTorrPer10s)
{
	if (mTorrPer10s == -1 || mTorrPer10s == 0)
		mTorrPer10s = _information->LEAKRATE_MAX;

	_model->SendInt("MAXLEAK", mTorrPer10s);

}

void LFT_Auto::SetTimeout(int timeout1, int timeout2)
{
	if (timeout1 <= 0)
		timeout1 = _information->DEFAULT_TIMEOUT1;
	if (timeout2 <= 0)
		timeout2 = _information->DEFAULT_TIMEOUT2;

	_model->SendInt("TIMEOUT30", timeout1);
	_model->SendInt("TIMEOUT45", timeout2);	
}

void LFT_Auto::SetSampleRate(int rate)
{
	//This value must be a multiple of 10

	//If -1, then use default
	if (rate == -1)
		rate = 30;		

	_model->SendInt("SAMPLERATE", rate);
}

void LFT_Auto::SetChamberSize(bool value)
{
	_information->ChamberState = value;
	if (value)
		_model->SendCommand("LARGECHAMBER");
	else
		_model->SendCommand("SMALLCHAMBER");
}

void LFT_Auto::SetMetalType(bool value)
{	
	_model->SendBool("PROCINFO1", value);
}

void LFT_Auto::SetChamberBeepReady(bool value)
{
	_information->ChamberReadyBeep = value;

	//Switched in software so value is positive when sound is played
	_model->SendBool("NOSOUND", !value);
}

void LFT_Auto::SetStirTime(int minutes)
{
	if (minutes == 0)
		minutes = _information->STIRRING_TIME;

	_model->SendInt("SETSTIRTIME", minutes);
}

void LFT_Auto::SetUsePurgeFans(bool value)
{
	_model->SendBool("STIRFANAUTO", value);
}

void LFT_Auto::RecoverToStage(int stage)
{
	CurrentStage = stage;
	_information->AllowStageAutoProgression = true;
	_information->ProgressUpdating = true;	

}

void LFT_Auto::QueStage(int stage)
{		

	switch (stage)
	{
	case LFT_STAGE_PRECHECKS:
		QuePrechecks();
		break;
	case LFT_STAGE_CHAMBER_CONDITIONING:
		QueChamberConditioning();
		break;
	case LFT_STAGE_FUMING:
		QueFuming();
		break;
	case LFT_STAGE_COOLDOWN:
		QueCool();
		break;
	case LFT_STAGE_READY_TO_FUME:
		DisableTimeout(true);
		SetStage(stage);
		break;
	case LFT_STAGE_FINISHED:		
		_information->ReadRunCounter();
		_information->ReadFilterCounter();
		SetStage(stage);
		break;
	default:
		SetStage(stage);		
		break;
	}
}

void LFT_Auto::QuePrechecks()
{
	_information->PreChecksRequired = true;	
	SetStage(LFT_STAGE_PRECHECKS);
}

void LFT_Auto::QueChamberConditioning()
{
	_information->ChamberConditioningRequired = true;	
	SetStage(LFT_STAGE_CHAMBER_CONDITIONING);
}

void LFT_Auto::QueFuming()
{
	_information->FumingRequired = true;	
	SetStage(LFT_STAGE_FUMING);
}

void LFT_Auto::QueCool()
{
	_information->CoolRequired = true;	
	SetStage(LFT_STAGE_COOLDOWN);
}

void LFT_Auto::QueAbort()
{
	_autoClean->Abort();

	_information->ProgressUpdating = false;
	_information->Progress = 0;

	_information->AbortRequired = true;		
}



void LFT_Auto::Abort()
{			
	_model->SendCommand("HALT");
}

void LFT_Auto::StartDrying(int min)
{
	if (min == -1)
		min = 15;

	_model->SendInt("DRYING", min);
	_model->SendCommand("DRYING");
}

int LFT_Auto::ReadDryingPercentage()
{
	String times = _model->ReadString("DRYING TIME");	

#ifdef SIMULATOR
	times = "¬10:00 00:00\n";
#endif

	int totalMin = 0, totalSec = 0, total = 0;
	int elapsedMin = 0, elapsedSec = 0, elapsed = 0;

	int position = 0;

	//Move to start digit
	int lastPos = 0;
	while (!String::isDigit(times[lastPos]))
		lastPos++;

	//Read data
	for (unsigned int i = 0; i < times.len(); i++)
	{
		if (times[i] != ':' && times[i] != ' ' && times[i] != '\n')
			continue;
		
		String sub = times.substr(lastPos, i - lastPos);
		int val = sub.toInt();

		if (val < 0)
			return 0;
		
		switch (position++)
		{
		case 0:
			totalMin = val;
			break;
		case 1:
			totalSec = val;
			break;
		case 2:
			elapsedMin = val;
			break;
		case 3:
			elapsedSec = val;
			break;
		default:
			break;
		}

		lastPos = i + 1;
	}

	//Tally up parts
	total = (totalMin * 60) + totalSec;
	elapsed = (elapsedMin * 60) + elapsedSec;

	//Return percentage
	return (int)(((float)elapsed / (float)total) * 100.0f);
}

void LFT_Auto::AbortCleanup()
{	
	//If precursor heater has heated up, do a full purge
	if (CurrentStage > LFT_STAGE_READY_TO_FUME)
	{
		QueStage(LFT_STAGE_COOLDOWN);
		return;
	}
	
	//Turn off standby
	_manual->SetStandbyState(false);

	//Since the stage is before the pre-cursor has been ran, we can skip cooling and purging	
	//Open the valves	
	_manual->SetInletState(true);

#ifndef  SIMULATOR
	//Wait 5 Seconds
	vTaskDelay(5000);
#endif
	_manual->SetPurgeState(true);

	//Wait for switch to be open
	while (_information->InternalSwitchValue1 != 1)
	{
		_information->ReadInternalSwitch1();
#ifndef  SIMULATOR
		//Wait 1 Second
		vTaskDelay(1000);
#endif
	}

	//Wait for the last bit of air to release
#ifndef  SIMULATOR
	//Wait 5 Seconds
	vTaskDelay(5000);
#endif

	//BreakSeal
	_manual->BreakSeal();

#ifndef  SIMULATOR
	//Wait 12 Seconds
	vTaskDelay(12000);
#endif
	
	//Stop Stirring Fans
	_manual->SetStiringFansState(false);

	//Put lid up
	_manual->LidUp();			

#ifndef  SIMULATOR
	//Wait 0.3 seconds
	vTaskDelay(300);
#endif

	//Set stage to finished
	SetStage(LFT_STAGE_FINISHED);
}

void LFT_Auto::SetStage(int value)
{
	_settings->SetRecoveryState(value);
	CurrentStage = value;
}

int LFT_Auto::GetStage()
{
	if (_autoClean->GetState() == AUTOCLEAN_STAGE_FINISHED)			
		return LFT_STAGE_FINISHED;	
	if (_autoClean->GetState() != AUTOCLEAN_STAGE_NONE)
		return LFT_STAGE_TUNING;

	return CurrentStage;
}

void LFT_Auto::ClearErrors()
{
	_model->SendCommand("CLEAR");
}

void LFT_Auto::DisableTimeout(bool state)
{
	_model->SendBool("DISABLETIMEOUT", state);
}

void LFT_Auto::SetSettings()
{	
	SetPreTemperatureSetting();
	SetBaseTemperatureSetting();
	SetVac();
	SetVacMax();
	SetLeakMax();
	SetStirTime();	
	SetTimeout();
	SetSampleRate();
	SetUsePurgeFans(true);
	_information->ClearPerformance();
}

void LFT_Auto::StartPreChecks()
{
	_model->SendCommand("INITIALISE");

	//Remove Que'd Command
	_information->PreChecksRequired = false;

	//Reset Comms and Progress
	_model->ResetComms();
	_information->Progress = 0;
	_information->ProgressUpdating = true;
}

void LFT_Auto::StartChamberConditioning()
{
	_model->SendCommand("PUMPDOWN");

	//Remove Que'd Command
	_information->ChamberConditioningRequired = false;

	//Set Start Timer
	_information->ConditioningStartTime = _information->GetCurrentTime();

	//Reset Comms and Progress
	_model->ResetComms();
	_information->Progress = 0;
	_information->ProgressUpdating = true;
}

void LFT_Auto::StartFuming()
{
	//Invalidate Information for next run
	_information->ConditioningStartTime = DateTime();
	_information->Delta = -1;

	//Setup Fuming timer
	_information->FumingStartTime = _information->GetCurrentTime();


	_model->SendCommand("HEAT");

	//Remove Que'd Command
	_information->FumingRequired = false;

	//Reset Comms and Progress
	_model->ResetComms();
	_information->Progress = 0;	
}

void LFT_Auto::StartCool()
{
	//Invalidate information for next run
	_information->FumingStartTime = DateTime();

	_model->SendCommand("COOL");

	//Remove Que'd Command
	_information->CoolRequired = false;

	//Reset Comms and Progress
	_model->ResetComms();
	_information->Progress = 0;
	_information->ProgressUpdating = true;
}
