#ifndef FREDA_PRESENTER_HPP
#define FREDA_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <string>

using namespace touchgfx;

class FREDAView;

class FREDAPresenter : public Presenter, public ModelListener
{
public:
    FREDAPresenter(FREDAView& v);

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

    virtual ~FREDAPresenter() {};

	void SendAndRead(unsigned char command[], int commandSize, unsigned char* result, int& resultLength) { model->SendAndRead(command, commandSize, result, resultLength); }
	
	//LFT Lid commands
	void LidDown() { model->LidDown(); }
	void LidUp() { model->LidUp(); }
	void LidStop() { model->LidStop(); }

	//LFT Manual Commands
	void SetStandbyState(bool state) { model->SetStandbyState(state); }
	void SetInletState(bool state) { model->SetInletState(state); }
	void SetPurgeState(bool state) { model->SetPurgeState(state); }
	void SetBypassState(bool state) { model->SetBypassState(state); }
	void SetPumpState(bool state) { model->SetPumpState(state); }
	void SetBaseFanState(bool state) { model->SetBaseFanState(state); }
	void SetStiringFansState(bool state) { model->SetStiringFansState(state); }
	void SetBaseTemperature(int temperature) { model->SetBaseTemperature(temperature); }
	void SetPreTemperature(int temperature) { model->SetPreTemperature(temperature); }
	void BreakSeal(int time) { model->BreakSeal(time); }

	//LFT Auto Commands
	void SetBaseTemperatureSetting() { model->SetBaseTemperatureSetting(); }
	void SetPreTemperatureSetting() { model->SetPreTemperatureSetting(); }
	void QuePrechecks() { model->QuePrechecks(); }
	void QuePumpDown() { model->QuePumpdown(); }
	void QueHeat() { model->QueHeat(); }
	void QueCool() { model->QueCool(); }
	void Abort() { model->Abort(); }	

	//LFT Information		
	int GetBaseTemperature() { return model->GetBaseTemp(); }
	int GetPreTemperature() { return model->GetPreTemp(); }
	float GetPressure() { return model->GetPressure(); }
	DateTime GetRTC() {  return model->GetRTC(); }
	DateTime GetTime() { return model->GetTime(); }
	DateTime GetTimerStart() { return model->GetTimerStart(); }
	int GetProgress() { return model->GetProgress(); }	
	int GetInternalSwitch1() { return model->GetInternalSwitch1(); }
	int GetInternalSwitch2() { return model->GetInternalSwitch2(); }
	int GetDoorSwitch() { return model->GetDoorSwitch(); }
	int GetStatus() { return model->GetStatus(); }
	int GetLastError() { return model->error; }
	int GetAttempts() { return model->attempts; }
	int GetErrors() { return model->errors; }
	void ResetTimer() { model->ResetTimer(); }

	bool GetStandbyState() { return model->GetStandbyState(); }
	bool GetInletState() { return model->GetInletState(); }
	bool GetPurgeState() { return model->GetPurgeState();  }
	bool GetBypassState() { return model->GetBypassState(); }
	bool GetPumpState() { return model->GetPumpState(); }
	bool GetBaseFanState() { return model->GetBaseFanState(); }
	bool GetStiringFanState() { return model->GetStiringFansState(); }
	int GetBaseHeaterSetPoint() { return model->GetBaseHeaterSetPoint(); }
	int GetPrecursorHeaterSetPoint() { return model->GetPrecursorHeaterSetPoint(); }
	void UpdateLFTDebug(LFTDebug *control) { return model->UpdateLFTDebug(control); }
	void SetSensitiveDataHidden(bool state) { model->SetSensitiveDataHidden(state); }
	bool GetSensitiveDataHidden() { return model->GetSensitiveDataHidden(); }

	void SetEngineeringMode(bool state) { model->isEngineerModeOn = state; }

	int HIGHTEMP_ON() { return model->HIGHTEMP_ON; }	
	int HIGHTEMP_OFF() { return model->HIGHTEMP_OFF; }
	int LOWTEMP_ON() { return model->LOWTEMP_ON; }
	int LOWTEMP_OFF() { return model->LOWTEMP_OFF; }
		
		

private:
    FREDAPresenter();

    FREDAView& view;
};


#endif // FREDA_PRESENTER_HPP
