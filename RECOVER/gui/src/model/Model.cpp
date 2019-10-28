#include <gui/model/Model.hpp>

Model::Model() : modelListener(0)
{						
#ifndef SIMULATOR
	/* Create a mutex type semaphore. */
	xSemaphore = xSemaphoreCreateMutex();		
#endif		

	error.SetSemaphore(xSemaphore);
	errors.SetSemaphore(xSemaphore);

	
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
		errors = errors + 1;
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
			errors = errors + 1;
			ResetComms();
			break;
		}
		if (byte[0] == 0x00)
		{
			error = -99;			
			errors = errors + 1;
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

void Model::SendCommand(String command, int timeout)
{
	if (command.back() != '\n')
		command += "\n";

#ifndef SIMULATOR	
	unsigned char commandData[50];
	strcpy((char*)commandData, command.c_str());

	int resultLength = 0;
	unsigned char result[255];

	int errorsBefore = errors;
	int attempts = 0;
	do
	{
		//Reset errors
		errors = errorsBefore;

		//Attempt Send+Read
		SendAndRead(commandData, command.len(), result, resultLength, timeout);

		attempts++;
		//Don't allow exit while errors has changed and attempts below 4
	} while (errorsBefore != errors && attempts <= 4);
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

	int errorsBefore = errors;
	int attempts = 0;
	do
	{
		//Reset the errors
		errors = errorsBefore;

		//Try and Send+Read
		SendAndRead(commandData, command.len(), resultData, length, timeout);

		//Increase Attempts
		attempts++;

		//Don't allow exit while errors has changed and attempts below 4
	} while (errorsBefore != errors && attempts <= 4);

	//Returned nothing (Timeout Normally)
	if (length == 0)
		return "-1";

	//Returned WHAT IS (Happens when 2 commands are sent simulataneously)
	if ((resultData[0] == 0x57 && resultData[1] == 0x48) || (resultData[0] == 0x77 && resultData[1] == 0x68))
		return "-2";

	return String(reinterpret_cast<const char*>(resultData), length);
#else 		
	return "0";
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

String Model::SendIntReadString(String command, int value, int timeout)
{
	if (command.back() != ' ')
		command += " ";

	command += String(value);

	return ReadString(command, timeout);
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


int Model::MountUSB()
{
	int res = 0;
#ifndef SIMULATOR							
	//HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_SET);


	MX_USB_DEVICE_DeInit();			
	
	vTaskDelay(50);	
	

	MX_USB_HOST_Init();	
	MX_FATFS_Init();

	int startTick = HAL_GetTick();

	bool deviceConnected = false;
	do
	{
		vTaskDelay(50);

		//Timeout after 12000 ticks
		if (startTick + 12000 < HAL_GetTick())
		{
			return 789;
		}

	} //Wait until timeout or USB is ready
	while (GetUsbState() != 2);
	
		
	res = f_mount(&USBHFatFS, "", 0);	
	
#endif
	return res;
}

int Model::UnmountUSB()
{
#ifndef SIMULATOR
	f_mount(0, "", 0);
		
	vTaskDelay(50);	

	//Change back to being a host	
	MX_USB_HOST_DeInit();			

	vTaskDelay(500);	
	
	MX_USB_DEVICE_Init();
	
	
#endif
	return 0;
}

int Model::WriteLog(RecoverLog * log)
{
	int res = 0;

#ifndef SIMULATOR					
	STime startTime(log->StartTime.getRaw());
	String date =
		Utils::PadL('0', String(startTime.GetYear()), 4) + "-" +
		Utils::PadL('0', String(startTime.GetMonth() + 1), 2) + "-" +
		Utils::PadL('0', String(startTime.GetDay()), 2) + "-" +
		Utils::PadL('0', String(startTime.GetHour()), 2) + "-" +
		Utils::PadL('0', String(startTime.GetMinute()), 2) + "-" +
		Utils::PadL('0', String(startTime.GetSecond()), 2);
	String filename = date + ".recover";
						
	uint16_t buffer[256];
	int length = 0;
	log->GetData(buffer, &length);

	UINT bytesWritten = 0;
	res = f_open(&USBHFile, filename.c_str(), FA_CREATE_ALWAYS | FA_WRITE);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_SET);
	if (res == 0)
	{						
		//Length has to be doubled since it's length in bytes
		res = f_write(&USBHFile, buffer, length * 2, &bytesWritten);

		f_close(&USBHFile);
	}		
#else
	//for (int i = 0; i < DATA_COUNT; i++)
		//touchgfx_printf("%s", dataLines[i].c_str());
#endif	

	return res;
}

void Model::SetRTC(int year, int month, int day, int hour, int minute, int second)
{
	if (year < 2000)
		year = 2000;

#ifndef SIMULATOR
	RTC_TimeTypeDef time;
	time.Hours = hour;
	time.Minutes = minute;
	time.Seconds = second;
	time.SubSeconds = 0;
	time.TimeFormat = RTC_HOURFORMAT_24;
	time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	time.StoreOperation = RTC_STOREOPERATION_RESET;

	HAL_RTC_SetTime(&hrtc, &time, FORMAT_BIN);

	RTC_DateTypeDef date;
	date.Year = year - 2000;
	date.Month = month;
	date.Date = day;
	date.WeekDay = 0;
	HAL_RTC_SetDate(&hrtc, &date, FORMAT_BIN);

#endif	
}

void Model::GetRTC(int * year, int * month, int * day, int * hour, int * minute, int * second)
{	
#ifndef SIMULATOR
	//Get Time
	RTC_TimeTypeDef currentTime;
	RTC_DateTypeDef currentDate;

	//Get time
	HAL_RTC_GetTime(&hrtc, &currentTime, FORMAT_BIN);

	//Get Date has to be done in order to unlock the values!
	HAL_RTC_GetDate(&hrtc, &currentDate, FORMAT_BIN);


	//Return Time
	*year = 2000 + currentDate.Year;
	*month = currentDate.Month;
	*day = currentDate.Date - 2;


	*hour = (int)currentTime.Hours;
	*minute = (int)currentTime.Minutes;
	*second = (int)currentTime.Seconds;
#else
	
	*year = simYear;
	*month = simMonth;
	*day = simDay;
	*hour = simHour;
	*minute = simMinute;
	*second = simSecond;
	
#endif	
}

void Model::ResetComms()
{
#ifndef SIMULATOR
	HAL_UART_DeInit(&huart7);
	HAL_UART_Init(&huart7);
#endif
}

void Model::tick()
{		
}

void Model::Test()
{
#ifndef SIMULATOR
	MX_USB_DEVICE_DeInit();
	MX_USB_HOST_Init();
#endif


}

void Model::SetLED(bool state)
{
	if (state)
#ifndef SIMULATOR
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_RESET);
#else
		touchgfx_printf("LED ON\n");
#endif	
	else
#ifndef SIMULATOR
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_8, GPIO_PIN_SET);
#else
		touchgfx_printf("LED OFF\n");
#endif

}

int Model::GetUsbState()
{
#ifndef SIMULATOR
	return GET_USB_STATE();
#else
	return 0;
#endif
}

int Model::GetPCMode()
{
#ifndef SIMULATOR	
	return (int)GetPCMode_C();
#endif	

	return 0;
}

void Model::SetPCMode(int pcMode)
{
#ifndef SIMULATOR
	SetPCMode_C(pcMode);
#endif
}
