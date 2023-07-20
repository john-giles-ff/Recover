#include <gui/common/LFTDebug.hpp>

LFTDebug::LFTDebug() :
	hideSensitiveValues(false)
{
	TxtBuild.setPosition(0, 0, 240, 28);
	TxtBuild.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtBuild.setLinespacing(0);
	TxtBuild.setTypedText(touchgfx::TypedText(T_BUILD));
	TxtBuildBuffer[0] = 0;
	TxtBuild.setWildcard(TxtBuildBuffer);
	add(TxtBuild);

	TxtBaseTemperature.setPosition(0, 0, 240, 28);
	TxtBaseTemperature.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtBaseTemperature.setLinespacing(0);
	TxtBaseTemperature.setTypedText(touchgfx::TypedText(T_BASETEMP));
	TxtBaseTemperatureBuffer1[0] = 0;
	TxtBaseTemperature.setWildcard1(TxtBaseTemperatureBuffer1);
	TxtBaseTemperatureBuffer2[0] = 0;
	TxtBaseTemperature.setWildcard2(TxtBaseTemperatureBuffer2);
	add(TxtBaseTemperature);

	TxtPrecursorTemperature.setPosition(0, 0, 240, 28);
	TxtPrecursorTemperature.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtPrecursorTemperature.setLinespacing(0);
	TxtPrecursorTemperature.setTypedText(touchgfx::TypedText(T_PRECTEMP));
	TxtPrecursorTemperatureBuffer1[0] = 0;
	TxtPrecursorTemperature.setWildcard1(TxtPrecursorTemperatureBuffer1);
	TxtPrecursorTemperatureBuffer2[0] = 0;
	TxtPrecursorTemperature.setWildcard2(TxtPrecursorTemperatureBuffer2);
	add(TxtPrecursorTemperature);

	TxtPressure.setPosition(0, 0, 240, 28);
	TxtPressure.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtPressure.setLinespacing(0);
	TxtPressure.setTypedText(touchgfx::TypedText(T_PRESSURE));
	TxtPressureBuffer[0] = 0;
	TxtPressure.setWildcard(TxtPressureBuffer);
	add(TxtPressure);

	TxtInternalSwitch.setPosition(0, 0, 240, 28);
	TxtInternalSwitch.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtInternalSwitch.setLinespacing(0);
	TxtInternalSwitch.setTypedText(touchgfx::TypedText(T_INTSWITCH));
	TxtInternalSwitchBuffer1[0] = 0;
	TxtInternalSwitch.setWildcard1(TxtInternalSwitchBuffer1);
	TxtInternalSwitchBuffer2[0] = 0;
	TxtInternalSwitch.setWildcard2(TxtInternalSwitchBuffer2);
	add(TxtInternalSwitch);

	TxtExternalSwitch.setPosition(0, 0, 240, 28);
	TxtExternalSwitch.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtExternalSwitch.setLinespacing(0);
	TxtExternalSwitch.setTypedText(touchgfx::TypedText(T_EXTSWITCH));
	TxtExternalSwitchBuffer[0] = 0;
	TxtExternalSwitch.setWildcard(TxtExternalSwitchBuffer);
	add(TxtExternalSwitch);

	TxtStatus.setPosition(0, 0, 240, 28);
	TxtStatus.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtStatus.setLinespacing(0);
	TxtStatus.setTypedText(touchgfx::TypedText(T_STATUS));
	TxtStatusBuffer[0] = 0;
	TxtStatus.setWildcard(TxtStatusBuffer);
	add(TxtStatus);

	TxtProgress.setPosition(0, 0, 240, 28);
	TxtProgress.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtProgress.setLinespacing(0);
	TxtProgress.setTypedText(touchgfx::TypedText(T_PROGRESS));
	TxtProgressBuffer[0] = 0;
	TxtProgress.setWildcard(TxtProgressBuffer);
	add(TxtProgress);		

	TxtTimer.setPosition(0, 0, 240, 28);
	TxtTimer.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtTimer.setLinespacing(0);
	TxtTimer.setTypedText(touchgfx::TypedText(T_TIMER));
	TxtTimerBuffer[0] = 0;
	TxtTimer.setWildcard(TxtTimerBuffer);
	add(TxtTimer);

	TxtValves.setPosition(0, 0, 240, 28);
	TxtValves.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtValves.setLinespacing(0);
	TxtValves.setTypedText(touchgfx::TypedText(T_VALVES));
	TxtValvesBuffer[0] = 0;
	TxtValves.setWildcard(TxtValvesBuffer);
	add(TxtValves);	

	TxtDelta.setPosition(0, 0, 240, 28);
	TxtDelta.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	TxtDelta.setLinespacing(0);
	TxtDelta.setTypedText(touchgfx::TypedText(T_DELTA));
	TxtDeltaBuffer[0] = 0;
	TxtDelta.setWildcard(TxtDeltaBuffer);
	TxtDelta.setWideTextAction(touchgfx::WIDE_TEXT_CHARWRAP);
	add(TxtDelta);

}

