#include <gui/common/Lever.hpp>
#include <texts/TextKeysAndLanguages.hpp>

Lever::Lever()
{
    setTouchable(true);

	//Set Control Images
	//background.setBitmap(Bitmap(BITMAP_WIDE_SLIDER_BACKGROUND_ID));
	handle.setBitmap(Bitmap(BITMAP_WIDE_SLIDER_HANDLE_ID));

	//Setup Animation
	animation.setBitmaps(BITMAP_SLIDERANIMATION_0000_ID, BITMAP_SLIDERANIMATION_0024_ID);
	animation.setUpdateTicksInterval(3);	

	//Set Handle Location
	handle.setXY(0, (animation.getHeight() / 2) - (handle.getHeight() / 2));

	//Add children
	add(animation);	
	add(handle);
	
	//Set Height and Width	
	setWidth(animation.getWidth());
	setHeight(animation.getHeight());    
}

Lever::~Lever()
{

}

void Lever::handleClickEvent(const ClickEvent& evt)
{    
	//Stop Animation from Occuring
	if (evt.getType() == ClickEvent::PRESSED)
		mouseDown = true;

	//Allow Animation, reset back to center
	if (evt.getType() == ClickEvent::RELEASED)
	{	
		mouseDown = false;
		

		int handleCenter = (this->getHeight() / 2) - (handle.getHeight() / 2);
		handle.setY(handleCenter);
		SetPosition(0);		
		animationTimer = 0;
		invalidate();		
	}
}

void Lever::handleDragEvent(const DragEvent& evt)
{
	//If they aren't within the range of the handle, return
	if (evt.getOldY() < handle.getY() || evt.getOldY() > handle.getY() + handle.getHeight())
		return;

	//Stop Animation if it's running
	if (animation.isRunning())
		animation.stopAnimation();
	
	//Get handle position
	int handleY = evt.getNewY() - (handle.getHeight() / 2);	
	SetYPos(handleY);
}

void Lever::handleTickEvent()
{	
	//If the user is interacting with the event don't do this
	if (mouseDown)
		return;

	//Have the animation repeat but with a deadzone in the middle
	animationTimer++;
	if (animationTimer == animationTimeout)
		animation.startAnimation(false, true, false);
	else if (animationTimer > animationTimeout && !animation.isRunning())
		animationTimer = 0;
}

void Lever::SetYPos(int handleY)
{
	int handleMinimumY = 0;
	int handleMaximumY = animation.getHeight() - handle.getHeight();

	//Restrict to lower limit
	if (handleY < handleMinimumY)
		handleY = handleMinimumY;

	//Restrict to higher limit
	if (handleY > handleMaximumY)
		handleY = handleMaximumY;

	//Update Position Value
	int handleCenter = (animation.getHeight() / 2) - (handle.getHeight() / 2);
	if (handleY < handleCenter - deadzone)
		SetPosition(+1);		
	else if (handleY > handleCenter + deadzone)
		SetPosition(-1);		
	else
		SetPosition(0);

	//Set Handle to calculated height
	handle.setY(handleY);

	//Update UI
	invalidate();
}

void Lever::SetAnimationAllowed(bool allowed)
{
	if (allowed)
		mouseDown = false;
	else
	{
		if (animation.isRunning())
			animation.stopAnimation();
		mouseDown = true;
	}
}

void Lever::SetPosition(int value)
{
	if (value == position)
		return;

	position = value;

	if (positionChangedEvent && positionChangedEvent->isValid())
		positionChangedEvent->execute(*this);
}
