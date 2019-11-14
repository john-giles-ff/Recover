#ifndef LFT_AUTO_HPP
#define LFT_AUTO_HPP


#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\LFT\LFT_Manual.hpp>
#include <gui\LFT\LFT_Settings.hpp>
#include <gui\LFT\LFT_AutoClean.hpp>

#include <gui\common\date.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>
#include <gui\LFT\Definitions\Stages.hpp>




class LFT_Auto
{
public:	
	LFT_Auto(LFT_Information * information, LFT_Manual * manual, LFT_Settings * settings, LFT_AutoClean * autoClean);

	void SetModel(Model * model);

	//Set Variables used for auto functions
	void SetBaseTemperatureSetting(int temperature = -1);
	void SetPreTemperatureSetting(int temperature = -1);
	void SetVac(int mTorr = -1);
	void SetVacMax(int mTorr = -1);
	void SetLeakMax(int mTorrPer10s = -1);
	void SetTimeout(int minutes = -1);
	void SetChamberSize(bool value);	
	void SetMetalType(bool value);
	void SetChamberBeepReady(bool value);
	void SetStirTime(int minutes = 0);
	void SetUsePurgeFans(bool value);

	void RecoverToStage(int stage);

	//Que Auto Functions
	void QueStage(int stage);

	void QuePrechecks();
	void QueChamberConditioning();
	void QueFuming();
	void QueCool();
	void QueAbort();

	//Misc
	void ClearErrors();
	void DisableTimeout(bool state);

	//Start Auto Functions on current thread
	void SetSettings();
	void StartPreChecks();
	void StartChamberConditioning();
	void StartFuming();
	void StartCool();	
	void Abort();

	void AbortCleanup();	

	void SetStage(int value);
	int GetStage();

private:	
	//Current Auto Stage
	ThreadSafe<int> CurrentStage = 0;

	Model * _model;
	LFT_Information * _information;
	LFT_Manual * _manual;
	LFT_Settings * _settings;
	LFT_AutoClean* _autoClean;
};


#endif /* MODEL_HPP */
