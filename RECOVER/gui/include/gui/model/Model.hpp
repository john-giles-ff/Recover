#ifndef MODEL_HPP
#define MODEL_HPP


#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <touchgfx\Application.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <cstring>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>
#include <gui\LFT\Definitions\RecoverLog.hpp>

#ifdef SIMULATOR
#include "touchgfx\Utils.hpp"

#else

extern "C"
{	
	#include "stm32f7xx_hal.h"
	#include "stm32f7xx_hal_spi.h"
	#include "stm32f7xx_hal_tim.h"
	#include "stm32f7xx_hal_def.h"
	#include "stm32f7xx_hal.h"
	#include "etem070001zdh6_backlight.h"
	#include "../../../FATFS/App/fatfs.h"
	#include "../../../USB_HOST/App/usb_host.h"	
	#include "../../USB_DEVICE/Target/usbd_cdc_if.h"		

	#include "../../USB_DEVICE/App/usb_device.h"
	#include "../../FATFS/App/fatfs.h"
	#include "stm32f7xx_hal_cortex.h"

	extern UART_HandleTypeDef huart3;
	extern UART_HandleTypeDef huart7;
	extern TIM_HandleTypeDef htim12;	
	extern uint8_t retUSBH; /* Return value for USBH */
	extern char USBHPath[4]; /* USBH logical drive path */
	extern FATFS USBHFatFS; /* File system object for USBH logical drive */
	extern FIL USBHFile; /* File object for USBH */	
	
	extern 	RTC_HandleTypeDef hrtc;
}

#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "semphr.h"


#endif

#include <gui/common/ThreadSafe.hpp>

constexpr int READ_DEFAULT_TIMEOUT_TIME = 1500;



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
	void SetBacklight(float pwm_value);		
	
	//Base Commands
	void SendAndRead(unsigned char command[], int commandSize, unsigned char* result, int& resultLength, int timeout = READ_DEFAULT_TIMEOUT_TIME);

	//Base up 1 commands
	void SendCommand(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	String ReadString(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);	

	//Calls SendCommand or Read String then parses to specified type
	void SendBool(String command, bool state, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	void SendInt(String command, int value, int timeout = READ_DEFAULT_TIMEOUT_TIME);	
	String SendIntReadString(String command, int value, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	int ReadInt(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	float ReadFloat(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	DateTime ReadTime(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);
	bool ReadBool(String command, int timeout = READ_DEFAULT_TIMEOUT_TIME);

	//USB Functions		
	int MountUSB();
	int UnmountUSB();
	int WriteLog(RecoverLog * log);	

	//Onboard RTC
	void SetRTC(int year, int month, int day, int hour, int minute, int second);
	void GetRTC(int *year, int *month, int *day, int *hour, int *minute, int *second);


	//Com Information
	void ResetComms();
	ThreadSafe<int> error = 0;	
	ThreadSafe<int> errors = 0;	

	void tick();

	void Test();

	void SetLED(bool state);


	int GetPCMode();
	void SetPCMode(int pcMode);

protected:
	/**
	 * Pointer to the currently active presenter.
	 */
	ModelListener* modelListener;

private:
	SemaphoreHandle_t xSemaphore;
	int GetUsbState();

	int counter = 0;	

#ifdef SIMULATOR
	int simYear = 2019;
	int simMonth = 5;
	int simDay = 2;
	int simHour = 18;
	int simMinute = 12;
	int simSecond = 32;
#endif

};


#endif /* MODEL_HPP */
