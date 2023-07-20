#include <gui/LFT/LFT_AutoClean.hpp>

LFT_AutoClean::LFT_AutoClean(LFT_Information* information, LFT_Manual* manual, LFT_Settings* settings) :
	_model(0),
	_information(information),
	_manual(manual),
	_settings(settings)
{		
	_status.SetSemaphore(_information->xSemaphore);
	_totalStartTime.SetSemaphore(_information->xSemaphore);
	_finished.SetSemaphore(_information->xSemaphore);
	_abort.SetSemaphore(_information->xSemaphore);
	_isStandalone.SetSemaphore(_information->xSemaphore);
}

void LFT_AutoClean::SetModel(Model* model)
{
	_model = model;
}



void LFT_AutoClean::StartAutoClean()
{
	if (_currentStage != AUTOCLEAN_STAGE_NONE)
		return;

	//Reset Values
	_finished = false;
	_abort = false;
	_status = 0;
	_totalStartTime = DateTime(0);

	//Setup Values
	_currentStage = AUTOCLEAN_STAGE_START;
	_stageStartTime = _information->GetCurrentTime();	
	_totalStartTime = _information->GetCurrentTime();

	_information->AllowStageAutoProgression = true;	
}

void LFT_AutoClean::ProcessAutoClean()
{
	if (_abort)
	{
		_currentStage = AUTOCLEAN_STAGE_FINISHED;		
		_abort = false;
		return;
	}
		


	switch (_currentStage)
	{
	case AUTOCLEAN_STAGE_START:
		StageStart();
		break;	
	case AUTOCLEAN_STAGE_PUSH_MOISTURE:
		StagePushMoisture();
		break;
	case AUTOCLEAN_STAGE_BUILD_PRESSURE:
		StageBuildPressure();
		break;
	case AUTOCLEAN_STAGE_FINISHING:
		StageFinishing();
		break;

	case AUTOCLEAN_STAGE_FINISHED:
	case AUTOCLEAN_STAGE_NONE:
		break;
	}
}

AutoCleanStage LFT_AutoClean::GetState()
{
	return _currentStage;
}

void LFT_AutoClean::SetStage(AutoCleanStage stage)
{
	_currentStage = stage;
}


int LFT_AutoClean::GetProgress()
{
	if (_finished)
		return 100;

	if (_currentStage < AUTOCLEAN_STAGE_PUSH_MOISTURE)
		return 0;

	long long startSeconds = ((DateTime)_totalStartTime).getRaw();
	if (startSeconds == 0)
		return 0;


	int estimateTime = ESTIMATE_FULL_CLEAN_SECONDS;	
		
	long long secondsElapsed = _information->GetCurrentTime().getRaw() - startSeconds;
	double decimal = ((double)secondsElapsed) / estimateTime;
	if (decimal > 0.99)
		decimal = 0.99;

	return (int)(decimal * 100);
}

bool LFT_AutoClean::GetAbortState()
{
	return _abort;
}

int LFT_AutoClean::GetStatus()
{
	return _status;
}

int LFT_AutoClean::ResetErrors()
{
	_status = 0;
	return 0;
}

void LFT_AutoClean::SetIsStandalone(bool value)
{
	_isStandalone = value;
}

bool LFT_AutoClean::GetIsStandalone()
{
	return _isStandalone;
}


void LFT_AutoClean::Abort()
{
	_abort = true;
}


void LFT_AutoClean::StageStart()
{	
	//Ensure not in standby
	_manual->SetStandbyState(false);	
	_manual->SetBaseFanState(true);
	msleep(500);
	
	//Set Pump running
	_manual->SetPumpState(true);
	msleep(500);

	//Set valves ready for next stage
	_manual->SetInletState(false);
	msleep(100);
	_manual->SetPurgeState(true);
	msleep(100);
	_manual->SetBypassState(false);
	msleep(100);
	_manual->SetBaseTemperature(35);	
	msleep(100);
	
	_currentStage = AUTOCLEAN_STAGE_PUSH_MOISTURE;
	_mainStartTime = _information->GetCurrentTime();
	_stageStartTime = _information->GetCurrentTime();
}


void LFT_AutoClean::StagePushMoisture()
{
	STime timeDifference = STime(_information->GetCurrentTime().getRaw() - _stageStartTime.getRaw());

	if (!_information->PurgeState)
	{
		_manual->SetPurgeState(true);
		msleep(100);
	}

	if (!_information->InletState)
	{
		_manual->SetInletState(true);
		msleep(100);
	}

	int waitTime = 150;

	//If stage is finished
	if (timeDifference.get() > waitTime)
	{
		STime totalTimeDifference = STime(_information->GetCurrentTime().getRaw() - _mainStartTime.getRaw());

		//If going to finish stage		
		if (totalTimeDifference.get() > 1200)
		{
			_currentStage = AUTOCLEAN_STAGE_FINISHING;						
		}
		else					
			_currentStage = AUTOCLEAN_STAGE_BUILD_PRESSURE;
		

		_stageStartTime = _information->GetCurrentTime();
	}

	
}

void LFT_AutoClean::StageBuildPressure()
{
	STime timeDifference = STime(_information->GetCurrentTime().getRaw() - _stageStartTime.getRaw());

	if (_information->PurgeState)
	{
		_manual->SetPurgeState(false);
		msleep(100);
	}
	if (_information->InletState)
	{
		_manual->SetInletState(false);
		msleep(100);
	}

	if (timeDifference.get() > 45)
	{		
		_currentStage = AUTOCLEAN_STAGE_PUSH_MOISTURE;
		_stageStartTime = _information->GetCurrentTime();
	}

}

void LFT_AutoClean::StageFinishing()
{	
	_manual->SetPumpState(false);
	msleep(500);

	_manual->SetInletState(true);
	msleep(100);
	_manual->SetPurgeState(true);
	msleep(100);
	_manual->SetBypassState(true);
	msleep(2000);



	_manual->SetPurgeState(false);
	msleep(100);
	_manual->SetInletState(false);
	msleep(100);


	_manual->SetStandbyState(true);

	_currentStage = AUTOCLEAN_STAGE_FINISHED;
	_finished = true;
}

void LFT_AutoClean::msleep(int ms)
{
#ifndef  SIMULATOR	
	vTaskDelay(ms);
#endif
}
