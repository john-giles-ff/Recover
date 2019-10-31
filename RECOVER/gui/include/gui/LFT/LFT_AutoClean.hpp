#ifndef LFT_AUTOCLEAN_HPP
#define LFT_AUTOCLEAN_HPP


#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\LFT\LFT_Manual.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\LFT\LFT_Settings.hpp>

typedef enum AUTOCLEAN_STAGES
{
	AUTOCLEAN_STAGE_START = 0,	
	AUTOCLEAN_STAGE_PUSH_MOISTURE,
	AUTOCLEAN_STAGE_BUILD_PRESSURE,
	AUTOCLEAN_STAGE_FINISHING,
	AUTOCLEAN_STAGE_FINISHED,
	AUTOCLEAN_STAGE_NONE,	
} AutoCleanStage;


class LFT_AutoClean
{
public:	
	LFT_AutoClean(LFT_Information * information, LFT_Manual * manual, LFT_Settings * settings);
	void SetModel(Model * model);	
	

	void StartAutoClean();
	void ProcessAutoClean();

	AutoCleanStage GetState();
	void SetStage(AutoCleanStage stage);
	
	int GetProgress();
	bool GetAbortState();

	int GetStatus();
	int ResetErrors();

	void SetIsStandalone(bool value);
	bool GetIsStandalone();
	
	void Abort();
	

private:	
	void StageStart();	
	void StagePushMoisture();
	void StageBuildPressure();
	void StageFinishing();


	void msleep(int ms);

	Model * _model;

	LFT_Information* _information;
	LFT_Manual* _manual;
	LFT_Settings* _settings;

	DateTime _stageStartTime;
	DateTime _mainStartTime;

	AutoCleanStage _currentStage = AUTOCLEAN_STAGE_NONE;	

	ThreadSafe<DateTime> _totalStartTime;	
	ThreadSafe<bool> _abort = false;	
	ThreadSafe<bool> _finished = false;
	ThreadSafe<int> _status = 0;
	ThreadSafe<bool> _isStandalone;

	static constexpr int ESTIMATE_FULL_CLEAN_SECONDS = 1350;	
	

};



#endif /* AUTOCLEAN_HPP */
