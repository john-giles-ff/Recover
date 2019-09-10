#include <gui/containers/TouchTest.hpp>

TouchTest::TouchTest()
{
	setTouchable(true);

	dragEvent.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
	click_cancel.setColor(touchgfx::Color::getColorFrom24BitRGB(60, 0, 255));
	click_pressed.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 239, 255));
	click_released.setColor(touchgfx::Color::getColorFrom24BitRGB(213, 255, 0));
	oldDragEvent.setColor(touchgfx::Color::getColorFrom24BitRGB(239, 0, 255));

	dragEvent.setHeight(BOX_SIZE);
	dragEvent.setWidth(BOX_SIZE);
	click_cancel.setHeight(BOX_SIZE);
	click_cancel.setWidth(BOX_SIZE);
	click_pressed.setHeight(BOX_SIZE);
	click_pressed.setWidth(BOX_SIZE);
	click_released.setHeight(BOX_SIZE);
	click_released.setWidth(BOX_SIZE);
	oldDragEvent.setHeight(BOX_SIZE);
	oldDragEvent.setWidth(BOX_SIZE);

	add(dragEvent);
	add(click_cancel);
	add(click_pressed);
	add(click_released);
	add(oldDragEvent);
}

void TouchTest::initialize()
{
    TouchTestBase::initialize();
}

void TouchTest::handleClickEvent(const ClickEvent& evt)
{
	if (evt.getType() == ClickEvent::CANCEL)
		click_cancel.setXY(evt.getX(), evt.getY());
	if (evt.getType() == ClickEvent::PRESSED)
		click_pressed.setXY(evt.getX(), evt.getY());
	if (evt.getType() == ClickEvent::RELEASED)
		click_released.setXY(evt.getX(), evt.getY());

	invalidate();
}

void TouchTest::handleDragEvent(const DragEvent& evt)
{
	oldDragEvent.setXY(evt.getOldX(), evt.getOldY());
	dragEvent.setXY(evt.getNewX(), evt.getNewY());
	invalidate();
}

