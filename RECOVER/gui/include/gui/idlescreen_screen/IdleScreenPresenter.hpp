#ifndef IDLESCREEN_PRESENTER_HPP
#define IDLESCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class IdleScreenView;

class IdleScreenPresenter : public Presenter, public ModelListener
{
public:
    IdleScreenPresenter(IdleScreenView& v);

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

	void DisallowScreensaver();
	void SetBacklight(float value);		

    virtual ~IdleScreenPresenter() {};	
private:
    IdleScreenPresenter();

    IdleScreenView& view;
};


#endif // IDLESCREEN_PRESENTER_HPP
