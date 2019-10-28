#include <gui/usbmode_screen/UsbModeView.hpp>

UsbModeView::UsbModeView()
{

}

void UsbModeView::setupScreen()
{
    UsbModeViewBase::setupScreen();
	presenter->SetScreensaverState(false);
}

void UsbModeView::tearDownScreen()
{
	presenter->SetScreensaverState(true);
    UsbModeViewBase::tearDownScreen();
}
