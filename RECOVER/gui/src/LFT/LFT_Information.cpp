#include <gui/LFT/LFT_Information.hpp>


void LFT_Information::SetModel(Model * model)
{
	_model = model;		
}

void LFT_Information::ResetTimer()
{			
	TimerStart = GetCurrentTime();	
}

void LFT_Information::UpdateLFTDebug(LFTDebug * control)
{	
	//Get Difference from time
	long long timerStartRaw = ((DateTime)TimerStart).getRaw();	
	long long rtcTimeRaw = GetCurrentTime().getRaw();

	STime differenceTimeData(rtcTimeRaw - timerStartRaw);	

	//Change setpoints off values to 0
	int preSetpoint = PrecursorHeaterSetpoint;
	int baseSetpoint = BaseHeaterSetpoint;
	if (preSetpoint == 40)
		preSetpoint = 0;
	if (baseSetpoint == 29)
		baseSetpoint = 0;
			
	control->SetHideSensitiveValues(SensitiveDataHidden);
	control->SetEnabled(IsInformationCurrent);
	control->SetBuild(Build);	
	control->SetBaseTemperature(baseSetpoint, BaseTemp);
	control->SetPrecursorTemperature(preSetpoint, PreTemp);
	control->SetPressure(Pressure);
	control->SetInternalSwitch(InternalSwitchValue1, InternalSwitchValue2);
	control->SetExternalSwitch(ExternalSwitchValue);
	control->SetStatus(Status);
	control->SetProgress(Progress);	
	control->SetTimer(differenceTimeData.GetHour(), differenceTimeData.GetMinute(), differenceTimeData.GetSecond());	
	control->SetValves(InletState, PurgeState, BypassState);
	control->SetDelta(Delta, (int)ActualAverageDelta(), (int)MinDeltaAtPressure(Pressure));
}

void LFT_Information::SetClockFrequency(long value)
{
	//Make sure it's a 8 digit number
	String valueString((int)value);
	while (valueString.len() > 8)	
		valueString.erase(valueString.len() - 1, 1);
	
	String command("AUTOTRIM ");
	command += valueString;	

	_model->SendCommand(command);
}

void LFT_Information::ReadBuild()
{
	Build = _model->ReadInt("BUILD");
}

void LFT_Information::ReadSerialNumber()
{
	SerialNumber = _model->ReadString("READSERIAL");

#ifdef SIMULATOR
	SerialNumber = "1236-00001";
#endif
}

void LFT_Information::ReadBaseTemperature()
{
	BaseTemp = _model->ReadInt("READBASE");
}

void LFT_Information::ReadBasePowerPercentage()
{
	BasePower = _model->ReadInt("BASEPOWER");
}

void LFT_Information::ReadPreTemperature()
{
	PreTemp = _model->ReadInt("READPRE");
}

void LFT_Information::ReadPrePowerPercentage()
{
	PrePower = _model->ReadInt("PREPOWER");
}

void LFT_Information::ReadPressure()
{
	Pressure = _model->ReadFloat("READPRESSURE");
}

