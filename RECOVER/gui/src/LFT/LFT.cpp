#include <gui/LFT/LFT.hpp>

//Static Declarations 
LFT_Information LFT::Information;
LFT_Settings LFT::Settings(&LFT::Information);
LFT_Manual LFT::Manual(&LFT::Information);
LFT_Logs LFT::Logs(&LFT::Information);
LFT_AutoClean LFT::AutoClean(&LFT::Information, &LFT::Manual, &LFT::Settings);
LFT_Auto LFT::Auto(&LFT::Information, &LFT::Manual, &LFT::Settings, &LFT::AutoClean);
LFT_ProductionTests LFT::ProductionTests(&LFT::Information, &LFT::Manual, &LFT::Auto, &LFT::Settings);
int LFT::progressContinueOverride(0);

Model* LFT::_model = 0;
bool LFT::_initalised = false;

void LFT::Initialise()
{
	if (_initalised)
		return;		
	
#ifndef SIMULATOR			
	xTaskCreate(LFTLoopThreadProcess,
		(TASKCREATE_NAME_TYPE)"LFT_VALUES",
		2400,
		NULL,
		3,
		NULL);	
#else
	LFTThreadProcess();

#endif // !SIMULATOR

	_initalised = true;
}

void LFT::SetModel(Model * model)
{
	_model = model;		
	Manual.SetModel(model);
	Auto.SetModel(model);
	AutoClean.SetModel(model);
	Logs.SetModel(model);
	Information.SetModel(model);
	ProductionTests.SetModel(model);
	Settings.SetModel(model);
}

void LFT::LFTLoopThreadProcess(void* params)
{
	while (true)
	{		
#ifndef  SIMULATOR
		vTaskDelay(TASK_WAIT_TIME);
#endif

		//If no model is set, continue;
		if (_model == 0)
			continue;

		LFTThreadProcess();		
	}

	//Tasks aren't supposed to exit, but if it does, delete it
#ifndef SIMULATOR
	vTaskDelete(NULL);
#endif
}

void LFT::LFTThreadProcess()
{	
	//Initialisation
	if (Information.RefreshValuesRequired)
	{
		Initialisation();
		Information.RefreshValuesRequired = false;
	}

	//If Log Fetch is qued
	if (Logs.GetLogsRequired)
		Logs.GetAllHeaders();
	if (Logs.GetSamplesRequired)
		Logs.GetAllSamples();
	//Export Logs if required
	if (Logs.ExportAllRequired)
		Logs.ExportAll();	

	//If Abort is qued
	if (Information.AbortRequired)
	{		
		//Abort Process
		Auto.Abort();				

		//Wait until process is aborted (Progress == 100)
		int progress;
		do
		{
			Information.ReadProgress(progress);	

#ifndef  SIMULATOR
			vTaskDelay(200);
#endif
		} while (progress != 100);

#ifndef  SIMULATOR
		vTaskDelay(200);
#endif		

		//Clean up from abort
		Auto.AbortCleanup();		

		//Clear Errors
		Auto.ClearErrors();
		Information.Status = 0;

		Information.AllowStageAutoProgression = true;

		Information.AbortRequired = false;
	}

	//If Auto Command's are qued, start them
	if (Information.PreChecksRequired)
	{
		Auto.SetSettings();
		Auto.StartPreChecks();
	}
	if (Information.ChamberConditioningRequired)
		Auto.StartChamberConditioning();
	if (Information.FumingRequired)
		Auto.StartFuming();
	if (Information.CoolRequired)
		Auto.StartCool();

	//If tests are qued, start them	
	if (ProductionTests.IsPumpdownTestQued)
	{
		ProductionTests.GraphPumpdown();
		ProductionTests.IsPumpdownTestQued = false;
	}
	if (ProductionTests.IsSoakTestQued)
	{
		ProductionTests.SoakTest();
		ProductionTests.IsSoakTestQued = false;
	}	
	if (ProductionTests.IsEMCModeOn)
		ProductionTests.EmcMode();	
	
	AutoClean.ProcessAutoClean();


	//Read General Information that should be kept up to date
	Information.ReadStandardValues(Auto.GetStage());

	if (Information.ProgressUpdating)	
		Information.ReadProgress();				
	
	//Check if next stage should be called
	CheckForNextStage();

	//Check if a PC has requested PC Mode
	CheckPCMode();
}

void LFT::Initialisation()
{
	_model->SendCommand("A");

	Information.RefreshValuesProgress = 0;

	Information.ReadBuild();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadSerialNumber();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadStandby();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadBaseHeater();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadPrecursorHeater();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadPump();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadBaseFan();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadStirringFan();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadInlet();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadPurge();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadBypass();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadChamberSize();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;
	
	Auto.SetChamberBeepReady(Settings.ReadBeepWhenReadySetting());
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;
	
	Texts::setLanguage(Settings.ReadLanguage());
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadRunCounter();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadFilterCounter();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadManifoldVersion();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Settings.ReadUsePurgeFans();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Settings.ReadLockMode();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	//Set PC mode to 0, if power reset then PC mode shouldn't be enabled
	Settings.SetPCMode(0);
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;
	
	Settings.ReadCiphers();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Information.ReadRTC();
	
	int recoveryState = Settings.ReadRecoveryState();
	if (recoveryState != LFT_STAGE_LID_CONTROL && recoveryState != LFT_STAGE_FINISHED)
		RecoverFromPowerloss();
	Information.RefreshValuesProgress = Information.RefreshValuesProgress + 1;

	Manual.Beep(120, 50, 5);
}

