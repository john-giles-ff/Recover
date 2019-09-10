#ifndef UNLOCK_HPP
#define UNLOCK_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>

#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/Callback.hpp>
#include <BitmapDatabase.hpp>

//#define SHOW_BOUNDS

using namespace touchgfx;

class Unlock : public Container
{
public:

	Unlock();
    virtual ~Unlock();
	
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);	
	   
	virtual void SetClickEvent(GenericCallback< const Unlock &> & callback) { clickEvent = &callback; _clickSet = true;  }
	virtual void SetSwipeEvent(GenericCallback< const Unlock &> & callback) { swipeEvent = &callback; _swipeSet = true;  }

	void SetSize(int x, int y, int width, int height);
	

protected:	
#ifdef SHOW_BOUNDS
	Box background;
	Box activationZone;
#endif

	int originY = 0;
	int originX = 0;

	const int activationWidth = 80;
	const int clickDeviance = 50;

private:
	GenericCallback< const Unlock &> *clickEvent;
	GenericCallback< const Unlock &> *swipeEvent;

	bool _clickSet = false;
	bool _swipeSet = false;

	float percentage;	
};


#endif /* CIRCULARPROGRESS_HPP */
