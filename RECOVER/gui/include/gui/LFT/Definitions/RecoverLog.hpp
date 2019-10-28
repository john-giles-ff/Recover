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
		

		//Start Time
		int next = 0;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		StartTime = input.substr(pos, next - pos).toDateTime();		

		//Base heater
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		BaseHeaterSetpoint = input.substr(pos, next - pos).toInt();

		//Precursor Heater
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		PrecursorHeaterSetpoint = input.substr(pos, next - pos).toInt();

		//Vacuum Setpoint
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		VacuumSetpoint = input.substr(pos, next - pos).toInt();

		//Final Base Temperature
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		FinalBaseHeaterTemperature = input.substr(pos, next - pos).toInt();		

		//Final Precursor Temperature
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		FinalPrecursorHeaterTemperature = input.substr(pos, next - pos).toInt();

		//Final Pressure
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		FinalPressureMeasurement = input.substr(pos, next - pos).toInt();	
		
		//Leak Test 1 Result
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		LeakTest1Result = input.substr(pos, next - pos).toInt();		
		
		//Leak Test 2 Result
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		LeakTest2Result = input.substr(pos, next - pos).toInt();		
		
		//Pumpdown runtime 
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		PumpDownRunTime = input.substr(pos, next - pos).toInt();		
		
		//Heat runtime
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		HeatRunTime = input.substr(pos, next - pos).toInt();		
		
		//Number of samples 
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		String samplesString = input.substr(pos, next - pos);
		if (samplesString[0] != '\\' || samplesString[0] != '/')
			NumberOfSamples = samplesString.toInt();		
		if (NumberOfSamples > 1000)
			NumberOfSamples = 1000;
		
		//Full Precursor temperature
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		FullPrecursorHeaterTemperature = input.substr(pos, next - pos).toInt();
		
		//Time to reach vacuum
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		TimeToReachVacuumSetpoint = input.substr(pos, next - pos).toInt();		

		//Maximum Leak Setting
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		MaximumLeakSetting = input.substr(pos, next - pos).toInt();

		//Chamber Size
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		ChamberSize = input.substr(pos, next - pos).toBool();

		//Filter Count
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		FilterCount = input.substr(pos, next - pos).toInt();

		//Total Run count
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		TotalRunCount = input.substr(pos, next - pos).toInt();

		//Time for PSW2 to operate
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		TimeForPSW2ToOperate = input.substr(pos, next - pos).toInt();

		//Pressure when PSW2 Operates
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		PressurePSW2Operates = input.substr(pos, next - pos).toInt();

		//Low Vac Setting
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		LowVacSetting = input.substr(pos, next - pos).toInt();

		//Sample Rate pumpdown
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		SampleRatePumpdown = input.substr(pos, next - pos).toInt();

		//Sample rate develop
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		SampleRateDevelop = input.substr(pos, next - pos).toInt();

		//Firmware Build
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && next < (int)input.len());
		firmwareBuild = input.substr(pos, next - pos).toInt();

		//Machine Serial
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && input[next] != '\n' && next < (int)input.len());
		SerialNumber = input.substr(pos, next - pos);		
		
		//Case Number (Ignored)
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && input[next] != '\n' && next < (int)input.len());

		//Metal Type
		pos = next + 1;
		do { next++; } while (input[next] != '\t' && input[next] != '\n' && next < (int)input.len());
		MetalType = input.substr(pos, next - pos).toBool();		
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