void LFT::CheckPCMode()
{
	//TODO: Check Able to enter PC mode
	int mode = _model->GetPCMode();
	if (mode == 1)
	{
		int stage = Auto.GetStage();
		if (stage == LFT_STAGE_LID_CONTROL || stage == LFT_STAGE_FINISHED)
		{
			//PC mode set to 2 as confirmed
			_model->SetPCMode(2);

			application().gotoUsbModeScreenNoTransition();
			do
			{
#ifndef  SIMULATOR
				vTaskDelay(500);
#endif		
				mode = _model->GetPCMode();

				//If PC disconnects, this will happen when user reconnects.
				if (mode == 1)
					_model->SetPCMode(2);


			} while (mode != 0);

			application().gotoIdleScreenScreenNoTransition();
		}
		else
		{
			//PC mode not allowed right now
			_model->SetPCMode(0);
		}
	}
}


void LFT::CheckForNextStage()
{	
	//If we're in chamber conditioning, then we must check the performance
	if (Auto.GetStage() == LFT_STAGE_CHAMBER_CONDITIONING && !Information.CheckPerformance())	
		Auto.StartDrying();


	//If progress is repeatedly reporting 100%, then allow stage auto progression is overriden
	if (Information.Progress == 100)
		progressContinueOverride++;
	else
		progressContinueOverride = 0;


	//If Auto-Progression is not allowed, return
	if (!Information.AllowStageAutoProgression && progressContinueOverride < CONTINUE_OVERRIDE_MAX)
		return;

	//For the precheck and cooldown stage, progress continues when 100% is reached
	if (Information.Progress == 100 && (Auto.GetStage() == LFT_STAGE_PRECHECKS || Auto.GetStage() == LFT_STAGE_COOLDOWN || Auto.GetStage() == LFT_STAGE_CHAMBER_CONDITIONING))
		GotoNextStage();	

	//For the chamber conditioning stage, progress continues when the correct pressure is reached
	//DONE THE NORMAL WAY!
	/*float vacValueFloat = (float)(Information.VAC_VALUE / 1000.0f);
	if (Auto.GetStage() == LFT_STAGE_CHAMBER_CONDITIONING && Information.Pressure > 0 && Information.Pressure <= vacValueFloat)
		GotoNextStage();*/
	
}

void LFT::GotoNextStage()
{	
	//Stop Progress updating (it will be started again when needed)
	Information.ProgressUpdating = false;
	
	//Stop Auto Stage Progression (It will be started again when needed)
	Information.AllowStageAutoProgression = false;

	//Reset Progress
	Information.Progress = 0;

	//Que Next Stage
	Auto.QueStage(Auto.GetStage() + 1);
}

void LFT::RecoverFromPowerloss()
{	
	int recoveryState = Settings.GetRecoveryState();
	//Not allowed into function if lid control or finished
	if (recoveryState == LFT_STAGE_LID_CONTROL || recoveryState == LFT_STAGE_FINISHED)
		return;


	//If uptime is less than 10 seconds, then firmware has been reset
	Information.ReadUptime();
	bool isFirmwareReset = ((DateTime)Information.Uptime).getRaw() < 10;		

	//Read Pressure
	Information.ReadPressure();

	//Show Powerloss Detected Box providing it's not the fuming state and less than 2.0T pressure!
	Information.PowerlossDetected = !(recoveryState == LFT_STAGE_FUMING && Information.Pressure < 2.0f);

	if (isFirmwareReset)
	{
		//Processes will often automatically reset to prechecks when safe to do since this can be cancelled if required.
		switch (recoveryState)
		{
			//If Conditioning or pre-checking just start at pre-checks again
			//Since pre-cursor element should be fine at this point and 
			//the stages set the state's they require
		case LFT_STAGE_PRECHECKS:
		case LFT_STAGE_CHAMBER_CONDITIONING:
		case LFT_STAGE_READY_TO_FUME:			
			Auto.QueStage(LFT_STAGE_PRECHECKS);
			application().gotoProcessScreenScreenNoTransition();			
			break;
			//If Fuming was stage before powerloss			
		case LFT_STAGE_FUMING:
			if (Information.Pressure < 2.0f)
			{
				//If pressure is low enough, allow user to decide if they want to recover
				application().gotoPowerlossRecoveryScreenNoTransition();				
			}
			else
			{
				//If pressure is too high then cooldown 
				Auto.QueStage(LFT_STAGE_COOLDOWN);
				application().gotoProcessScreenScreenNoTransition();
			}			
			break;

		case LFT_STAGE_COOLDOWN:			
			Auto.QueStage(LFT_STAGE_COOLDOWN);
			application().gotoProcessScreenScreenNoTransition();
			break;

			//Don't care if the process was Finished or Lid Control
		default:
			break;
		}
	}
	else
	{
		//Should be all that's required
		Auto.RecoverToStage(recoveryState);		
		application().gotoProcessScreenScreenNoTransition();
	}



}
