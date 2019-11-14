#ifndef LFT_INFORMATION_HPP
#define LFT_INFORMATION_HPP

#include <gui\model\Model.hpp>

#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>
#include <gui\common\ThreadSafe.hpp>
#include <gui\LFT\Definitions\Stages.hpp>

constexpr int FILTER_GOOD_BAND = 90;
constexpr int FILTER_BAD_BAND = 100;

constexpr int UPDATE_INTERNAL_RTC_MAX = 120;

class LFT_Information
{	

public:	
	LFT_Information() {
		_model = { 0 };		
#ifndef SIMULATOR
		/* Create a mutex type semaphore. */
		xSemaphore = xSemaphoreCreateMutex();		
#else
		xSemaphore = { 0 };
#endif	

		Build.SetSemaphore(xSemaphore);
		SerialNumber.SetSemaphore(xSemaphore);
		BaseTemp.SetSemaphore(xSemaphore);
		BasePower.SetSemaphore(xSemaphore);
		PreTemp.SetSemaphore(xSemaphore);
		PrePower.SetSemaphore(xSemaphore);
		Pressure.SetSemaphore(xSemaphore);
		LidClosedState.SetSemaphore(xSemaphore);
		ChamberState.SetSemaphore(xSemaphore);
		IsInformationCurrent.SetSemaphore(xSemaphore);
		RtcTime.SetSemaphore(xSemaphore);
		Time.SetSemaphore(xSemaphore);
		TimerStart.SetSemaphore(xSemaphore);
		Uptime.SetSemaphore(xSemaphore);
		Progress.SetSemaphore(xSemaphore);
		Status.SetSemaphore(xSemaphore);
		InternalSwitchValue1.SetSemaphore(xSemaphore);
		InternalSwitchValue2.SetSemaphore(xSemaphore);
		ExternalSwitchValue.SetSemaphore(xSemaphore);
		TachoValue.SetSemaphore(xSemaphore);
		StandbyMode.SetSemaphore(xSemaphore);
		InletState.SetSemaphore(xSemaphore);
		PurgeState.SetSemaphore(xSemaphore);
		BypassState.SetSemaphore(xSemaphore);
		PumpState.SetSemaphore(xSemaphore);
		BaseFanState.SetSemaphore(xSemaphore);
		StiringFansState.SetSemaphore(xSemaphore);
		BaseHeaterSetpoint.SetSemaphore(xSemaphore);
		PrecursorHeaterSetpoint.SetSemaphore(xSemaphore);
		ChamberReadyBeep.SetSemaphore(xSemaphore);
		ProgressUpdating.SetSemaphore(xSemaphore);
		SensitiveDataHidden.SetSemaphore(xSemaphore);
		EngineeringMode.SetSemaphore(xSemaphore);
		AllowStageAutoProgression.SetSemaphore(xSemaphore);
		RefreshValuesRequired.SetSemaphore(xSemaphore);
		RefreshValuesProgress.SetSemaphore(xSemaphore);		
		PreChecksRequired.SetSemaphore(xSemaphore);
		ChamberConditioningRequired.SetSemaphore(xSemaphore);
		FumingRequired.SetSemaphore(xSemaphore);
		CoolRequired.SetSemaphore(xSemaphore);
		AbortRequired.SetSemaphore(xSemaphore);
		RunCounter.SetSemaphore(xSemaphore);
		FilterCounter.SetSemaphore(xSemaphore);
		StatusUpToDate.SetSemaphore(xSemaphore);
		Delta.SetSemaphore(xSemaphore);
		AlwaysUpdateRTC.SetSemaphore(xSemaphore);
		DoorOpenState.SetSemaphore(xSemaphore);
		PowerlossDetected.SetSemaphore(xSemaphore);
		ConditioningStartTime.SetSemaphore(xSemaphore);
		UserCipherMode.SetSemaphore(xSemaphore);
}


	void SetModel(Model* model);
//#ifndef  SIMULATOR
	SemaphoreHandle_t xSemaphore;
//#endif // ! SIMULATOR




	//LFT Information
	//Updating this information will not update the value on the LFT
	//int Build;
	ThreadSafe<int> Build;

	ThreadSafe<String> SerialNumber;
	ThreadSafe<int> BaseTemp;
	ThreadSafe<int> BasePower;
	ThreadSafe<int> PreTemp;
	ThreadSafe<int> PrePower;
	ThreadSafe<float> Pressure;
	ThreadSafe<bool> LidClosedState;
	ThreadSafe<bool> ChamberState;

	ThreadSafe<bool> IsInformationCurrent = true;

	ThreadSafe<DateTime> RtcTime;
	ThreadSafe<DateTime> Time;
	ThreadSafe<DateTime> TimerStart;
	ThreadSafe<DateTime> Uptime;
	ThreadSafe<DateTime> ConditioningStartTime;

