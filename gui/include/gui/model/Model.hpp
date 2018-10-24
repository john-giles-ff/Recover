#ifndef MODEL_HPP
#define MODEL_HPP


#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <touchgfx\Application.hpp>

//#include <gui/model/ModelListener.hpp>
#include <touchgfx/hal/HAL.hpp>
/*#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstdint>*/
#include <cstring>
#include <gui\common\String.hpp>


#ifdef SIMULATOR
#include "touchgfx\Utils.hpp"

#else
extern "C"
{
	//#include "Calibration_Parameters.h"  
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_spi.h"
#include "stm32f7xx_hal_tim.h"
#include "stm32f7xx_hal_def.h"

//#include "etem070001zdh6_gpio.h"
//#include "etem070001zdh6_msp.h"
//#include "etem070001zdh6_lcd.h" 
#include "etem070001zdh6_backlight.h"

	extern UART_HandleTypeDef huart7;
	extern TIM_HandleTypeDef htim12;

}

#include "FreeRTOS.h"
#include "Task.h"
#include "semphr.h"
#endif

constexpr int TASK_WAIT_TIME = 500;
constexpr int READ_DEFAULT_TIMEOUT_TIME = 1500;

constexpr int PUMPDOWN_TIMEOUT = 60;
constexpr int HEAT_TIMEOUT = 60;


class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

	//Generic Functions
	void CreateTask();
	void SetBacklight(float pwm_value);		
	
	//Base Commands
	void SendAndRead(unsigned char command[], int commandSize, unsigned char* result, int& resultLength, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	void SendBool(String command, bool state, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	void SendInt(String command, int value, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	void SendCommand(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	String ReadString(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	int ReadInt(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	float ReadFloat(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	DateTime ReadTime(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	bool ReadBool(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);

	//Read LFT Information
	void UpdateLFTValues();	

	//Get LFT Information
	//Updated Automatically every 500ms
	int GetBuild() { return _build; }
	int GetBaseTemp() { return _baseTemp; }
	int GetPreTemp() { return _preTemp; }
	float GetPressure() { return _pressure; }
	DateTime GetRTC() { return _rtcTime; }
	DateTime GetTime() { return _time; }
	DateTime GetTimerStart() { return _timerStart; }
	int GetProgress() { return _progress; }
	int GetStatus() { return _status; }
	int GetInternalSwitch1() { return _internalSwitchValue1; }
	int GetInternalSwitch2() { return _internalSwitchValue2; }
	int GetDoorSwitch() { return _doorSwitchValue; }

	//Relevant to last command, will not update if it's changed from an outside source
	bool GetStandbyState() { return _standbyMode; }
	bool GetInletState() { return _inletState; }
	bool GetPurgeState() { return _purgeState; }
	bool GetBypassState() { return _bypassState; }
	bool GetPumpState() { return _pumpState; }
	bool GetBaseFanState() { return _baseFanState; }
	bool GetStiringFansState() { return _stiringFansState; }
	int GetBaseHeaterSetPoint() { return _baseHeaterSetpoint; }
	int GetPrecursorHeaterSetPoint() { return _precursorHeaterSetpoint; }

	//LFT Commands
	//LFT Manual Commands
	void SetStandbyState(bool state);
	void SetInletState(bool state);
	void SetPurgeState(bool state);
	void SetBypassState(bool state);
	void SetPumpState(bool state);
	void SetBaseFanState(bool state);
	void SetStiringFansState(bool state);
	void SetBaseTemperature(int temperature);
	void SetPreTemperature(int temperature);
	void BreakSeal(int time);
	void ResetTimer();
	void Beep(int pitch, int length, int repeat = 0);

	//LFT Auto Commands
	void SetBaseTemperatureSetting();
	void SetPreTemperatureSetting();
	void QueRefreshValues();
	void QuePrechecks();
	void QuePumpdown();
	void QueHeat();
	void QueCool();
	void Abort();		    	
	void ClearErrors();

	//LFT Lid Controls
	void LidDown();
	void LidUp();
	void LidStop();
	bool GetLidClosedState() { return _lidClosedState; }
	void SetLidClosedState(bool value) { _lidClosedState = value; }

	//Misc
	void UpdateLFTDebug(LFTDebug *control);	
	bool GetRefreshValuesRequired() { return _refreshValuesRequired; }
	int GetRefreshProgress() { return _refreshValuesProgress; }
	int GetRefreshMaximum() { return _refreshValuesMaxmimum; }
	bool GetSensitiveDataHidden() { return _sensitiveDataHidden; }
	void SetSensitiveDataHidden(bool state) { _sensitiveDataHidden = state; }
	
	//Read back and error
	int ReadError(int errorNumber);

	bool isEngineerModeOn;

	//Com Information
	int error = 0;
	int reads = 0;
	int attempts = 0;
	int errors = 0;	

	//Constant Information
	const int HIGHTEMP_ON = 190;
	const int HIGHTEMP_OFF = 40;

	const int LOWTEMP_ON = 35;
	const int LOWTEMP_OFF = 29;
protected:				
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;

private:
	//States	
	bool _refreshValuesRequired = true;
	bool _progressUpdating = false;
	bool _sensitiveDataHidden = true;

	//Initialisation Progress
	int _refreshValuesProgress = 0;
	const int _refreshValuesMaxmimum = 10;

	//Auto Variables
	bool _preChecksRequired;
	bool _pumpdownRequired;
	bool _heatRequired;
	bool _coolRequired;

	//LFT Information, Will be kept relevant to TASK_WAIT_TIME ms from the task running LoopUpdateValues
	int _build;
	int _baseTemp;
	int _preTemp;
	float _pressure;
	bool _lidClosedState;

	bool _isInformationCurrent = true;

	DateTime _rtcTime;
	DateTime _time;
	DateTime _timerStart;

	int _progress;
	int _status;	
	int _internalSwitchValue1;
	int _internalSwitchValue2;
	int _doorSwitchValue;

	//LFT Information, relevant to last corresponding command, doesn't update from board
	bool _standbyMode;
	bool _inletState;
	bool _purgeState;
	bool _bypassState;
	bool _pumpState;
	bool _baseFanState;
	bool _stiringFansState;
	int _baseHeaterSetpoint;
	int _precursorHeaterSetpoint;


	//Read LFT Information from Board	
	void ReadBuild();
	void ReadBaseTemperature();
	void ReadPreTemperature();
	void ReadPressure();
	void ReadRTC();
	void ReadTime();
	void ReadProgress();
	void ReadStatus();	
	void ReadInternalSwitch1();
	void ReadInternalSwitch2();
	void ReadDoorSwitch();
	void ReadLidState();	

	void ReadBaseHeater();
	void ReadPrecursorHeater();
	void ReadInlet();
	void ReadPurge();
	void ReadBypass();
	void ReadPump();
	void ReadBaseFan();
	void ReadStirringFan();
	void ReadStandby();

	//Auto Commands
	void StartPreChecks();
	void StartPumpDown(int timeout);
	void StartHeat(int timeout);
	void StartCool();
		
	//Static Pointer to model instance
	static Model* model;

	//Static Looping of updating the LFT values
	static void LoopUpdateValues(void* params);		
	void ResetComms();
	
#ifndef  SIMULATOR
	SemaphoreHandle_t xSemaphore;
#endif // ! SIMULATOR
	
	
};


#endif /* MODEL_HPP */
