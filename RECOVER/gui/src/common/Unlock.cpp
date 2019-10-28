#include <gui/common/Unlock.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx\Color.hpp>

Unlock::Unlock()
{
    setTouchable(true);	

#ifdef SHOW_BOUNDS
	add(background);
	add(activationZone);
#endif

	percentage = 0;			   	
}

Unlock::~Unlock()
{

}

void Unlock::handleClickEvent(const ClickEvent& evt)
{    	
	//Take measurement for click
	if (evt.getType() == ClickEvent::PRESSED)
	{
		originX = evt.getX();
		originY = evt.getY();
		
		percentage = 0;		
	}	

	if (!_clickSet)
		return;

	//reset back to left / send swiped event
	if (evt.getType() == ClickEvent::RELEASED)
	{												
		if (percentage == 100 && swipeEvent && swipeEvent->isValid())
			swipeEvent->execute(*this);		
		
		if (evt.getX() > originX + clickDeviance || evt.getX() < originX - clickDeviance)
			return;		

		//If it was a click not a drag, send the click event
		if (clickEvent && clickEvent->isValid())
			clickEvent->execute(*this);		
	}
}

void Unlock::handleDragEvent(const DragEvent& evt)
{		
	if (!_swipeSet)	
	{		
		return;
	}	


	//Check that it started in the start area	
	if (originX < getWidth() - activationWidth)
		return;
	
	//Get Percentage
	int handleX = getWidth() - evt.getNewX();
	percentage = ((float)handleX / (float)getWidth()) * 100;

	//Restrict
	if (percentage >= 100)
	{		
		if (swipeEvent && swipeEvent->isValid())
			swipeEvent->execute(*this);
	}
	else if (percentage < 0)
		percentage = 0;	
	
			
}

void Unlock::SetSize(int x, int y, int width, int height)
{
	setXY(x, y);
	setWidth(width);
	setHeight(height);

#ifdef SHOW_BOUNDS
	background.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 0, 0));
	background.setAlpha(150);
	background.setXY(0, 0);
	background.setWidth(width);
	background.setHeight(height);

	activationZone.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 255, 0));
	activationZone.setAlpha(150);
	activationZone.setXY(width - activationWidth, 0);
	activationZone.setWidth(activationWidth);
	activationZone.setHeight(getHeight());
#endif
}

