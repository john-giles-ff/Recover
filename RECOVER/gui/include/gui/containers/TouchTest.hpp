#ifndef TOUCHTEST_HPP
#define TOUCHTEST_HPP

#include <gui_generated/containers/TouchTestBase.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>

constexpr int BOX_SIZE = 25;


class TouchTest : public TouchTestBase
{
public:
    TouchTest();
    virtual ~TouchTest() {}

    virtual void initialize();
protected:


	virtual void handleClickEvent(const ClickEvent& evt);
	virtual void handleDragEvent(const DragEvent& evt);


	Box dragEvent;
	Box oldDragEvent;
	Box click_pressed;
	Box click_released;
	Box click_cancel;
};

#endif // TOUCHTEST_HPP
