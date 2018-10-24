#ifndef LEVER_HPP
#define LEVER_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class Lever : public Container
{
public:

    Lever();
    virtual ~Lever(); 
	
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
	virtual void handleTickEvent();

	int GetPosition() { return position; }

	void SetYPos(int handleY);
	void SetAnimationAllowed(bool allowed);

	virtual void SetPositionChangedEvent(GenericCallback< const Lever &> & callback) { positionChangedEvent = &callback; }

   
protected:
	//Image background;
	Image handle;
	AnimatedImage animation;

	GenericCallback< const Lever &> *positionChangedEvent;	


	void SetPosition(int value);

	bool mouseDown;

	int position;
	const int deadzone = 20;

	int animationTimer = 0;
	const int animationTimeout = 60;
};


#endif /* CIRCULARPROGRESS_HPP */
