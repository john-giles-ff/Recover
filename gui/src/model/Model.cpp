#include <gui/model/Model.hpp>

Model* Model::model = 0;

Model::Model() : modelListener(0)
{				
#ifndef SIMULATOR
	/* Create a mutex type semaphore. */
	xSemaphore = xSemaphoreCreateMutex();
#endif

	CreateTask();
}

void Model::SetBacklight(float pwm_value)
{
#ifndef SIMULATOR
	if (pwm_value > 100)
	{
		pwm_value = 100;
	}
	pwm_value = 100 - pwm_value;
	setPWM(htim12, TIM_CHANNEL_2, 100, pwm_value);

#endif
}

void Model::SendAndRead(unsigned char command[], int commandSize, unsigned char *result, int& resultLength, int timeout)
{
	attempts++;
	
	//Make sure command ends with new line
	if (command[commandSize - 1] != 0x0A)
	{
		command[commandSize] = 0x0A;
		commandSize++;
	}
			
#ifndef SIMULATOR	
	//Take Mutex	
	if (xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE)
		return;

	int comStatus = 0;


	//Send Command
	comStatus = HAL_UART_Transmit(&huart7, command, commandSize, 150);
	if (comStatus != 0)
	{
		error = 1000 + comStatus;
		errors++;
		ResetComms();
		xSemaphoreGive(xSemaphore);
		return;
	}


	//Loop Reading until a new line / error happens
	resultLength = 0;
	
	while (true)
	{		
		//Read 1 byte, from device
		unsigned char byte[1]{ 0x00 };				
		comStatus = HAL_UART_Receive(&huart7, byte, 1, timeout);

		//If failed to read, return error code or return what you've already got
		if (comStatus != 0)
		{
			error = comStatus;
			errors++;
			ResetComms();
			break;
		}
		if (byte[0] == 0x00)
		{
			error = -99;			
			errors++;
			ResetComms();
			break;
		}			


		//Save the byte to the buffer, 
		result[resultLength] = byte[0];
		if (byte[0] == 0x0A)
			break;

		//If it exceeds the buffer, continue
		resultLength++;
		if (resultLength > 255)
			break;

	}		

	//Release Mutex
	xSemaphoreGive(xSemaphore);

	reads++;

#else	
	
	//Test Values
	resultLength = 6;
	result[0] = 0x31;
	result[1] = 0x35;
	result[2] = 0x33;
	result[3] = 0x38;
	result[4] = 0x35;
	result[5] = 0x0A;
	

#endif
	
}

void Model::SendBool(String command, bool state, int timeout)
{			
	if (command.back() != ' ')
		command += " ";

	if (state)
		command += "1";
	else
		command += "0";

	SendCommand(command, timeout);
}

void Model::SendInt(String command, int value, int timeout)
{
	if (command.back() != ' ')
		command += " ";

	command += String(value);

	SendCommand(command, timeout);
	
}

void Model::SendCommand(String command, int timeout)
{
	if (command.back() != '\n')
		command += "\n";

#ifndef SIMULATOR	
	unsigned char commandData[50];
	strcpy((char*)commandData, command.c_str());

	int resultLength = 0;
	unsigned char result[255];
	SendAndRead(commandData, command.len(), result, resultLength, timeout);
#else		
	//touchgfx_printf("%s", command.c_str());
#endif
}

String Model::ReadString(String command, int timeout)
{
	if (command.back() != '\n')
		command += "\n";
	
#ifndef SIMULATOR
	unsigned char commandData[50];
	strcpy((char*)commandData, command.c_str());

	int length = 0;
	unsigned char resultData[256];
	SendAndRead(commandData, command.len(), resultData, length, timeout);

	if (length == 0)	
		return "-1";	
	if ((resultData[0] == 0x57 && resultData[1] == 0x48) || (resultData[0] == 0x77 && resultData[1] == 0x68))			
		return "-2";		

	return String(reinterpret_cast<const char*>(resultData), length);
#else 		
	return "0";
#endif

}

int Model::ReadInt(String command, int timeout)
{
	return ReadString(command, timeout).toInt();
}

float Model::ReadFloat(String command, int timeout)
{
	return ReadString(command, timeout).toFloat();	
}

DateTime Model::ReadTime(String command, int timeout)
{	
	String result = ReadString(command, timeout);	

	if (result == "-2" || result == "-1" || result == "0")
		return DateTime();

	return result.toDateTime();
}

bool Model::ReadBool(String command, int timeout)
{
	return ReadInt(command, timeout) == 1;
}

void Model::CreateTask()
{
	Model::model = this;
#ifndef SIMULATOR		
	
	xTaskCreate(LoopUpdateValues,
		(TASKCREATE_NAME_TYPE)"LFT_VALUES",
		2400,
		NULL,
		3,
		NULL);		
	//vTaskStartScheduler();	

#else
	UpdateLFTValues();

#endif // !SIMULATOR

}

