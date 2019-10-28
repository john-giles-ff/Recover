#include <gui/containers/Background.hpp>
#include <gui/LFT/LFT.hpp>


Background::Background() :
	SwipeUnlockedCallback(this, &Background::SwipeUnlocked)
{
	if (LFT::Information.EngineeringMode)
	{
		//Setup SwipeUnlock	
		swipeUnlock.SetSize(300, 440, 200, 40);
		swipeUnlock.SetSwipeEvent(SwipeUnlockedCallback);
		add(swipeUnlock);
	}
}

void Background::SwipeUnlocked(const Unlock &)
{
	application().gotoFREDAScreenNoTransition();
}

