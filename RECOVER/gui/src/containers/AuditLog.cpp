#include <gui/containers/AuditLog.hpp>

AuditLog::AuditLog() :
	_log(),
	resizedEvent(),
	deleteClickedEvent()
{
	setHeight(CollapsedSize);	
	UpdateExpandedState();
}

void AuditLog::SetLog(RecoverLog * log)
{
	_log = log;
	
	//Setup Header
	STime startTime(_log->StartTime.getRaw());		
	uint16_t unicodeBuffer[52];
	Unicode::fromUTF8((const uint8_t *)Utils::STimeToString(startTime).c_str(), unicodeBuffer, 26);


	Unicode::snprintf(TxtAuditLogBuffer, TXTAUDITLOG_SIZE, "%s", unicodeBuffer);

	//Setup Other Values
	//Base Heater Deviation
	if (_log->BaseHeaterDeviationAcceptable())
		TxtBaseHeaterDeviationValue.setTypedText(TypedText(T_LOG_TESTSUCCESS));
	else
		TxtBaseHeaterDeviationValue.setTypedText(TypedText(T_LOG_TESTFAIL));
	//Asked to remove deviation values, keeping here incase
	//Unicode::snprintf(TxtBaseHeaterDeviationValueBuffer, TXTBASEHEATERDEVIATIONVALUE_SIZE, "%d", _log->BaseHeaterDeviation());
	TxtBaseHeaterDeviationValue.setColor(GetColorFromBool(_log->BaseHeaterDeviationAcceptable()));

	//Precursor Heater Deviation
	if (_log->PrecursorDeviationAcceptable())
		TxtPrecursorHeaterDeviationValue.setTypedText(TypedText(T_LOG_TESTSUCCESS));
	else
		TxtPrecursorHeaterDeviationValue.setTypedText(TypedText(T_LOG_TESTFAIL));
	//Asked to remove deviation values, keeping here incase
	//Unicode::snprintf(TxtPrecursorHeaterDeviationValueBuffer, TXTPRECURSORHEATERDEVIATIONVALUE_SIZE, "%d", _log->PrecursorHeaterDeviation());
	TxtPrecursorHeaterDeviationValue.setColor(GetColorFromBool(_log->PrecursorDeviationAcceptable()));

	//Pressure Deviation
	if (_log->PressureDeviationAcceptable())
		TxtPressureDeviationValue.setTypedText(TypedText(T_LOG_TESTSUCCESS));
	else
		TxtPressureDeviationValue.setTypedText(TypedText(T_LOG_TESTFAIL));
	//Asked to remove deviation values, keeping here incase
	//Unicode::snprintf(TxtPressureDeviationValueBuffer, TXTPRESSUREDEVIATIONVALUE_SIZE, "%d", _log->PressureDeviation());
	TxtPressureDeviationValue.setColor(GetColorFromBool(_log->PressureDeviationAcceptable()));

	//Leak Test 1
	if (_log->LeakTest1Acceptable())
		TxtLeakTest1Value.setTypedText(TypedText(T_LOG_TESTSUCCESS));
	else
		TxtLeakTest1Value.setTypedText(TypedText(T_LOG_TESTFAIL));
	TxtLeakTest1Value.setColor(GetColorFromBool(_log->LeakTest1Acceptable()));

	//Leak Test 2
	if (_log->LeakTest2Acceptable())
		TxtLeakTest2Value.setTypedText(TypedText(T_LOG_TESTSUCCESS));
	else
		TxtLeakTest2Value.setTypedText(TypedText(T_LOG_TESTFAIL));
	TxtLeakTest2Value.setColor(GetColorFromBool(_log->LeakTest2Acceptable()));

	//Conditioning Time
	Unicode::snprintf(TxtConditioningTimeValueBuffer, TXTCONDITIONINGTIMEVALUE_SIZE, "%d", _log->TimeToReachVacuumSetpoint);

	//Fuming Time
	Unicode::snprintf(TxtFumingTimeValueBuffer, TXTFUMINGTIMEVALUE_SIZE, "%d", _log->HeatRunTime);

	//Firmware Version
	Unicode::snprintf(TxtFirmwareVersionValueBuffer, TXTFIRMWAREVERSIONVALUE_SIZE, "%d", _log->firmwareBuild);

	//Serial Number
	Unicode::UnicodeChar utf16buffer[10];
	Unicode::fromUTF8((const uint8_t *)_log->SerialNumber.c_str(), utf16buffer, 10);	
	Unicode::snprintf(TxtSerialNumberValueBuffer, TXTSERIALNUMBERVALUE_SIZE, "%s", utf16buffer);
	
	//Metal Type
	if (_log->MetalType)
		TxtMetalTypeValue.setTypedText(TypedText(T_METAL_SILVER));
	else
		TxtMetalTypeValue.setTypedText(TypedText(T_METAL_COPPER));

	//Chamber Type
	if (_log->ChamberSize)
		TxtChamberTypeValue.setTypedText(TypedText(T_CHAMBER_LARGE));
	else
		TxtChamberTypeValue.setTypedText(TypedText(T_CHAMBER_SMALL));

	//Develop-R Type
	TxtDevelopTypeValue.setTypedText(TypedText(Utils::GetDevelopRString(_log->ChamberSize, _log->MetalType, true)));

	//Advanced mode
	BtnAdvanced.setVisible(_log->NumberOfSamples > 0 && LFT::Information.EngineeringMode);

	invalidate();
}

void AuditLog::ToggleExpanded()
{
	SetExpanded(!_isExpanded);
}

void AuditLog::Delete()
{
	if (deleteClickedEvent && deleteClickedEvent->isValid() && _isLoaded)
		deleteClickedEvent->execute(*this);
}

void AuditLog::Advanced()
{
	LFT::Logs.QueGetAllSamples(_log->Index, _log->NumberOfSamples);
	application().gotoGraphScreenNoTransition();
}

void AuditLog::SetExpanded(bool state)
{
	_isExpanded = state;
	UpdateExpandedState();
}

void AuditLog::SetLoaded(bool state)
{
	_isLoaded = state;
}

void AuditLog::UpdateExpandedState()
{
	if (_isExpanded)
	{
		setHeight(ExpandedSize);
		BtnExpander.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_MINIMISE_ID), Bitmap(BITMAP_MINIMISE_ID));
	}
	else
	{
		setHeight(CollapsedSize);
		BtnExpander.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_MAXIMISE_ID), Bitmap(BITMAP_MAXIMISE_ID));
	}

	//Fire event
	
	
	if (resizedEvent && resizedEvent->isValid() && _isLoaded)
		resizedEvent->execute(*this);
		

	invalidate();
}

colortype AuditLog::GetColorFromBool(bool state)
{
	if (state)
		return Color::getColorFrom24BitRGB(0, 255, 0);
	else
		return Color::getColorFrom24BitRGB(255, 0, 0);
}

