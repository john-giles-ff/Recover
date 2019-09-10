#ifndef POWERLOSSRECOVERYSCREEN_VIEW_HPP
#define POWERLOSSRECOVERYSCREEN_VIEW_HPP

#include <gui_generated/powerlossrecoveryscreen_screen/PowerlossRecoveryScreenViewBase.hpp>
#include <gui/powerlossrecoveryscreen_screen/PowerlossRecoveryScreenPresenter.hpp>

class PowerlossRecoveryScreenView : public PowerlossRecoveryScreenViewBase
{
public:
    PowerlossRecoveryScreenView();
    virtual ~PowerlossRecoveryScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

	virtual void AttemptRecovery();
	virtual void Cancel();
protected:

private:
	void Finish(bool state);
};

#endif // POWERLOSSRECOVERYSCREEN_VIEW_HPP
