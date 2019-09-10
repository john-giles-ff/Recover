#include <gui/eggscreen_screen/EggScreenView.hpp>

EggScreenView::EggScreenView() 
	: buttonCallback(this, &EggScreenView::buttonCallbackHandler)
{

}

void EggScreenView::setupScreen()
{
	pong.setXY(0, 0);
	pong.setWidth(800);
	pong.setHeight(480);

	exit.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_X_ID), Bitmap(BITMAP_X_ID));
	exit.setXY(0, 0);
	exit.setAction(buttonCallback);

	add(pong);
	add(exit);
}

void EggScreenView::tearDownScreen()
{

}

void EggScreenView::handleTickEvent()
{
	pong.handleTickEvent();
}

void EggScreenView::buttonCallbackHandler(const touchgfx::AbstractButton & src)
{
	application().gotoIdleScreenScreenNoTransition();

}
