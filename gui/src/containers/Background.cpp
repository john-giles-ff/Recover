#include <gui/containers/Background.hpp>


Background::Background() :
	SwipeUnlockedCallback(this, &Background::SwipeUnlocked)
{
	//Setup SwipeUnlock	
	swipeUnlock.SetSize(300, 440, 200, 40);
	swipeUnlock.SetSwipeEvent(SwipeUnlockedCallback);
	add(swipeUnlock);
}

void Background::SwipeUnlocked(const Unlock &)
{
	application().gotoFREDAScreenNoTransition();
}