void LFT_Information::ReadRTC()
{
#ifndef SIMULATOR
	RtcTime = _model->ReadTime("READRTC");
#else	
	//RtcTime = DateTime(63718728180);
	DateTime now;
	now.incYear(2019);
	now.incMonth(6);
	now.incDay(2);
	now.incHour(4);
	now.incMinute(23);
	now.incSecond(54);

	RtcTime = now;
#endif

	//Set Internal RTC from external RTC
	STime time(((DateTime)RtcTime).getRaw());
	_model->SetRTC(time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
}

void LFT_Information::ReadTime()
{
#ifndef SIMULATOR
	String input = _model->ReadString("TIME");
#else
	String input = "13:02";
#endif
	DateTime result;

	int next = 0;
	int pos = 0;
	do { next++; } while (input[next] != ':' && next < (int)input.len());
	result.incMinute(input.substr(pos, next - pos).toInt());

	pos = next + 1;
	do { next++; } while (input[next] != '\n' && next < (int)input.len());
	result.incSecond(input.substr(pos, next - pos).toInt());

	Time = result;
}

void LFT_Information::ReadUptime()
{
	String input = _model->ReadString("UPTIME");
	DateTime result;	

	int next = 0;
	int pos = 0;
	do { next++; } while (input[next] != ':' && next < (int)input.len());
	result.incMinute(input.substr(pos, next - pos).toInt());

	pos = next + 1;
	do { next++; } while (input[next] != '\n' && next < (int)input.len());
	result.incSecond(input.substr(pos, next - pos).toInt());

	Uptime = result;	
}

void LFT_Information::ReadProgress(int & output)
{
	int value = _model->ReadInt("PROGRESS");

	if (value < 0)
		return;

	output = value;	
}

void LFT_Information::ReadProgress()
{
	int progress = 0;
	ReadProgress(progress);

	//If Progress is between 10-80%, allow next stage
	if (progress > 10 && progress < 80)
		AllowStageAutoProgression = true;

	Progress = progress;
}

void LFT_Information::ReadStatus()
{
	Status = _model->ReadInt("STATUS");
	StatusUpToDate = true;
}

void LFT_Information::ReadInternalSwitch1()
{
	InternalSwitchValue1 = _model->ReadInt("PSW1");
}

void LFT_Information::ReadInternalSwitch2()
{
	InternalSwitchValue2 = _model->ReadInt("PSW2");
}

void LFT_Information::ReadExternalSwitch()
{
	ExternalSwitchValue = _model->ReadInt("DOORSW");
}

void LFT_Information::ReadTacho()
{
	TachoValue = _model->ReadInt("READTACHO");
}

void LFT_Information::ReadLidState()
{
	LidClosedState = !_model->ReadBool("LID");
}

void LFT_Information::ReadChamberSize()
{
	ChamberState = _model->ReadString("CHAMBER") == "Large";
}

void LFT_Information::ReadBaseHeater()
{
	BaseHeaterSetpoint = _model->ReadInt("READBASEHEATER");
}

void LFT_Information::ReadPrecursorHeater()
{
	PrecursorHeaterSetpoint = _model->ReadInt("READPRESET");
}

void LFT_Information::ReadInlet()
{
	String result = _model->ReadString("READINLET", 2000);
	InletState = String(result.substr(0, 1)).toInt() == 1;
}

void LFT_Information::ReadPurge()
{
	String result = _model->ReadString("READPURGE", 2000);
	PurgeState = String(result.substr(0, 1)).toInt() == 1;
}

void LFT_Information::ReadBypass()
{
	String result = _model->ReadString("READBYPASS", 2000);
	BypassState = String(result.substr(0, 1)).toInt() == 1;
}

void LFT_Information::ReadPump()
{
	String result = _model->ReadString("READPUMP", 2000);
	PumpState = String(result.substr(0, 1)).toInt() == 1;
}

void LFT_Information::ReadBaseFan()
{
	BaseFanState = _model->ReadBool("READBASEFAN");
}

void LFT_Information::ReadStirringFan()
{
	StiringFansState = _model->ReadBool("READSFAN");
}

void LFT_Information::ReadStandby()
{
	StandbyMode = _model->ReadBool("READSTANDBY");
}

void LFT_Information::ReadRunCounter()
{
	RunCounter = _model->ReadInt("READRUNCOUNT");
}

void LFT_Information::ReadFilterCounter()
{
	FilterCounter = _model->ReadInt("READFILTER");
}

void LFT_Information::ReadDelta()
{
	//Shouldn't be ran if above 15 Torr
	if (Pressure > 15.0f)
		return;

	Delta = _model->ReadInt("DELTA");
}

void LFT_Information::ReadManifoldVersion()
{
	ManifoldVersion = (MANIFOLD_VERSION)_model->ReadInt("VCONFIG");
}

int LFT_Information::GetFilterValue()
{
	int counter = FilterCounter;

	if (counter < FILTER_GOOD_BAND)
		return 0;
	else if (counter < FILTER_BAD_BAND)
		return 1;
	else
		return 2;

	return 0;
}

void LFT_Information::ReadBasePower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 11");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadPrecursorPower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 10");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadPumpPower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 12");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadInletPower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 14");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadPurgePower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 15");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadBypassPower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 16");	
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadPressurePower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 19");
	DecodePower(result, voltage, current);
}

