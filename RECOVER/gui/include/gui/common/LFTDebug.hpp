#ifndef GUI_LFT_DEBUG_HPP
#define GUI_LFT_DEBUG_HPP

#include <texts\TextKeysAndLanguages.hpp>
#include <touchgfx\containers\Container.hpp>
#include <touchgfx\containers\ListLayout.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx\widgets\TextArea.hpp>
#include <touchgfx\widgets\TextAreaWithWildcard.hpp>
#include <gui/common/Build.hpp>
#include <cmath>

class LFTDebug : public touchgfx::ListLayout
{
public:	
	LFTDebug();
	~LFTDebug();

	void SetTextEnabledColor(touchgfx::colortype color);
	void SetTextDisabledColor(touchgfx::colortype color);
	void SetEnabled(bool state);

	void SetHideSensitiveValues(bool state);

	void SetBuild(int value);
	void SetBaseTemperature(int setPoint, int actual);
	void SetPrecursorTemperature(int setPoint, int actual);
	void SetPressure(float value);
	void SetInternalSwitch(int value1, int value2);	
	void SetExternalSwitch(int value);
	void SetStatus(int value);
	void SetProgress(int value);		
	void SetTimer(int hour, int minute, int second);
	void SetValves(bool inlet, bool purge, bool bypass);
	
private:		
	touchgfx::colortype enabledColor;
	touchgfx::colortype disabledColor;

	bool hideSensitiveValues;
	/*
	 *	Text Areas
	 */	
	touchgfx::TextAreaWithOneWildcard  TxtBuild;
	touchgfx::TextAreaWithTwoWildcards TxtBaseTemperature;
	touchgfx::TextAreaWithTwoWildcards TxtPrecursorTemperature;
	touchgfx::TextAreaWithOneWildcard  TxtPressure;
	touchgfx::TextAreaWithTwoWildcards TxtInternalSwitch;	
	touchgfx::TextAreaWithOneWildcard  TxtExternalSwitch;
	touchgfx::TextAreaWithOneWildcard  TxtStatus;
	touchgfx::TextAreaWithOneWildcard  TxtProgress;		
	touchgfx::TextAreaWithOneWildcard  TxtTimer;
	touchgfx::TextAreaWithOneWildcard  TxtValves;		



	/*
	 * Wildcard Buffers
	 */	
	static const uint16_t GENERIC_BUFFER_SIZE = 10;

	touchgfx::Unicode::UnicodeChar TxtBuildBuffer[GENERIC_BUFFER_SIZE];	
	touchgfx::Unicode::UnicodeChar TxtBaseTemperatureBuffer1[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtBaseTemperatureBuffer2[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtPrecursorTemperatureBuffer1[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtPrecursorTemperatureBuffer2[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtPressureBuffer[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtInternalSwitchBuffer1[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtInternalSwitchBuffer2[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtExternalSwitchBuffer[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtStatusBuffer[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtProgressBuffer[GENERIC_BUFFER_SIZE];
	//touchgfx::Unicode::UnicodeChar TxtAttemptsBuffer[GENERIC_BUFFER_SIZE];	
	touchgfx::Unicode::UnicodeChar TxtTimerBuffer[GENERIC_BUFFER_SIZE];
	touchgfx::Unicode::UnicodeChar TxtValvesBuffer[GENERIC_BUFFER_SIZE];	
};


#endif /* GUI_UTILS_HPP */
