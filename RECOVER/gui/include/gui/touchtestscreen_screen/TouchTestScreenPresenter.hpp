#ifndef TOUCHTESTSCREEN_PRESENTER_HPP
#define TOUCHTESTSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class TouchTestScreenView;

class TouchTestScreenPresenter : public Presenter, public ModelListener
{
public:
    TouchTestScreenPresenter(TouchTestScreenView& v);

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

    virtual ~TouchTestScreenPresenter() {};

private:
    TouchTestScreenPresenter();

    TouchTestScreenView& view;
};


#endif // TOUCHTESTSCREEN_PRESENTER_HPP
