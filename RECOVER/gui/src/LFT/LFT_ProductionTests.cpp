#include <gui/LFT/LFT_ProductionTests.hpp>

LFT_ProductionTests::LFT_ProductionTests(LFT_Information* lft_information, LFT_Manual* lft_manual, LFT_Auto* lft_auto, LFT_Settings* lft_settings) :
	_information(lft_information),
	_manual(lft_manual),
	_auto(lft_auto),
	_settings(lft_settings),
	_model(0)	
{	
	//LFTActual[StatusMaximumPoints];
	for (int i = 0; i < StatusMaximumPoints; i++)
		LFTActual[i] = LFT_Status();


	SoakTestStatus.SetSemaphore(_information->xSemaphore);
	SoakTestCount.SetSemaphore(_information->xSemaphore);	
	IsPumpdownTestQued.SetSemaphore(_information->xSemaphore);
	IsSoakTestQued.SetSemaphore(_information->xSemaphore);	
	maxBaseCurrent.SetSemaphore(_information->xSemaphore);
	maxPreCurrent.SetSemaphore(_information->xSemaphore);
	LFT_ACTUAL_LENGTH.SetSemaphore(_information->xSemaphore);
	_isAborted.SetSemaphore(_information->xSemaphore);	
	IsEMCModeOn.SetSemaphore(_information->xSemaphore);
	GraphPumpDownTime.SetSemaphore(_information->xSemaphore);
}

void LFT_ProductionTests::SetModel(Model * model)
{
	_model = model;
}

void LFT_ProductionTests::AbortProductionTest()
{
	_isAborted = true;
}

bool LFT_ProductionTests::GetProductionTestAborted()
{
	return _isAborted;
}


void LFT_ProductionTests::QueGraphPumpdown()
{
	IsPumpdownTestQued = true;
}

void LFT_ProductionTests::GraphPumpdown()
{	
	LFT_ACTUAL_LENGTH = 0;
	int nextRecordedSecond = 0;

	_isAborted = false;

	//Disable Standby
	_manual->SetStandbyState(false);
	_manual->SetPumpState(false);

	//Return to atmos
	ReturnToAtmos();

	//Setup time so that it is ready for taking measurements, and so that the elapsed time can be seen on screen.	
	DateTime startTime = _information->GetCurrentTime();
	_information->ResetTimer();

	//Check Base Maximum Current
	BaseCurrentCheck();
	//Precursor Current fake (Since pre-cursor can't turn on until pressure is down)
	maxPreCurrent = 0.001f;


	//Set Valves to 0|1|0
	_manual->SetInletState(false);
	_manual->SetPurgeState(true);
	_manual->SetBypassState(false);

	//Turn on pump and base heater
	_manual->SetPumpState(true);
	_manual->SetBaseTemperature(35);	

	//Wait for either timeout, or pressure is at 0.8Torr, or Aborted
	while (LFT_ACTUAL_LENGTH < StatusMaximumPoints && _information->Pressure >= 0.75f && !_isAborted)
	{
		//Update Values
		_information->ReadBaseTemperature();
		_information->ReadBasePowerPercentage();
		_information->ReadPreTemperature();
		_information->ReadPrePowerPercentage();
		_information->ReadPressure();		
		_information->ReadInternalSwitch1();
		_information->ReadInternalSwitch2();
		_information->ReadExternalSwitch();

		//Swap to bypass at 15Torr
		if (_information->Pressure <= 15 && _information->BypassState == false)
		{
			_manual->SetPurgeState(false);
			_manual->SetBypassState(true);
		}

		//Record when due (Every 10 seconds)
		STime differenceTimeData(_information->GetCurrentTime().getRaw() - startTime.getRaw());	
		if (differenceTimeData.get() >= nextRecordedSecond)
		{
			LFTActual[LFT_ACTUAL_LENGTH] =
			{
				(short)nextRecordedSecond,
				_information->Pressure,
				(short)_information->BaseTemp,
				(short)_information->BasePower,
				(short)_information->PreTemp,
				(short)_information->PrePower
			};
			LFT_ACTUAL_LENGTH = LFT_ACTUAL_LENGTH+1;
			nextRecordedSecond += 10;

			GraphPumpDownTime = STime(_information->GetCurrentTime().getRaw() - startTime.getRaw());
		}				

		//Wait to stop overloading
#ifndef  SIMULATOR
		vTaskDelay(500);
#endif

	}
	
	GraphPumpDownTime = STime(_information->GetCurrentTime().getRaw() - startTime.getRaw());	

	//Close purge and turn off pump
	_manual->SetPumpState(false);
	_manual->SetBypassState(false);
}