LFTDebug::~LFTDebug()
{
}

void LFTDebug::SetTextEnabledColor(touchgfx::colortype color)
{
	enabledColor = color;

	TxtBuild.setColor(color);
	TxtBaseTemperature.setColor(color);
	TxtPrecursorTemperature.setColor(color);
	TxtPressure.setColor(color);
	TxtInternalSwitch.setColor(color);
	TxtExternalSwitch.setColor(color);
	TxtStatus.setColor(color);
	TxtProgress.setColor(color);		
	TxtTimer.setColor(color);
	TxtValves.setColor(color);	
	TxtDelta.setColor(color);
}

void LFTDebug::SetTextDisabledColor(touchgfx::colortype color)
{
	disabledColor = color;
}

void LFTDebug::SetEnabled(bool state)
{
	touchgfx::colortype currentColor;
	if (state)
		currentColor = enabledColor;
	else
		currentColor = disabledColor;

	TxtBuild.setColor(currentColor);
	TxtBaseTemperature.setColor(currentColor);
	TxtPrecursorTemperature.setColor(currentColor);
	TxtPressure.setColor(currentColor);
	TxtInternalSwitch.setColor(currentColor);
	TxtExternalSwitch.setColor(currentColor);
	TxtStatus.setColor(currentColor);
	TxtProgress.setColor(currentColor);		
	TxtTimer.setColor(currentColor);
	TxtValves.setColor(currentColor);
	TxtDelta.setColor(currentColor);
}

void LFTDebug::SetHideSensitiveValues(bool state)
{	
	hideSensitiveValues = state;
}


void LFTDebug::SetBuild(int value)
{	
	if (value == _build)
		return;

	_build = value;

	touchgfx::Unicode::snprintf(TxtBuildBuffer, GENERIC_BUFFER_SIZE, "%d.%d", Build::Number, value);
	TxtBuild.invalidate();
}

void LFTDebug::SetBaseTemperature(int setPoint, int actual)
{	
	if (hideSensitiveValues)
	{
		setPoint = (int)(((float)setPoint / 35.0f) * 100);
		actual = (int)(((float)actual / 35.0f) * 100);
	}

	if (setPoint == _baseSetpoint && actual == _baseActual)
		return;

	_baseSetpoint = setPoint;
	_baseActual = actual;

	touchgfx::Unicode::snprintf(TxtBaseTemperatureBuffer1, GENERIC_BUFFER_SIZE, "%d", setPoint);
	touchgfx::Unicode::snprintf(TxtBaseTemperatureBuffer2, GENERIC_BUFFER_SIZE, "%d", actual);	
	TxtBaseTemperature.invalidate();
}

