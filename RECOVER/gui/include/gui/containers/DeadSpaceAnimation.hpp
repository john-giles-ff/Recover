#ifndef DEADSPACEANIMATION_HPP
#define DEADSPACEANIMATION_HPP

#include <gui_generated/containers/DeadSpaceAnimationBase.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>

class DeadSpaceAnimation : public DeadSpaceAnimationBase
{
	/*	The idea of this control is to save space.
	 *	In some of the main animations, a lot of the image doesn't update throughout the animation (Deadspace)
	 *	To save space, the animated part of the image should be cut out and stored, whilst 1 image containing the entire image
	 *	should be stored so it can fill in the deadspace
	 *	
	 *	This essentially reduces the reproduction of data between frames in the animation, therefor lowering the entire build size.
	 */


public:
    DeadSpaceAnimation();
    virtual ~DeadSpaceAnimation() {}

	void SetAnimationXY(int x, int y);	
	void SetUpdateTicksInterval(uint8_t interval);

	void StartAnimation(bool reverse, bool reset, bool loop);
	void StopAnimation();
	
	void SetImages(touchgfx::Bitmap background, touchgfx::BitmapId startImage, touchgfx::BitmapId endImage);
	void SetImages(touchgfx::Bitmap background);
	void SetImages(touchgfx::BitmapId startImage, touchgfx::BitmapId endImage);

protected:
	AnimatedImage animatedImage;
	Image backgroundImage;


};

#endif // DEADSPACEANIMATION_HPP