void LFT_ProductionTests::QueSoakTest()
{
	IsSoakTestQued = true;
}

void LFT_ProductionTests::SoakTest()
{
	_auto->ClearErrors();
	SoakTestCount = 0;
	SoakTestStatus = 1;

	_isAborted = false;
	while (!_isAborted)
	{		
		//Put lid down until switch triggered
		_manual->LidDown();		
		SoakTestStatus = 2;
		do
		{
#ifndef  SIMULATOR
			vTaskDelay(500);
#endif
			_information->ReadStandardValues(_auto->GetStage());
			_information->ReadLidState();
			_information->ReadStatus();

			if (_isAborted || _information->Status != 0)
			{
				_manual->LidStop();
				return;
			}

		} while (!_information->LidClosedState);

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Set Settings
		_auto->SetSettings();		
		_auto->SetChamberSize(false);
		_auto->SetMetalType(false);
		_auto->SetStirTime(1);				

		//Start Prechecks
		_auto->StartPreChecks();
		SoakTestStatus = 3;


#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Wait for 100%
		do
		{
#ifndef  SIMULATOR
			vTaskDelay(500);
#endif
			_information->ReadStandardValues(_auto->GetStage());
			_information->ReadProgress();
			_information->ReadStatus();

			if (_isAborted || _information->Status > 0)
			{
				_auto->Abort();
				return;
			}

		} while (_information->Progress != 100);

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Start Chamber Conditioning
		_auto->StartChamberConditioning();
		SoakTestStatus = 4;

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Wait for pressure under 0.75Torr
		do
		{
#ifndef  SIMULATOR
			vTaskDelay(500);
#endif

			_information->ReadStandardValues(_auto->GetStage());

			if (_isAborted || _information->Status > 0)
			{
				_auto->Abort();
				return;
			}			
		} while (_information->Pressure >= 0.75f && _information->Pressure != -1);

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Start Fuming		
		DateTime fumingStart = _information->GetCurrentTime();
		_auto->StartFuming();
		SoakTestStatus = 5;

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Wait for 240 seconds to pass
		STime differenceTimeData;
		do
		{
#ifndef  SIMULATOR
			vTaskDelay(500);
#endif
			_information->ReadStandardValues(_auto->GetStage());
			differenceTimeData = STime(_information->GetCurrentTime().getRaw() - fumingStart.getRaw());
			if (differenceTimeData.get() >= 240)
				break;
			_information->ReadStatus();

			if (_isAborted || _information->Status > 0)
			{
				_auto->Abort();
				return;
			}

		} while (true);

#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif

		//Start Cooling
		_auto->StartCool();
		SoakTestStatus = 6;
		
#ifndef  SIMULATOR
		vTaskDelay(2000);
#endif
		//Wait for 100%
		do
		{
#ifndef  SIMULATOR
			vTaskDelay(500);
#endif
			_information->ReadProgress();
			_information->ReadStandardValues(_auto->GetStage());

			if (_information->Status > 0)
			{
				_auto->Abort();
				return;
			}

		} while (_information->Progress != 100);

		//Allow Abort now so user doesn't have to wait 5 minutes for cooling
		if (_isAborted)
		{
			_auto->Abort();
			return;
		}


		//Wait for cooling (5 Minutes)
		DateTime lidStart = _information->GetCurrentTime();		
		do {

#ifndef  SIMULATOR
			vTaskDelay(500);
#endif
			_information->ReadStandardValues(_auto->GetStage());
			differenceTimeData = STime(_information->GetCurrentTime().getRaw() - lidStart.getRaw());
			

		} while (differenceTimeData.get() < 300);

		SoakTestCount = ((int)SoakTestCount) + 1;
	}

	_auto->Abort();
}

