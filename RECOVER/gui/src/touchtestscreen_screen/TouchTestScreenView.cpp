#include <gui/touchtestscreen_screen/TouchTestScreenView.hpp>

TouchTestScreenView::TouchTestScreenView()
{

}

void TouchTestScreenView::setupScreen()
{
    TouchTestScreenViewBase::setupScreen();
}

void TouchTestScreenView::tearDownScreen()
{
    TouchTestScreenViewBase::tearDownScreen();
}

void TouchTestScreenView::Exit()
{
	application().gotoIdleScreenScreenNoTransition();
}
