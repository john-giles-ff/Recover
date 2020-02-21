#include <gui/LFT/LFT_Logs.hpp>

LFT_Logs::LFT_Logs(LFT_Information* information) :
	_model(0),
	_information(information)
{	

	GetSamplesRequired.SetSemaphore(_information->xSemaphore);
	GetLogsRequired.SetSemaphore(_information->xSemaphore);
	ExportAllRequired.SetSemaphore(_information->xSemaphore);
	ExportLogsResult.SetSemaphore(_information->xSemaphore);
	GetLogsProgress.SetSemaphore(_information->xSemaphore);
	quedLogIndex.SetSemaphore(_information->xSemaphore);
	quedSampleCount.SetSemaphore(_information->xSemaphore);
	sampleLoadedAborted.SetSemaphore(_information->xSemaphore);

}

void LFT_Logs::SetModel(Model * model)
{
	_model = model;
}



RecoverLog LFT_Logs::GetHeader(int index)
{		
	//Attempt read 10 times
	for (int i = 0; i < 10; i++)
	{
		String result = _model->SendIntReadString("READHEADER", index);

		if (result[0] == '-' && result[1] == '1')
			break;

		//If mis-read then try again
		if (result.index('\t') == -1 || result.index('\0') != -1)
			continue;

		return RecoverLog(result, index);
	}

	return RecoverLog();	
}

bool LFT_Logs::IsNextLogPresent()
{
#ifdef SIMULATOR
	return true;
#endif

	//Read Current Index
	int logIndex = _model->ReadInt("LOGINDEX");
	
	//Find next index
	logIndex++;	
	if (logIndex > 63)
		logIndex = 0;

	//Return if a header for the log index exists
	RecoverLog header = GetHeader(logIndex);

	return header.Exists && header.HeatRunTime > 0;
}

Sample LFT_Logs::GetSample(int logIndex, int sampleIndex)
{
	String command = "READSAMPLE " + String(logIndex) + " " + String(sampleIndex);
	String result = _model->ReadString(command);

	//If no sample, return sample where Exists = false
	if (result.index('\t') == -1)
	{		
		return Sample();
	}

	return Sample(result);	
}

bool LFT_Logs::sortHeaders(RecoverLog a, RecoverLog b)
{
return a.StartTime.getRaw() > b.StartTime.getRaw();

}

void LFT_Logs::QueGetAllSamples(int logIndex, int sampleCount)
{
	sampleLoadedAborted = false;
	quedLogIndex = logIndex;
	quedSampleCount = sampleCount;
	GetSamplesRequired = true;
}

void LFT_Logs::GetAllSamples(int logIndex, int sampleCount)
{
	//If Values are default, fill them with the qued values
	if (logIndex == -1)
	{
		logIndex = quedLogIndex;
	}
	if (sampleCount == -1)
	{
		sampleCount = quedSampleCount;
	}

	//Only possible to have at maximum 1000 samples
	if (sampleCount > SAMPLE_MAX)
		sampleCount = SAMPLE_MAX;


	LoadedSampleCount = 0;
	for (int i = 0; i < sampleCount; i++)
	{
		if (sampleLoadedAborted)
			break;

		LoadedSamples[LoadedSampleCount] = GetSample(logIndex, i);

		//If sample exists, increase count of samples
		if (LoadedSamples[LoadedSampleCount].Exists)
			LoadedSampleCount++;
	}

	GetSamplesRequired = false;


}

void LFT_Logs::AbortSampleLoading()
{
	sampleLoadedAborted = true;
}

void LFT_Logs::DeleteLog(int logIndex)
{
	_model->SendInt("DELETELOG", logIndex, 3000);
}

void LFT_Logs::QueGetAllHeaders()
{
	GetLogsRequired = true;
	GetLogsProgress = 0;
}

void LFT_Logs::GetAllHeaders()
{
	for (int i = 0; i < LOG_MAX; i++)
	{
		LoadedLogs[i] = GetHeader(i);
		GetLogsProgress = GetLogsProgress + 1;
	}

#ifdef SIMULATOR
	LoadedLogs[0] = RecoverLog("Tue Nov 26 09:12:31 2019	35	190	750	35	190	743	8	3	22	0	6514	189	22	75	0	56	54	6	4609	900	10	10	1128	1236-00001	__________	0	0	0	0	30	26	65	1052	", 0);	
#endif

	//Sort Headers	
	InsertionSort();		

	GetLogsRequired = false;
}

