#ifndef RECOVER_LOGS_HPP
#define RECOVER_LOGS_HPP

#include <algorithm>

//const short LOG_MAX = 64;
const short SAMPLE_MAX = 1000;

enum SAMPLE_MODES
{
	SAMPLE_UNKNOWN = 15,
	SAMPLE_INITIALISE = 0,
	SAMPLE_PUMPDOWN = 1,
	SAMPLE_HEAT = 2,
	SAMPLE_COOL = 3,
	SAMPLE_BREAKSEAL = 4
};

class Sample
{
public:
	SAMPLE_MODES Mode;
	//DateTime SampleTime;
	short SampleNumber;
	short BaseTemperature;
	short PrecursorTemperature;
	short Pressure;
	short Status;
	bool Exists;

	Sample() { 
		Exists = false; 
		Mode = SAMPLE_INITIALISE;
		BaseTemperature = 0;
		PrecursorTemperature = 0;
		Pressure = 0;
		SampleNumber = 0;
		Status = 0;			
	}
	Sample(String input) {
		Exists = true;
		int pos = 0;
		int next = 0;		
		do { next++; } while (input[next] != '\t');
		
		switch (input.substr(0, next)[0])
		{
		case 'I':
			Mode = SAMPLE_INITIALISE;
			break;
		case 'P':
			Mode = SAMPLE_PUMPDOWN;
			break;
		case 'H':
			Mode = SAMPLE_HEAT;
			break;
		case 'C':
			Mode = SAMPLE_COOL;
			break;
		case 'B':
			Mode = SAMPLE_BREAKSEAL;
			break;		
		default:
			Mode = SAMPLE_UNKNOWN;
			break;
		}
				
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		SampleNumber = input.substr(pos, next - pos).toInt();		

		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		BaseTemperature = input.substr(pos, next - pos).toInt();		

		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		PrecursorTemperature = input.substr(pos, next - pos).toInt();		

		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		Pressure = (input.substr(pos, next - pos).toInt() / 10);		

		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		Status = input.substr(pos, next - pos).toInt();		
	}
	Sample(SAMPLE_MODES mode, short number, short baseTemp, short preTemp, short pressure, short status)
	{
		Exists = true;
		Mode = mode;
		SampleNumber = number;
		BaseTemperature = baseTemp;
		PrecursorTemperature = preTemp;
		Pressure = pressure;
		Status = status;
	}
};

class RecoverLog
{
public:
	short Index;
	DateTime StartTime;
	short BaseHeaterSetpoint;
	short PrecursorHeaterSetpoint;
	int VacuumSetpoint;
	short FinalBaseHeaterTemperature;
	short FinalPrecursorHeaterTemperature;
	short FinalPressureMeasurement;
	short LeakTest1Result;
	short LeakTest2Result;
	int PumpDownRunTime;
	int HeatRunTime;
	int NumberOfSamples;
	short FullPrecursorHeaterTemperature;
	int TimeToReachVacuumSetpoint;
	short MaximumLeakSetting;
	bool ChamberSize;
	int FilterCount;
	int TotalRunCount;
	int TimeForPSW2ToOperate;
	int PressurePSW2Operates;
	int LowVacSetting;
	int SampleRatePumpdown;
	int SampleRateDevelop;
	int firmwareBuild;
	String SerialNumber;
	bool MetalType;
	
	bool Exists;

	//Calculations from values
	int BaseHeaterDeviation();
	bool BaseHeaterDeviationAcceptable();
	int PrecursorHeaterDeviation();
	bool PrecursorDeviationAcceptable();
	int PressureDeviation();
	bool PressureDeviationAcceptable();
	bool LeakTest1Acceptable();
	bool LeakTest2Acceptable();

	
	RecoverLog(String input, int index) {
		Index = index;
		Exists = true;		
		

		//Split to parts

		
		//Decode Parts
		StartTime = GetPart(input, 0).toDateTime();
		BaseHeaterSetpoint = GetPart(input, 1).toInt();
		PrecursorHeaterSetpoint = GetPart(input, 2).toInt();
		VacuumSetpoint = GetPart(input, 3).toInt();
		FinalBaseHeaterTemperature = GetPart(input, 4).toInt();
		FullPrecursorHeaterTemperature = GetPart(input, 5).toInt();
		FinalPressureMeasurement = GetPart(input, 6).toInt();
		LeakTest1Result = GetPart(input, 7).toInt();
		LeakTest2Result = GetPart(input, 8).toInt();
		PumpDownRunTime = GetPart(input, 9).toInt();
		//HeatRunTime = int.Parse(datas[10]); //Old 8 bit value, no longer used

		//Samples will return \ if there are no samples (Happens when the process fails)		
		String samplesString = GetPart(input, 11);
		if (samplesString[0] != '\\' && samplesString[0] != '/')
			NumberOfSamples = samplesString.toInt();
		if (NumberOfSamples > 1000)
			NumberOfSamples = 1000;

		FinalPrecursorHeaterTemperature = GetPart(input, 12).toInt();
		
		TimeToReachVacuumSetpoint = GetPart(input, 13).toInt();
		MaximumLeakSetting = GetPart(input, 14).toInt();
		ChamberSize = GetPart(input, 15).toBool();
		FilterCount = GetPart(input, 16).toInt();
		TotalRunCount = GetPart(input, 17).toInt();
		TimeForPSW2ToOperate = GetPart(input, 18).toInt();
		PressurePSW2Operates = GetPart(input, 19).toInt();
		LowVacSetting = GetPart(input, 20).toInt();
		SampleRatePumpdown = GetPart(input, 21).toInt();
		SampleRateDevelop = GetPart(input, 22).toInt();
		firmwareBuild = GetPart(input, 23).toInt();
		SerialNumber = GetPart(input, 24);
		//There is a skipped case number variable here
		MetalType = GetPart(input, 26).toBool();

		//New 16 bit value
		HeatRunTime = GetPart(input, 33).toInt();
	}
	RecoverLog() 
	{ 
		Exists = false; 
		BaseHeaterSetpoint = 0;
		ChamberSize = 0;
		FilterCount = 0;
		FinalBaseHeaterTemperature = 0;
		FinalPrecursorHeaterTemperature = 0;
		FinalPressureMeasurement = 0;
		FullPrecursorHeaterTemperature = 0;
		HeatRunTime = 0;
		Index = 0;
		LeakTest1Result = 0;
		LeakTest2Result = 0;
		LowVacSetting = 0;
		MaximumLeakSetting = 0;
		MetalType = 0;
		NumberOfSamples = 0;
		PrecursorHeaterSetpoint = 0;
		PressurePSW2Operates = 0;
		PumpDownRunTime = 0;
		SampleRateDevelop = 0;
		SampleRatePumpdown = 0;
		TimeForPSW2ToOperate = 0;
		TimeToReachVacuumSetpoint = 0;
		TotalRunCount = 0;
		VacuumSetpoint = 0;
		firmwareBuild = 0;		

	};

	void GetData(uint16_t* buffer, int *length);	


private:
	String GetPart(String input, int index)
	{		
		int i = 0;
		int pos = 0;
		int next = 0;
		do
		{
			do { next++; } while (input[next] != '\t' && next < (int)input.len());

			if (index == i)
				return input.substr(pos, next - pos);			

			pos = next + 1;
			i++;
		} while (next != input.len());

		return String();
	}

};


#endif /* MODEL_HPP */
