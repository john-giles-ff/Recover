#ifndef PROCESSSCREEN_VIEW_HPP
#define PROCESSSCREEN_VIEW_HPP

#include <gui_generated/processscreen_screen/ProcessScreenViewBase.hpp>
#include <gui/processscreen_screen/ProcessScreenPresenter.hpp>
#include <gui/common/LFTDebug.hpp>
#include <gui/common/ProgressBar.hpp>
#include <gui/containers/AbortingWindow.hpp>
#include <gui/containers/ExternalSwitchErrorWindow.hpp>
#include <gui/containers/FilterStateMessage.hpp>
#include <gui/containers/InternalSwitchError.hpp>
#include <gui/LFT/Definitions/ProgressStages.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/containers/UsageInstructions.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <texts\TextKeysAndLanguages.hpp>
#include <texts\TypedTextDatabase.hpp>
#include <gui\common\Lever.hpp>
#include <gui\containers\ErrorBox.hpp>
#include <gui/common/Unlock.hpp>
#include <gui/common/Utils.hpp>
#include <gui/containers/MoistureDetectedMessage.hpp>
#include <ctgmath>
#include <gui/containers/MsgBox.hpp>
#include <gui/common/Cipher.hpp>

#ifndef SIMULATOR
#define REAL
#endif

constexpr int DUMMY_WAIT_TIME = 1000; 
constexpr int INSTRUCTION_LENGTH = 7;

constexpr int LVR_POS_START_X = 375;
constexpr int LVR_POS_START_Y = 150;
constexpr int LVR_POS_START_R_X = 725;
constexpr int LVR_POS_START_R_Y = 145;
constexpr int LVR_POS_END_X = 725;
constexpr int LVR_POS_END_Y = 132;





enum LIDCONTROL_STAGE
{
	LIDCONTROL_SELECTTYPE = 0,
	LIDCONTROL_SETCHAMBER,
	LIDCONTROL_MAIN,
};

class ProcessScreenView : public ProcessScreenViewBase
{
public:
	ProcessScreenView() :
		LidControlValueChangedCallback(this, &ProcessScreenView::LidControlValueChanged),
		LogOverwriteMsgBoxReturnedCallback(this, &ProcessScreenView::LogOverwriteMsgBoxReturned)
	{}
    virtual ~ProcessScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	virtual void handleTickEvent();

	virtual void ShowInstructions();

	virtual void NextSubStage();
	virtual void NextStage();	
	virtual void UpdateStage();				

	virtual void ConfirmStopFuming();
	virtual void ConfirmAbort();
	
	virtual void DemoProcess();

	virtual void Type1Selected();
	virtual void Type2Selected();

	virtual void SmallChamberSelected();
	virtual void LargeChamberSelected();

	virtual void Back();

	virtual void ShowPowerLossDetected();
	virtual void ShowRunsRemaining();
	virtual void ShowMoistureDetectedWindow();
	virtual void ShowProcessSelectorChamber();	
	virtual void ShowProcessSelectorType();
	virtual void StartProcess();
	virtual void StartProcess(bool skipOverwriteCheck);
	virtual void StartFume();
	virtual void StartCool();
	virtual void AbortProcess();		

	virtual void GotoHome();
protected:
	ProgressBar progressBar;
	ErrorBox ErrorWindow;	
	ExternalSwitchErrorWindow ExtSwitchErrorWindow;
	InternalSwitchError InternalSwitchErrorWindow;
	UsageInstructions HintInstructions;
	AbortingWindow AbortInProcessWindow;
	FilterStateMessage FilterStateMessageWindow;
	MoistureDetectedMessage MoistureMessage;
	ErrorBox PowerLossWindow;

	MsgBox ConfirmLogOverwrite;

	Image pointerImage;
	Image tapImage;

	TextArea txtDemoMode;

	Lever lvrLidControl;				

	touchgfx::Callback<ProcessScreenView, const Lever&> LidControlValueChangedCallback;
	void LidControlValueChanged(const Lever& u);	

	void LogOverwriteMsgBoxReturned(const MsgBox& u, bool state);
	touchgfx::Callback<ProcessScreenView, const MsgBox&, bool> LogOverwriteMsgBoxReturnedCallback;

	LFTDebug lftDebug;

private:		
	int shownStage = 0;
	int demoStage = LFT_STAGE_LID_CONTROL;
	bool _isDemoModeLidClosed = false;	
	bool _isDemoModePaused = false;
	bool _guiLidState = false;
	bool _isSetupFinished = false;
	bool _requiresInvalidate = false;
	bool _chamberSelected = false;
	bool _typeSelected = false;
	bool _processSuccess = true;	

	int _shownExtValue = 0;	
	int localProgress = 0;	
	
	void pulseStageText();
	void updateLid();
	void checkLFTValues();			
	void updateToProgress();

	int tickCounter = 0;			
	int demoCounter = 0;	
	void checkDemoLidState(bool direction);

	int stageTextCounter = 0;
	const float stageTextMaximum = 255;
	const float stageTextDeviation = 100;

	int previousChamberConditioningPercentage = 0;
	int lidSubStage = 0;
	bool selectSingleStage = false;
	
	long _cipher = -1;
	bool _isUpdateCipherAllowed = false;

	void showError(ErrorMessage * errorCode, int count);	

	void UpdateExternalSwitch(int state);
	void UpdateProcessSelected();	
	

	void RepositionErrors();
	
	Instruction instructions[INSTRUCTION_LENGTH]{
		Instruction(T_INSTRUCTION_ADDPRECURSOR, BITMAP_INSTRUCTION_PRECURSOR_ID),
		Instruction(T_INSTRUCTION_GLASSCHAMBER, BITMAP_INSTRUCTION_ADDGLASS_ID),
		Instruction(T_INSTRUCTION_ADDSAMPLE, BITMAP_INSTRUCTION_SAMPLE_ID),
		Instruction(T_INSTRUCTION_SELECTMETAL, BITMAP_INSTRUCTION_SELECTMETAL_ID),
		Instruction(T_INSTRUCTION_SELECTCHAMBER, BITMAP_INSTRUCTION_SELECTCHAMBER_ID),
		Instruction(T_INSTRUCTION_LOWERLID, BITMAP_INSTRUCTION_LIDCLOSED_ID),
		Instruction(T_INSTRUCTION_STARTPROCESS, BITMAP_INSTRUCTION_STARTBUTTON_ID)
	};

	bool _isDemoLidPaused = false;

	
#ifdef SIMULATOR
	int simAbortCounter = 0;	
#endif

};



#endif // PROCESSSCREEN_VIEW_HPP