void LFT_Information::ReadTemperaturePower(float & voltage, float & current)
{
	String result = _model->ReadString("RV 20");
	DecodePower(result, voltage, current);
}

void LFT_Information::SetRTC(int year, int month, int day, int hour, int minute, int second)
{
	String command = "SETRTC " +
		Utils::PadL('0', String(hour), 2) + " " + 
		Utils::PadL('0', String(minute), 2) + " " + 
		Utils::PadL('0', String(second), 2) + " " +
		Utils::PadL('0', String(day), 2) + " " +
		Utils::PadL('0', String(month), 2) + " " +
		Utils::PadL('0', String(year % 100), 2);
	
	touchgfx_printf("%s\n", command.c_str());


	_model->SendCommand(command);
	_model->SetRTC(year, month, day, hour, minute, second);	
}

void LFT_Information::ClearPerformance()
{
	_performance = true;
	_deltaAvgsIndex = 0;
	_deltaAvgsCount = 0;

	for (int i = 0; i < DELTA_AVG_SIZE; i++)
		_deltaAvgs[i] = 0x0;
}

void LFT_Information::CheckPerformance()
{
	//If already flagged as underperforming don't bother checking again
	if (!_performance)
		return;

	//Check delta is valid
	int delta = Delta;
	if (delta < 0)
		return;

	//if not in correct range don't bother checking
	float pressure = Pressure;
	if (pressure > 10.0f)
		return;
	

	//Check if its been 10 seconds since we last checked
	long long time = GetCurrentTime().getRaw();
	if (time - _lastPerfCheckTime < 10)
		return;

	//Add current delta to average delta array and update tracker values
	_deltaAvgs[_deltaAvgsIndex] = delta;

	_deltaAvgsIndex++;
	_deltaAvgsCount++;
	_deltaAvgsIndex %= DELTA_AVG_SIZE;
	_deltaAvgsCount = MIN(_deltaAvgsCount, DELTA_AVG_SIZE);
	
	//Fetch current average delta and minimum delta at current pressure
	float deltaAvg = ActualAverageDelta();
	float minDelta = MinDeltaAtPressure(pressure);

	//See if we're performing
	_performance = deltaAvg > minDelta;

	//Update time so we don't do this too often
	_lastPerfCheckTime = time;
}

float LFT_Information::ActualAverageDelta()
{
	if (_deltaAvgsCount == 0)
		return -1;

	//Calculate average
	float sum = 0.0f;
	for (int i = 0; i < _deltaAvgsCount; i++)
		sum += _deltaAvgs[i];
	return sum / _deltaAvgsCount;
}

