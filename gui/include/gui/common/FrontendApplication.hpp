#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>
#include <gui/common/date.hpp>
#include <gui/common/Build.hpp>

#ifdef SIMULATOR
#include "touchgfx\Utils.hpp"
#endif // SIMULATOR


class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:	
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }
    
	virtual void handleTickEvent();
	virtual void handleClickEvent(const ClickEvent& evt);
	virtual void handleDragEvent(const DragEvent& evt);	

	void gotoFREDAScreenNoTransition();	
	void gotoEggNoTransition();
	void gotoMainScreenScreenSlideTransitionNorth();

	bool AllowIdle;
	bool isEngineerModeOn;
	bool isDemoModeOn;
protected:	
	touchgfx::Callback<FrontendApplication> transitionCallback;
	void gotoFREDAScreenNoTransitionImpl();
	void gotoEggNoTransitionImpl();
	void gotoMainScreenScreenSlideTransitionNorthImpl();
	

private:
	//Not exact times, a tick is roughly 1/60 second.
	//static const int SCREEN_SAVER_TIMEOUT = 600; // 10 Seconds
	static const int SCREEN_SAVER_TIMEOUT = 10800; // 3 Minutes


	bool screenSaverTriggered = false;		
						

	int tickCounter;	




};

#endif // FRONTENDAPPLICATION_HPP