void LFT_Logs::InsertionSort()
{
	for (int i = 1; i < LOG_MAX; i++)
	{
		RecoverLog value = LoadedLogs[i];
		int hole = i;
		while (hole > 0 && LoadedLogs[hole - 1].StartTime.getRaw() > value.StartTime.getRaw())
		{
			LoadedLogs[hole] = LoadedLogs[hole - 1];
			hole--;
		}
		LoadedLogs[hole] = value;
	}

	std::reverse(LoadedLogs, LoadedLogs + LOG_MAX);
}

void LFT_Logs::QueExportAll()
{
	ExportAllRequired = true;
	ExportLogsResult = 0;
}

void LFT_Logs::ExportAll()
{	
	ExportLogsResult = _model->MountUSB();
	if (ExportLogsResult == 0)
	{
		for (int i = 0; i < LOG_MAX; i++)
		{
			if (!LoadedLogs[i].Exists)
				continue;

			ExportLogsResult = _model->WriteLog(&LoadedLogs[i]);
			if (ExportLogsResult != 0)							 	
				break;
			
		}		
	}
	else
	{
		//To distinguish between types of error
		ExportLogsResult = 50 + ExportLogsResult;
	}



	_model->UnmountUSB();

	ExportAllRequired = false;
}

RecoverLog* LFT_Logs::GetSampleQueLogs()
{
	for (int i = 0; i < LOG_MAX; i++)
	{
		if (LoadedLogs[i].Index == quedLogIndex)
			return &LoadedLogs[i];
	}

	return NULL;
}

int RecoverLog::BaseHeaterDeviation()
{
	float percentage = ((float)FinalBaseHeaterTemperature / BaseHeaterSetpoint) * 100;
	return (int)(percentage - 100);
}

bool RecoverLog::BaseHeaterDeviationAcceptable()
{
	int deviation = BaseHeaterDeviation();
	return deviation > -10 && FinalBaseHeaterTemperature < 60;
	
}

int RecoverLog::PrecursorHeaterDeviation()
{
	float percentage = ((float)FullPrecursorHeaterTemperature / PrecursorHeaterSetpoint) * 100;
	return (int)(percentage - 100.0f);
}

bool RecoverLog::PrecursorDeviationAcceptable()
{
	int deviation = PrecursorHeaterDeviation();
	return deviation > -5 && deviation < 5;
}

int RecoverLog::PressureDeviation()
{
	float percentage = ((float)FinalPressureMeasurement / VacuumSetpoint) * 100;
	return (int)(percentage - 100.0f);
}

bool RecoverLog::PressureDeviationAcceptable()
{
	return FinalPressureMeasurement < 1000 && FinalPressureMeasurement != 0;
}

bool RecoverLog::LeakTest1Acceptable()
{
	return LeakTest1Result < 75;
}

bool RecoverLog::LeakTest2Acceptable()
{
	return LeakTest2Result < 75;
}

void RecoverLog::GetData(uint16_t* buffer, int *length)
{	
	*length = 25;

	STime startTime(StartTime.getRaw());

	//Start Time
	buffer[0x00] = startTime.GetYear() - 2000;
	buffer[0x01] = startTime.GetMonth();
	buffer[0x02] = startTime.GetDay();
	buffer[0x03] = startTime.GetHour();
	buffer[0x04] = startTime.GetMinute();
	buffer[0x05] = startTime.GetSecond();

	//Stage 1 Heater Deviation
	buffer[0x06] = FinalBaseHeaterTemperature;
	buffer[0x07] = BaseHeaterSetpoint;

	//Stage 2 Heater Deviation
	buffer[0x08] = FinalPrecursorHeaterTemperature;
	buffer[0x09] = FullPrecursorHeaterTemperature;
	buffer[0x0A] = PrecursorHeaterSetpoint;

	//Pressure Deviation
	buffer[0x0B] = FinalPressureMeasurement;
	buffer[0x0C] = VacuumSetpoint;

	//Leak Test 1
	buffer[0x0D] = LeakTest1Result;

	//Leak Test 2
	buffer[0x0E] = LeakTest2Result;

	//Conditioning Time
	buffer[0x0F] = TimeToReachVacuumSetpoint;

	//Fuming Time
	buffer[0x10] = HeatRunTime;

	//Firmware Version
	buffer[0x11] = firmwareBuild;

	//Serial Number
	//We can assume the 1236- so we just need the 5 characters after that
	buffer[0x12] = SerialNumber[5];
	buffer[0x13] = SerialNumber[6];
	buffer[0x14] = SerialNumber[7];
	buffer[0x15] = SerialNumber[8];
	buffer[0x16] = SerialNumber[9];

	//Metal Type
	buffer[0x17] = MetalType;

	//Chamber Type
	buffer[0x18] = ChamberSize;	
}
