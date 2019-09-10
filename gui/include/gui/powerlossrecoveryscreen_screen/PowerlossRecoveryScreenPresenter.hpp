#ifndef POWERLOSSRECOVERYSCREEN_PRESENTER_HPP
#define POWERLOSSRECOVERYSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class PowerlossRecoveryScreenView;

class PowerlossRecoveryScreenPresenter : public Presenter, public ModelListener
{
public:
    PowerlossRecoveryScreenPresenter(PowerlossRecoveryScreenView& v);

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

    virtual ~PowerlossRecoveryScreenPresenter() {};

private:
    PowerlossRecoveryScreenPresenter();

    PowerlossRecoveryScreenView& view;
};


#endif // POWERLOSSRECOVERYSCREEN_PRESENTER_HPP