void Model::UpdateLFTValues()
{
	//Refresh Loop
	if (_refreshValuesRequired)
	{
		//Send a garbage command to flush out LFT buffer in the case that the LCD has power-cycled and the board hasn't
		SendCommand("A");		
		
		//Set Values Progress to 0
		_refreshValuesProgress = 0;

		ReadBuild();
		_refreshValuesProgress++;

		ReadStandby();
		_refreshValuesProgress++;

		ReadBaseHeater();
		_refreshValuesProgress++;

		ReadPrecursorHeater();
		_refreshValuesProgress++;
				
		ReadPump();
		_refreshValuesProgress++;

		ReadBaseFan();
		_refreshValuesProgress++;

		ReadStirringFan();
		_refreshValuesProgress++;
		
		//Mostly an alarm to fetch attention when a flash has finished
		Beep(120, 50, 5);
	}	

	if (_refreshValuesRequired || isEngineerModeOn)
	{
		ReadInlet();		
		if (_refreshValuesRequired)
			_refreshValuesProgress++;

		ReadPurge();		
		if (_refreshValuesRequired)
			_refreshValuesProgress++;

		ReadBypass();
		if (_refreshValuesRequired)
			_refreshValuesProgress++;

		_refreshValuesRequired = false;
	}

	//Auto Commands
	if (_preChecksRequired)
	{
		_preChecksRequired = false;
		StartPreChecks();
		
		//Comms must be reset before continuing
		ResetComms();		

		//Update the progress value
		_progressUpdating = true;
	}
	
	if (_pumpdownRequired)
	{
		_pumpdownRequired = false;
		StartPumpDown(PUMPDOWN_TIMEOUT);		

		//Comms must be reset before continuing
		ResetComms();

		//Update the progress value
		_progressUpdating = true;
	}
	
	if (_heatRequired)
	{
		_heatRequired = false;
		StartHeat(HEAT_TIMEOUT);
		
		//Comms must be reset before continuing
		ResetComms();

		//Update the progress value
		_progressUpdating = true;		
	}	

	if (_coolRequired)
	{
		_coolRequired = false;
		StartCool();
		
		//Comms must be reset before continuing
		ResetComms();

		//Update the progress value
		_progressUpdating = true;

	}
	
	//General Information
	ReadBaseTemperature();
	ReadPreTemperature(); 
	ReadPressure();		
	ReadInternalSwitch1();
	ReadInternalSwitch2();
	ReadDoorSwitch();
	ReadStatus();
	ReadRTC();		

	if (_progressUpdating)
		ReadProgress();
}

void Model::SetStandbyState(bool state)
{
	_standbyMode = state;
	SendBool("STANDBY", state);	
}

void Model::SetInletState(bool state)
{
	_inletState = state;	
	SendBool("INLET", state);
}

void Model::SetPurgeState(bool state)
{
	_purgeState = state;	
	SendBool("PURGE", state);
}

void Model::SetBypassState(bool state)
{
	_bypassState = state;	
	SendBool("BYPASS", state);
}

void Model::SetPumpState(bool state)
{
	_pumpState = state;
	SendBool("PUMP", state);	
}

void Model::SetBaseFanState(bool state)
{
	_baseFanState = state;
	SendBool("FAN", state);	
}

void Model::SetStiringFansState(bool state)
{
	_stiringFansState = state;	
	SendBool("STIRFAN", state);	
}

void Model::SetBaseTemperature(int temperature)
{
	_baseHeaterSetpoint = temperature;			
	SendInt("SETBASE", temperature);
}

void Model::SetPreTemperature(int temperature)
{
	_precursorHeaterSetpoint = temperature;	
	SendInt("SETPRE", temperature);	
}

void Model::SetBaseTemperatureSetting()
{		
	SendInt("BASETEMP", LOWTEMP_ON);	
}

void Model::SetPreTemperatureSetting()
{
	SendInt("PRETEMP", HIGHTEMP_ON);
}

void Model::QueRefreshValues()
{
	_refreshValuesRequired = true;
}

void Model::QuePrechecks()
{
	ClearErrors();

	if (_progress == 100)
		_progress = 0;

	_preChecksRequired = true;
}

void Model::QuePumpdown()
{
	if (_progress == 100)
		_progress = 0;

	_pumpdownRequired = true;
}

void Model::QueHeat()
{
	if (_progress == 100)
		_progress = 0;

	_heatRequired = true;
}

void Model::QueCool()
{
	if (_progress == 100)
		_progress = 0;

	_coolRequired = true;
}

void Model::StartPreChecks()
{	
	SendCommand("INITIALISE");	
}

void Model::StartPumpDown(int timeout)
{
	SendCommand("PUMPDOWN");
	//SendInt("PUMPDOWN", timeout);
}

void Model::StartHeat(int timeout)
{
	SendCommand("HEAT");
	//SendInt("HEAT", timeout);	
}

void Model::StartCool()
{
	SendCommand("COOL");
}

void Model::BreakSeal(int time)
{
	SendInt("BREAKSEAL", time);	
}

void Model::ResetTimer()
{
	_timerStart = _rtcTime;
}

void Model::Beep(int pitch, int length, int repeat)
{
	String command("BEEP ");
	command += String(pitch) + " " + String(length);

	if (repeat != 0)
		command += " " + String(repeat);

	int timeout = length + 100;

	if (repeat != 0)
		timeout = timeout * repeat;
	
	SendCommand(command, timeout);

}

