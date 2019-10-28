#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <gui_generated/containers/BackgroundBase.hpp>
#include <gui/common/Unlock.hpp>



class Background : public BackgroundBase
{
public:
    virtual ~Background() {}
	Background();

protected:

	Unlock swipeUnlock;
	touchgfx::Callback<Background, const Unlock&> SwipeUnlockedCallback;
	void SwipeUnlocked(const Unlock &);

};

#endif // BACKGROUND_HPP
