#ifndef EGGSCREEN_VIEW_HPP
#define EGGSCREEN_VIEW_HPP

#include <gui_generated/eggscreen_screen/EggScreenViewBase.hpp>
#include <gui/eggscreen_screen/EggScreenPresenter.hpp>
#include <gui/common/Pong.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <BitmapDatabase.hpp>

class EggScreenView : public EggScreenViewBase
{
public:
	EggScreenView();
    virtual ~EggScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	virtual void handleTickEvent();
protected:
	Pong pong;
	ButtonWithIcon exit;
	
	void buttonCallbackHandler(const touchgfx::AbstractButton& src);
	touchgfx::Callback<EggScreenView, const touchgfx::AbstractButton&> buttonCallback;

};

#endif // EGGSCREEN_VIEW_HPP
