#ifndef PROCESSSCREEN_PRESENTER_HPP
#define PROCESSSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#ifndef SIMULATOR
//#define REAL
#endif

using namespace touchgfx;

class ProcessScreenView;

class ProcessScreenPresenter : public Presenter, public ModelListener
{
public:
    ProcessScreenPresenter(ProcessScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ProcessScreenPresenter() {};

	virtual void SetScreensaverState(bool state);
	
	//LFT LID COMMANDS
	void LidDown() { model->LidDown(); }
	void LidUp() { model->LidUp(); }
	void LidStop() { model->LidStop(); }	
	bool GetLidClosedState() { return model->GetLidClosedState(); }
	void SetLidClosedState(bool value) { model->SetLidClosedState(value); }

	//LFT Commands
#ifdef REAL
	void SetBaseTemperatureSetting() { model->SetBaseTemperatureSetting(); }
	void SetPreTemperatureSetting() { model->SetPreTemperatureSetting(); }
	void QuePrechecks() { model->QuePrechecks(); }
	void QueRefreshValues() { model->QueRefreshValues(); }
	void QuePumpDown() { model->QuePumpdown(); }
	void Abort() { model->Abort(); }
	void ClearErrors() { model->ClearErrors(); }	
#else	
	void SetBaseTemperatureSetting() { }
	void SetPreTemperatureSetting() { }
	void QuePrechecks() { }
	void QuePumpDown() { }
	void QueRefreshValues() { }	
	void Abort() { }
	void ClearErrors() { }
#endif	
	
#ifdef USEAUTO			
	void QueHeat() { model->QueHeat(); }		
	void QueCool() { model->QueCool(); }
#else		
	void QueHeat() { }		
	void QueCool() { }
#endif // USEAUTO

	//LFT Information
	int ReadBaseTemperature() { return model->GetBaseTemp(); }
	int ReadPreTemperature() { return model->GetPreTemp(); }
	float ReadPressure() { return model->GetPressure(); }
	DateTime ReadTime() { return model->GetTime(); }
	int GetProgress() { return model->GetProgress(); }
	int GetStatus() { return model->GetStatus(); }
	int GetError(int errorNumber) { return model->ReadError(errorNumber); }
	void UpdateLFTDebug(LFTDebug *control) { model->UpdateLFTDebug(control); }

private:
    ProcessScreenPresenter();

    ProcessScreenView& view;
};


#endif // PROCESSSCREEN_PRESENTER_HPP
