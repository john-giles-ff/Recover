#ifndef TOUCHTESTSCREEN_VIEW_HPP
#define TOUCHTESTSCREEN_VIEW_HPP

#include <gui_generated/touchtestscreen_screen/TouchTestScreenViewBase.hpp>
#include <gui/touchtestscreen_screen/TouchTestScreenPresenter.hpp>

class TouchTestScreenView : public TouchTestScreenViewBase
{
public:
    TouchTestScreenView();
    virtual ~TouchTestScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
	virtual void Exit();
};

#endif // TOUCHTESTSCREEN_VIEW_HPP
