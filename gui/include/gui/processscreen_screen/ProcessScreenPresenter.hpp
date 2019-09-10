#ifndef PROCESSSCREEN_PRESENTER_HPP
#define PROCESSSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#ifndef SIMULATOR
#define REAL
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
private:
    ProcessScreenPresenter();

    ProcessScreenView& view;
};


#endif // PROCESSSCREEN_PRESENTER_HPP
