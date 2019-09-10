#include <gui/containers/ExternalSwitchErrorWindow.hpp>
#include <gui/LFT/LFT.hpp>

ExternalSwitchErrorWindow::ExternalSwitchErrorWindow()
{

}

void ExternalSwitchErrorWindow::AllowCancel(bool state)
{
	_allowCancel = state;

	if (state)
		TxtMessage.setTypedText(TypedText(T_FUMEISSUEALLOWCANCEL));
	else
		TxtMessage.setTypedText(TypedText(T_FUMEISSUE));
		
	BtnCancel.setVisible(state);

	TxtMessage.invalidate();
	BtnCancel.invalidate();
}

void ExternalSwitchErrorWindow::Cancel()
{
	if (!_allowCancel)
		return;

	LFT::Auto.QueStage(LFT_STAGE_COOLDOWN);
	setVisible(false);
	invalidate();
}

void ExternalSwitchErrorWindow::initialize()
{
    ExternalSwitchErrorWindowBase::initialize();
}

