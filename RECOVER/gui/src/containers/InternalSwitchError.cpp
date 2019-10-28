#include <gui/containers/InternalSwitchError.hpp>
#include <gui/LFT/LFT.hpp>

InternalSwitchError::InternalSwitchError()
{

}

void InternalSwitchError::initialize()
{
    InternalSwitchErrorBase::initialize();
}

void InternalSwitchError::SetState(bool state)
{
	_state = state;

	if (state)
		TxtMessage.setTypedText(TypedText(T_FUMINGVACUUMFAILIURE));
	else
		TxtMessage.setTypedText(TypedText(T_PUMPDOWNVACUUMFAILIURE));
}

void InternalSwitchError::Cancel()
{
	if (_state)
		LFT::Auto.QueStage(LFT_STAGE_COOLDOWN);
	else
		LFT::Auto.QueAbort();

	setVisible(false);
	invalidate();
}

