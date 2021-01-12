#ifndef LFT_PRODUCTION_TESTS_HPP
#define LFT_PRODUCTION_TESTS_HPP


#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\LFT\LFT_Manual.hpp>
#include <gui\LFT\LFT_TestValues.hpp>
#include <gui\LFT\LFT_Auto.hpp>
#include <gui\LFT\LFT_Settings.hpp>
#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>

constexpr float PrecursorCurrentUpperBound = 3.0f;
constexpr float PrecursorCurrentLowerBound = 2.0f;
constexpr float BaseCurrentUpperBound = 4.2f;
constexpr float BaseCurrentLowerBound = 3.0f;


class LFT_ProductionTests
{
public:
	LFT_ProductionTests(LFT_Information * lft_information, LFT_Manual * lft_manual, LFT_Auto * lft_auto, LFT_Settings * lft_settings);

	void SetModel(Model * model);

	//LFT Generic Test Commands	
	void AbortProductionTest();
	bool GetProductionTestAborted();	

	void QueGraphPumpdown();
	void GraphPumpdown();
	ThreadSafe<STime> GraphPumpDownTime;

	void QueSoakTest(int numberOfTests = -1);
	void SoakTest();
	ThreadSafe<int> SoakTestStatus;
	ThreadSafe<int> SoakTestCount;

	void QueEMCMode();
	void EmcMode();


	void ReturnToAtmos();
	
	ThreadSafe<bool> IsPumpdownTestQued = false;
	ThreadSafe<bool> IsSoakTestQued = false;
	ThreadSafe<int> NumberOfTests = -1;
	ThreadSafe<bool> IsEMCModeOn = false;

	//Test Data
	ThreadSafe<float> maxBaseCurrent = 0;
	ThreadSafe<float> maxPreCurrent = 0;	


	//Actual Status Data
	ThreadSafe<int> LFT_ACTUAL_LENGTH = 0;
	LFT_Status LFTActual[StatusMaximumPoints];
	int GetStatusValue(GraphType type, LFT_Status status);
	
private:
	Model * _model;
	LFT_Information * _information;
	LFT_Manual * _manual;
	LFT_Auto * _auto;
	LFT_Settings * _settings;

	//Que Information
	ThreadSafe<bool> _isAborted;	


	int lidCounter = 0;


	bool lidDirection = false;

	//Fetch Current Information
	void BaseCurrentCheck();
	void PreCurrentCheck();
	

};


#endif /* MODEL_HPP */
