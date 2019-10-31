#ifndef LFT_HPP
#define LFT_HPP

#include <gui\model\Model.hpp>

#include <gui\LFT\LFT_Auto.hpp>
#include <gui\LFT\LFT_AutoClean.hpp>
#include <gui\LFT\LFT_Manual.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\LFT\LFT_Logs.hpp>
#include <gui\LFT\LFT_ProductionTests.hpp>
#include <gui\LFT\LFT_Settings.hpp>

#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>
#include <gui\common\FrontendApplication.hpp>

#ifndef SIMULATOR
#include "FreeRTOS.h"
#include "Task.h"
#include "semphr.h"
#endif

constexpr int TASK_WAIT_TIME = 500;

constexpr int CONTINUE_OVERRIDE_MAX = 25;




class LFT
{
public:	
	static void Initialise();

	static void SetModel(Model* model);	

	static LFT_Information Information;	
	static LFT_Manual Manual;
	static LFT_Auto Auto;
	static LFT_AutoClean AutoClean;
	static LFT_Logs Logs;
	static LFT_ProductionTests ProductionTests;
	static LFT_Settings Settings;


private:	
	static Model* _model;
	static FrontendApplication& application() {
		return *static_cast<FrontendApplication*>(Application::getInstance());
	}

	static void LFTLoopThreadProcess(void* params);
	static void LFTThreadProcess();
	static void Initialisation();
	static bool _initalised;

	static void CheckPCMode();
	static void CheckForNextStage();
	static void GotoNextStage();	

	//If the device looses power, attempt to recover!
	static void RecoverFromPowerloss();

	static int progressContinueOverride;
};


#endif /* MODEL_HPP */