	//Current Values
	ThreadSafe<int> Progress;
	ThreadSafe<int> Status;
	ThreadSafe<int> InternalSwitchValue1;
	ThreadSafe<int> InternalSwitchValue2;
	ThreadSafe<int> ExternalSwitchValue;
	ThreadSafe<int> TachoValue;
	ThreadSafe<bool> StandbyMode;
	ThreadSafe<bool> InletState;
	ThreadSafe<bool> PurgeState;
	ThreadSafe<bool> BypassState;
	ThreadSafe<bool> PumpState;
	ThreadSafe<bool> BaseFanState;
	ThreadSafe<bool> StiringFansState;
	ThreadSafe<int> BaseHeaterSetpoint;
	ThreadSafe<int> PrecursorHeaterSetpoint;
	ThreadSafe<bool> DoorOpenState;

	//Settings
	ThreadSafe<bool> ChamberReadyBeep = true;

	//States
	ThreadSafe<bool> ProgressUpdating = false;
	ThreadSafe<bool> SensitiveDataHidden = false;
	ThreadSafe<bool> EngineeringMode = false;
	ThreadSafe<bool> UserCipherMode = false;
	ThreadSafe<bool> AllowStageAutoProgression = false;

	//Initialisation Progress
	ThreadSafe<bool> RefreshValuesRequired = true;
	ThreadSafe<int> RefreshValuesProgress = 0;
	const int RefreshValuesMaxmimum = 21;
	ThreadSafe<bool> PowerlossDetected = false;

	//Auto Variables
	ThreadSafe<bool> PreChecksRequired;
	ThreadSafe<bool> ChamberConditioningRequired;
	ThreadSafe<bool> FumingRequired;
	ThreadSafe<bool> CoolRequired;
	ThreadSafe<bool> AbortRequired;
	ThreadSafe<int> RunCounter;
	ThreadSafe<int> FilterCounter;
	ThreadSafe<int> Delta = -1;	

	//Constant Information
	const int HIGHTEMP_ON = 190;
	const int HIGHTEMP_OFF = 40;

	const int LOWTEMP_ON = 35;
	const int LOWTEMP_OFF = 29;

	const int VAC_VALUE = 750;
	const int VAC_MAX = 900;

	const int DELTA_MIN = 1;
	const float DELTA_STOPPED_ACCEPTABLE_MAX = 1.0f;		

	const int DEFAULT_TIMEOUT = 30;

	const int LEAKRATE_MAX = 75;

	const int STIRRING_TIME = 10;

	//Misc
	void ResetTimer();
	void UpdateLFTDebug(LFTDebug *control);
	void SetClockFrequency(long value);

	//Information Reading from board		
	void ReadBuild();
	void ReadSerialNumber();
	void ReadBaseTemperature();
	void ReadBasePowerPercentage();
	void ReadPreTemperature();
	void ReadPrePowerPercentage();
	void ReadPressure();
	void ReadRTC();
	void ReadTime();
	void ReadUptime();
	void ReadProgress(int& output);
	void ReadProgress();
	void ReadStatus();
	void ReadInternalSwitch1();
	void ReadInternalSwitch2();
	void ReadExternalSwitch();
	void ReadTacho();
	void ReadLidState();
	void ReadChamberSize();
	void ReadBaseHeater();
	void ReadPrecursorHeater();
	void ReadInlet();
	void ReadPurge();
	void ReadBypass();
	void ReadPump();
	void ReadBaseFan();
	void ReadStirringFan();
	void ReadStandby();
	void ReadRunCounter();
	void ReadFilterCounter();
	void ReadDelta();

	//Get Calculated Values
	int GetFilterValue();

	//Rare Information Board Reading Values
	void ReadBasePower(float &voltage, float& current);
	void ReadPrecursorPower(float &voltage, float &current);
	void ReadPumpPower(float &voltage, float &current);
	void ReadInletPower(float &voltage, float &current);
	void ReadPurgePower(float &voltage, float &current);
	void ReadBypassPower(float &voltage, float &current);
	void ReadPressurePower(float &voltage, float &current);
	void ReadTemperaturePower(float &voltage, float &current);

	void SetRTC(int year, int month, int day, int hour, int minute, int second);


	void ReadStandardValues(int stage);

	int ReadError(int errorNumber);
	void DeleteError(int errorCode);

	void ResetFilterCounter();
	void FormatData();
	
	ThreadSafe<bool> StatusUpToDate = false;

	ThreadSafe<bool> AlwaysUpdateRTC = false;

	DateTime GetCurrentTime();


private:
	Model* _model;		


	void DecodePower(String input, float &voltage, float &current);

	int updateInternalRTCTicker = 0;	
};


#endif /* MODEL_HPP */
