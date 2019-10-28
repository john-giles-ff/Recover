#ifndef USBMODE_PRESENTER_HPP
#define USBMODE_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class UsbModeView;

class UsbModePresenter : public Presenter, public ModelListener
{
public:
    UsbModePresenter(UsbModeView& v);

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

    virtual ~UsbModePresenter() {};

	virtual void SetScreensaverState(bool state);
private:
    UsbModePresenter();

    UsbModeView& view;
};


#endif // USBMODE_PRESENTER_HPP
