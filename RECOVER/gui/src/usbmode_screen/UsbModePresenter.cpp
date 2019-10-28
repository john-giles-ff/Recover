#include <gui/usbmode_screen/UsbModeView.hpp>
#include <gui/usbmode_screen/UsbModePresenter.hpp>

UsbModePresenter::UsbModePresenter(UsbModeView& v)
    : view(v)
{
}

void UsbModePresenter::activate()
{

}

void UsbModePresenter::deactivate()
{

}

void UsbModePresenter::SetScreensaverState(bool state)
{
	static_cast<FrontendApplication*>(Application::getInstance())->AllowIdle = state;
}
