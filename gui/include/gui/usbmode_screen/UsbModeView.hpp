#ifndef USBMODE_VIEW_HPP
#define USBMODE_VIEW_HPP

#include <gui_generated/usbmode_screen/UsbModeViewBase.hpp>
#include <gui/usbmode_screen/UsbModePresenter.hpp>

class UsbModeView : public UsbModeViewBase
{
public:
    UsbModeView();
    virtual ~UsbModeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();	
protected:
	
};

#endif // USBMODE_VIEW_HPP