void LFT_ProductionTests::QueEMCMode()
{
	IsEMCModeOn = true;
	lidCounter = 200;

}

void LFT_ProductionTests::EmcMode()
{
	lidCounter++;
	if (lidCounter < 20)
		return;

	lidCounter = 0;

	if (lidDirection)
		_manual->ForceLidUp();		
	else
		_manual->ForceLidDown();

	lidDirection = !lidDirection;
}

void LFT_ProductionTests::ReturnToAtmos()
{

	//Turn off heaters (If they're not on, nothing will happen)
	_manual->SetPreTemperature(_information->HIGHTEMP_OFF);
	_manual->SetBaseTemperature(_information->LOWTEMP_OFF);


	do
	{
#ifndef  SIMULATOR
		vTaskDelay(200);
#endif
		
		_information->ReadPreTemperature();

		//Wait until pre temperature is below 70
	} while (_information->PreTemp > 70);


	//Open all valves
	_manual->SetInletState(true);
	_manual->SetPurgeState(true);
	_manual->SetBypassState(true);


	do
	{
#ifndef  SIMULATOR
		vTaskDelay(200);
#endif
		
		_information->ReadPressure();
		_information->ReadInternalSwitch1();
		_information->ReadInternalSwitch2();

		//Wait until atmos
	} while (_information->Pressure < 20.0f || _information->InternalSwitchValue1 != 1);

	//Wait 10 seconds (Just to make sure that pressure is fully up)
#ifndef  SIMULATOR
	vTaskDelay(10000);
#endif

	//Close all valves
	_manual->SetInletState(false);
	_manual->SetPurgeState(false);
	_manual->SetBypassState(false);

}

int LFT_ProductionTests::GetStatusValue(GraphType type, LFT_Status status)
{
	switch (type)
	{
	case Graph_Pressure:
		return (int)(status.Pressure * 10);
	case Graph_BaseTemp:
		return status.BaseTemp;
	case Graph_BasePower:
		return status.BasePower;
	case Graph_PreTemp:
		return status.PreTemp;
	case Graph_PrePower:
		return status.PrePower;
	default:
		return 0;
	}
}

void LFT_ProductionTests::BaseCurrentCheck()
{
	_manual->SetBaseTemperature(50);
	float maxCurrentPerRead = 0;
	float voltage, current;
	int reads = 0;
	//Get the maximum current of 10 reads
	while (reads < 10)
	{
		_information->ReadBasePower(voltage, current);

		if (current > maxCurrentPerRead)
			maxCurrentPerRead = current;

		reads++;
	}

	_manual->SetBaseTemperature(_information->LOWTEMP_OFF);
	maxBaseCurrent = maxCurrentPerRead;
}

void LFT_ProductionTests::PreCurrentCheck()
{
	_manual->SetBaseTemperature(190);
	float maxCurrentPerRead = 0;
	float voltage, current;
	int reads = 0;
	//Get the maximum current of 10 reads
	while (reads < 10)
	{
		_information->ReadPrecursorPower(voltage, current);

		if (current > maxCurrentPerRead)
			maxCurrentPerRead = current;

		reads++;
	}

	_manual->SetBaseTemperature(_information->LOWTEMP_OFF);
	maxPreCurrent = maxCurrentPerRead;

}

