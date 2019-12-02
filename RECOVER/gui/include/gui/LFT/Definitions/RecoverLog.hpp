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
		int pos = 0;
		int next = 0;

		String parts[40];
		int i = 0;
		do
		{
			do { next++; } while (input[next] != '\t' && next < (int)input.len());
			parts[i] = input.substr(pos, next - pos);

			pos = next + 1;
			i++;
		} while (next != input.len());
		
		StartTime = parts[0].toDateTime();
		BaseHeaterSetpoint = parts[1].toInt();
		PrecursorHeaterSetpoint = parts[2].toInt();
		VacuumSetpoint = parts[3].toInt();
		FinalBaseHeaterTemperature = parts[4].toInt();
		FullPrecursorHeaterTemperature = parts[5].toInt();
		FinalPressureMeasurement = parts[6].toInt();
		LeakTest1Result = parts[7].toInt();
		LeakTest2Result = parts[8].toInt();
		PumpDownRunTime = parts[9].toInt(); //Old 8 bit value, no longer used
		//HeatRunTime = int.Parse(datas[10]);

		//Samples will return \ if there are no samples (Happens when the process fails)		
		if (parts[11][0] != '\\' || parts[11][0] != '/')
			NumberOfSamples = parts[11].toInt();
		if (NumberOfSamples > 1000)
			NumberOfSamples = 1000;

		FinalPrecursorHeaterTemperature = parts[12].toInt();
		
		TimeToReachVacuumSetpoint = parts[13].toInt();
		MaximumLeakSetting = parts[14].toInt();
		ChamberSize = parts[15].toBool();
		FilterCount = parts[16].toInt();
		TotalRunCount = parts[17].toInt();
		TimeForPSW2ToOperate = parts[18].toInt();
		PressurePSW2Operates = parts[19].toInt();
		LowVacSetting = parts[20].toInt();
		SampleRatePumpdown = parts[21].toInt();
		SampleRateDevelop = parts[22].toInt();
		firmwareBuild = parts[23].toInt();
		SerialNumber = parts[24];
		//There is a skipped case number variable here
		MetalType = parts[26].toBool();

		//New 16 bit value
		HeatRunTime = parts[33].toInt();
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

};


#endif /* MODEL_HPP */
