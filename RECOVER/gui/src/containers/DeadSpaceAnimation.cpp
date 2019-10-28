#include <gui/containers/DeadSpaceAnimation.hpp>

DeadSpaceAnimation::DeadSpaceAnimation()
{
	add(backgroundImage);
	add(animatedImage);

}

void DeadSpaceAnimation::SetAnimationXY(int x, int y)
{
	animatedImage.setXY(x, y);
	
}

void DeadSpaceAnimation::SetUpdateTicksInterval(uint8_t interval)
{	
	animatedImage.setUpdateTicksInterval(interval);
}

void DeadSpaceAnimation::StartAnimation(bool reverse, bool reset, bool loop)
{
	animatedImage.startAnimation(reverse, reset, loop);
}

void DeadSpaceAnimation::StopAnimation()
{
	animatedImage.stopAnimation();
}

void DeadSpaceAnimation::SetImages(touchgfx::Bitmap background, touchgfx::BitmapId startImage, touchgfx::BitmapId endImage)
{	
	animatedImage.setBitmaps(startImage, endImage);
	backgroundImage.setBitmap(background);

	setWidth(background.getWidth());
	setHeight(background.getHeight());
}

void DeadSpaceAnimation::SetImages(touchgfx::Bitmap background)
{
	backgroundImage.setBitmap(background);
	setWidth(background.getWidth());
	setHeight(background.getHeight());
}

void DeadSpaceAnimation::SetImages(touchgfx::BitmapId startImage, touchgfx::BitmapId endImage)
{
	animatedImage.setBitmaps(startImage, endImage);
}
