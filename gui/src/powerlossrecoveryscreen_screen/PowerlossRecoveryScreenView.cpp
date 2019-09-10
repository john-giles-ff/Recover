#include <gui/powerlossrecoveryscreen_screen/PowerlossRecoveryScreenView.hpp>

PowerlossRecoveryScreenView::PowerlossRecoveryScreenView()
{

}

void PowerlossRecoveryScreenView::setupScreen()
{
    PowerlossRecoveryScreenViewBase::setupScreen();
}

void PowerlossRecoveryScreenView::tearDownScreen()
{
    PowerlossRecoveryScreenViewBase::tearDownScreen();
}

void PowerlossRecoveryScreenView::AttemptRecovery()
{
	Finish(true);
}

void PowerlossRecoveryScreenView::Cancel()
{
	Finish(false);
}

void PowerlossRecoveryScreenView::Finish(bool state)
{
	LFT::Auto.ClearErrors();
	LFT::Auto.SetSettings();

	if (state)
		LFT::Auto.QueFuming();
	else
		LFT::Auto.QueCool();

	application().gotoProcessScreenScreenNoTransition();
}