void LFTDebug::SetPrecursorTemperature(int setPoint, int actual)
{	
	if (hideSensitiveValues)
	{
		setPoint = (int)(((float)setPoint / 190.0f) * 100);
		actual = (int)(((float)actual / 190.0f) * 100);
	}

	if (setPoint == _precursorSetpoint && actual == _precursorActual)
		return;

	_precursorSetpoint = setPoint;
	_precursorActual = actual;

	touchgfx::Unicode::snprintf(TxtPrecursorTemperatureBuffer1, GENERIC_BUFFER_SIZE, "%d", setPoint);
	touchgfx::Unicode::snprintf(TxtPrecursorTemperatureBuffer2, GENERIC_BUFFER_SIZE, "%d", actual);
	TxtPrecursorTemperature.invalidate();
}

void LFTDebug::SetPressure(float value)
{
	if (hideSensitiveValues)
		value = (std::abs(20.475f - (float)value) / 20.475f) * 100;

	if (abs(value - _pressure) < 0.001)
		return;

	_pressure = value;

	touchgfx::Unicode::snprintfFloat(TxtPressureBuffer, GENERIC_BUFFER_SIZE, "%f", value);
	TxtPressure.invalidate();
}

void LFTDebug::SetInternalSwitch(int value1, int value2)
{
	if (value1 == _internalSwitch1 && value2 == _internalSwitch2)
		return;

	_internalSwitch1 = value1;
	_internalSwitch2 = value2;


	touchgfx::Unicode::snprintf(TxtInternalSwitchBuffer1, GENERIC_BUFFER_SIZE, "%d", value1);
	touchgfx::Unicode::snprintf(TxtInternalSwitchBuffer2, GENERIC_BUFFER_SIZE, "%d", value2);
	TxtInternalSwitch.invalidate();
}


void LFTDebug::SetExternalSwitch(int value)
{
	if (value == _externalSwitch)
		return;

	_externalSwitch = value;

	touchgfx::Unicode::snprintf(TxtExternalSwitchBuffer, GENERIC_BUFFER_SIZE, "%d", value);
	TxtExternalSwitch.invalidate();
}

void LFTDebug::SetStatus(int value)
{
	if (value == _status)
		return;

	_status = value;

	touchgfx::Unicode::snprintf(TxtStatusBuffer, GENERIC_BUFFER_SIZE, "%d", value);
	TxtStatus.invalidate();
}

void LFTDebug::SetProgress(int value)
{
	if (value == _progress)
		return;

	_progress = value;

	touchgfx::Unicode::snprintf(TxtProgressBuffer, GENERIC_BUFFER_SIZE, "%d", value);
	TxtProgress.invalidate();
}

void LFTDebug::SetTimer(int hour, int minute, int second)
{
	if (hour == _timerHour && minute == _timerMinute && second == _timerSecond)
		return;

	_timerHour = hour;
	_timerMinute = minute;
	_timerSecond = second;

	touchgfx::Unicode::snprintf(TxtTimerBuffer, GENERIC_BUFFER_SIZE, "%02d:%02d:%02d", hour, minute, second);
	TxtTimer.invalidate();
}

void LFTDebug::SetValves(bool inlet, bool purge, bool bypass)
{
	if (inlet == _inletValve && purge == _purgeValve && bypass == _bypassValve)
		return;

	_inletValve = inlet;
	_purgeValve = purge;
	_bypassValve = bypass;

	touchgfx::Unicode::snprintf(TxtValvesBuffer, GENERIC_BUFFER_SIZE, "%d|%d|%d", (int)inlet, (int)purge, (int)bypass);
	TxtValves.invalidate();
}

void LFTDebug::SetDelta(int delta, int avgDelta, int minDelta)
{
	if (delta == _delta && avgDelta == _avgDelta && minDelta == _minDelta)
		return;

	_delta = delta;
	_avgDelta = avgDelta;
	_minDelta = minDelta;

	touchgfx::Unicode::snprintf(TxtDeltaBuffer, GENERIC_BUFFER_SIZE, "%d|%d|%d", delta, avgDelta, minDelta);
	TxtDelta.invalidate();
	TxtDelta.resizeHeightToCurrentText();
	TxtDelta.invalidate();
}
