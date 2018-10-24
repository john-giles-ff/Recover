#ifndef PROCESSSCREEN_VIEW_HPP
#define PROCESSSCREEN_VIEW_HPP

#include <gui_generated/processscreen_screen/ProcessScreenViewBase.hpp>
#include <gui/processscreen_screen/ProcessScreenPresenter.hpp>
#include <gui/common/LFTDebug.hpp>
#include <gui/common/ProgressBar.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <texts\TextKeysAndLanguages.hpp>
#include <texts\TypedTextDatabase.hpp>
#include <gui\common\Lever.hpp>
#include <gui\containers\ErrorBox.hpp>
#include <gui/common/Unlock.hpp>
#include <ctgmath>

#ifndef SIMULATOR
#define REAL
#endif

constexpr int DUMMY_WAIT_TIME = 1000;


constexpr int STAGE_COUNT = 7;
enum LFT_STAGE
{
	LFT_STAGE_LID_CONTROL = 0,	
	LFT_STAGE_INITIALISING = 1,
	LFT_STAGE_CHAMBER_CONDITIONING = 2,
	LFT_STAGE_READY_TO_FUME = 3,
	LFT_STAGE_FUMING = 4,
	LFT_STAGE_PURGING = 5,
	LFT_STAGE_FINISHED = 6
};



class ProcessScreenView : public ProcessScreenViewBase
{
public:
	ProcessScreenView() :
		LidControlValueChangedCallback(this, &ProcessScreenView::LidControlValueChanged)	
	{}
    virtual ~ProcessScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	virtual void handleTickEvent();

	virtual void NextStage();	
	virtual void UpdateStage();				

	virtual void ConfirmStopFuming();
	virtual void ConfirmAbort();
	
	virtual void DemoProcess();

	virtual void AbortProcess();		
protected:
	ProgressBar progressBar;
	ErrorBox ErrorWindow;

	Image pointerImage;
	Image tapImage;
	TextArea txtDemoMode;

	Lever lvrLidControl;				
	touchgfx::Callback<ProcessScreenView, const Lever&> LidControlValueChangedCallback;
	void LidControlValueChanged(const Lever& u);	

	LFTDebug lftDebug;

private:
	int stage = LFT_STAGE_LID_CONTROL;			
	bool _isDemoModeLidClosed = false;	
	bool _guiLidState = false;
	bool _isSetupFinished = false;
	
	void pulseStageText();
	void updateLid();
	void checkLFTValues();		

	//Stage Handlers
	void updateToProgress();

	int tickCounter = 0;			
	int demoCounter = 0;
	int errorNumber = 0;

	float stageTextCounter = 0;
	const float stageTextMaximum = 255;
	const float stageTextDeviation = 100;

	void showError(TEXTS errorMessage);
	
};



#endif // PROCESSSCREEN_VIEW_HPP