float LFT_Information::AvgDeltaAtPressure(float pressure)
{
	struct AvgPoint {
		float Pressure;
		float Delta;

		AvgPoint(float pressure, float delta) {
			Pressure = pressure;
			Delta = delta;
		}
	};

	AvgPoint avgPoints[]{
		AvgPoint(9.5f, 1080),
		AvgPoint(9.0f, 1023),
		AvgPoint(8.5f, 920),
		AvgPoint(8.0f, 825),
		AvgPoint(7.5f, 755),
		AvgPoint(7.0f, 673),
		AvgPoint(6.5f, 591),
		AvgPoint(6.0f, 516),
		AvgPoint(5.5f, 445),
		AvgPoint(5.0f, 376),
		AvgPoint(4.5f, 318),
		AvgPoint(4.0f, 263),
		AvgPoint(3.5f, 209),
		AvgPoint(3.0f, 161),
		AvgPoint(2.5f, 118),
		AvgPoint(2.0f, 82),
		AvgPoint(1.5f, 51),
		AvgPoint(1.0f, 27),
		AvgPoint(0.5f, 13),
	};

	//If too high a pressure
	if (pressure > avgPoints[0].Pressure)
		return -1.0f;

	for (int i = 1; i < 19; i++)
	{
		//Check if pressure is between this and pressure item
		if (avgPoints[i].Pressure > pressure)
			continue;

		//Fetch min + max
		AvgPoint smallPoint = avgPoints[i - 1];
		AvgPoint largePoint = avgPoints[i];

		//Find percentage between relevant points
		float pressureDistance = abs(largePoint.Pressure - smallPoint.Pressure);
		float pressurePercentage = abs(pressure - smallPoint.Pressure) / pressureDistance;

		//Get value
		float deltaDistance = abs(largePoint.Delta - smallPoint.Delta);
		return smallPoint.Delta - (pressurePercentage * deltaDistance);
	}

	//If too low a pressure
	return -1.0f;
}

float LFT_Information::MinDeltaAtPressure(float pressure)
{
	float avg = AvgDeltaAtPressure(pressure) * MIN_DELTA_FACTOR;
	return avg > 0 ? avg * MIN_DELTA_FACTOR : -1;
}

bool LFT_Information::Performance()
{
	return _performance;
}

void LFT_Information::ReadStandardValues(int stage)
{
	ReadBaseTemperature();
	ReadPreTemperature();
	ReadPressure();
	ReadInternalSwitch1();
	ReadInternalSwitch2();
	ReadExternalSwitch();	
	ReadTacho();
	ReadStatus();
	//ReadRTC();		
	ReadInlet();
	ReadBypass();
	ReadPurge();
	
	if (stage == LFT_STAGE_CHAMBER_CONDITIONING)
		ReadDelta();
	
	if (EngineeringMode)
	{
		//These values are not needed unless in engineering mode
		ReadStandby();
		ReadBaseHeater();
		ReadPrecursorHeater();
		ReadPump();
		ReadBaseFan();
		ReadStirringFan();
		ReadInlet();
		ReadPurge();
		ReadBypass();		
	}

	updateInternalRTCTicker++;
	if (updateInternalRTCTicker > UPDATE_INTERNAL_RTC_MAX || AlwaysUpdateRTC)
	{
		updateInternalRTCTicker = 0;
		ReadRTC();
	}
}

int LFT_Information::ReadError(int errorNumber)
{
	String command = "ERROR ";
	command += String(errorNumber);
	StatusUpToDate = false;
	return _model->ReadInt(command);
}

void LFT_Information::DeleteError(int errorCode)
{
	_model->SendInt("DELETEERROR", errorCode);
	StatusUpToDate = false;
}

void LFT_Information::ResetFilterCounter()
{
	FilterCounter = 0;
	_model->SendCommand("RESETFILTER 0");
}

void LFT_Information::FormatData()
{
	_model->SendCommand("FORMAT", 10000);
}

DateTime LFT_Information::GetCurrentTime()
{
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	_model->GetRTC(&year, &month, &day, &hour, &minute, &second);
	DateTime current;
	current.incYear(year);
	current.incMonth(month - 1);
	current.incDay(day);
	current.incHour(hour);
	current.incMinute(minute);
	current.incSecond(second);

	return current;
}

void LFT_Information::DecodePower(String input, float & voltage, float & current)
{
	int spaceIndex = input.index(' ') + 1;

	String voltageString = input.substr(0, input.index('V'));
	String currentString = input.substr(spaceIndex, input.index('A') - spaceIndex);	

	voltage = voltageString.toFloat();	
	current = currentString.toFloat();
}
