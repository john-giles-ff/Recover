#ifndef LFT_MANUAL_HPP
#define LFT_MANUAL_HPP

#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>
#include <gui\common\ProductionTestStep.hpp>


class LFT_Manual
{
public:
	LFT_Manual(LFT_Information * information);

	void SetModel(Model * model);

	//Manual Board Functions
	void SetStandbyState(bool state);
	void SetInletState(bool state);
	void SetPurgeState(bool state);
	void SetBypassState(bool state);
	void SetPumpState(bool state);
	void SetBaseFanState(bool state);
	void SetStiringFansState(bool state);
	void SetBaseTemperature(int temperature);
	void SetPreTemperature(int temperature);
	void BreakSeal(int time = 3000);	
	void SetOpenDoorState(bool state);
	void RtcOn();

	void Beep(int pitch, int length, int repeat = 0);
	void Beep(int pitch);

	//LFT Lid Controls
	void LidDown();
	void LidUp();
	void LidStop();	
	void ForceLidDown();
	void ForceLidUp();
	void ForceLidStop();

private:
	Model * _model;
	LFT_Information * _information;




};


#endif /* MODEL_HPP */