void Model::Abort()
{	
	SendCommand("HALT");
	_standbyMode = true;
}

void Model::ClearErrors()
{	
	SendCommand("CLEAR");	
	_status = 0;
}

void Model::LidDown()
{
	SendCommand("LIDDOWN");
}

void Model::LidUp()
{
	SendCommand("LIDUP");	
}

void Model::LidStop()
{
	//Before stopping the lid, check if it's already stopped
	ReadLidState();
	SendCommand("LIDSTOP");	
}

void Model::ReadBuild()
{		
	_build = ReadInt("BUILD");			
}

void Model::ReadBaseTemperature()
{	
	_baseTemp = ReadInt("READBASE");
}

void Model::ReadPreTemperature()
{	
	_preTemp = ReadInt("READPRE");	
}

void Model::ReadPressure()
{
	_pressure = ReadFloat("READPRESSURE");	
}

void Model::ReadRTC()
{		
	_rtcTime = ReadTime("READRTC");
}

void Model::ReadTime()
{
	_time = ReadTime("READTIME");	
}

void Model::ReadProgress()
{
	_progress = ReadInt("PROGRESS");

	if (_progress == 100)
		_progressUpdating = false;
}

void Model::ReadStatus()
{
	_status = ReadInt("STATUS");	
}

void Model::ReadInternalSwitch1()
{
	_internalSwitchValue1 = ReadInt("PSW1");	
}

void Model::ReadInternalSwitch2()
{
	_internalSwitchValue2 = ReadInt("PSW2");	
}

void Model::ReadDoorSwitch()
{
	_doorSwitchValue = ReadInt("DOORSW");	
}

void Model::ReadLidState()
{
	_lidClosedState = !ReadBool("LID");	
}

void Model::ReadBaseHeater()
{
	_baseHeaterSetpoint = ReadInt("READBASEHEATER");
}

void Model::ReadPrecursorHeater()
{
	_precursorHeaterSetpoint = ReadInt("READPRESET");
}

void Model::ReadInlet()
{
#ifndef SIMULATOR
	String result = ReadString("READINLET", 2000);	
#else
	String result("1, 368");
#endif
	_inletState = String(result.substr(0, 1)).toInt() == 1;
}

void Model::ReadPurge()
{
	String result = ReadString("READPURGE", 2000);
	_purgeState = String(result.substr(0, 1)).toInt() == 1;
}

void Model::ReadBypass()
{
	String result = ReadString("READBYPASS", 2000);
	_bypassState = String(result.substr(0, 1)).toInt() == 1;
}

void Model::ReadPump()
{
	String result = ReadString("READPUMP", 2000);
	_pumpState = String(result.substr(0, 1)).toInt() == 1;
}

void Model::ReadBaseFan()
{	
	_baseFanState = ReadBool("READBASEFAN");
}

void Model::ReadStirringFan()
{
	_stiringFansState = ReadBool("READSFAN");
}

void Model::ReadStandby()
{
	_standbyMode = ReadBool("READSTANDBY");
}

void Model::LoopUpdateValues(void* params)
{
	for ( ;; )
	{					
#ifndef  SIMULATOR
		vTaskDelay(TASK_WAIT_TIME);
#endif

		model->UpdateLFTValues();
	}

	//Tasks aren't supposed to exit, but if it does, delete it
#ifndef SIMULATOR
	vTaskDelete(NULL);
#endif
}

void Model::ResetComms()
{
#ifndef SIMULATOR
	HAL_UART_DeInit(&huart7);
	HAL_UART_Init(&huart7);
#endif
}

int Model::ReadError(int errorNumber)
{
	String command = "ERROR ";
	command += String(errorNumber);	
	return ReadInt(command);
}

void Model::UpdateLFTDebug(LFTDebug * control)
{
	//Get Difference from time
	STime differenceTimeData(_rtcTime.getRaw() - _timerStart.getRaw());

	//Change setpoints off values to 0
	int preSetpoint = _precursorHeaterSetpoint;
	int baseSetpoint = _baseHeaterSetpoint;
	if (preSetpoint == 40)
		preSetpoint = 0;
	if (baseSetpoint == 29)
		baseSetpoint = 0;

	control->SetHideSensitiveValues(_sensitiveDataHidden);
	control->SetEnabled(_isInformationCurrent);
	control->SetBuild(_build);
	control->SetBaseTemperature(baseSetpoint, _baseTemp);
	control->SetPrecursorTemperature(preSetpoint, _preTemp);
	control->SetPressure(_pressure);
	control->SetInternalSwitch(_internalSwitchValue1, _internalSwitchValue2);
	control->SetDoorSwitch(_doorSwitchValue);
	control->SetStatus(_status);
	control->SetProgress(_progress);
	control->SetAttempts(attempts);
	control->SetErrorCount(errors);
	control->SetLastError(error);
	control->SetTimer(differenceTimeData.GetHour(), differenceTimeData.GetMinute(), differenceTimeData.GetSecond());
	control->SetValves(_inletState, _purgeState, _bypassState);
}