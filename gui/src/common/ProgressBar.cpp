#include <gui\common\ProgressBar.hpp>

ProgressBar::ProgressBar() 
{	
	ProgressText.setXY(0, 0);
	ProgressText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
	ProgressText.setLinespacing(0);	
	ProgressText.setTypedText(TypedText(T_WILDCARD));
	ProgressTextBuffer[0] = 0;
	ProgressText.setWildcard(ProgressTextBuffer);	


	Background.setXY(0, 0);
	Foreground.setXY(0, 0);	

	setWidth(getWidth());	
	setHeight(getHeight());
	Foreground.setWidth(0);

	add(Background);
	add(Foreground);
	add(ProgressText);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::SetBackgroundColor(touchgfx::colortype value)
{
	//Update Background Color
	Background.setColor(value);
}

void ProgressBar::SetForegroundColor(touchgfx::colortype value)
{
	//Update Foreground Color
	Foreground.setColor(value);
}

void ProgressBar::SetMaximum(int value)
{
	if (_maximum == value)
		return;

	if (_maximum < 1)
		_maximum = 1;

	//Update Maximum
	_maximum = value;
	updateProgress();
}

void ProgressBar::SetValue(int value)
{
	//Update Value
	if (_value == value)
		return;

	if (_value < 0)
		_value = 0;

	_value = value;	
	updateProgress();
}

void ProgressBar::SetProgressText(TEXTS value)
{
	ProgressText.setTypedText(TypedText(value));	
}

void ProgressBar::SetEasing(bool state)
{
	_easing = state;
}

void ProgressBar::SetAnimate(bool state)
{
	_animate = state;
}

void ProgressBar::setWidth(int16_t width)
{
	Container::setWidth(width);	
	Background.setWidth(width);
	ProgressText.setWidth(width);	
}

void ProgressBar::setHeight(int16_t height)
{
	Container::setHeight(height);

	Foreground.setHeight(height);
	Background.setHeight(height);

	ProgressText.resizeHeightToCurrentText();	
	ProgressText.setY((height / 2) - (ProgressText.getHeight() / 2));
	
}

void ProgressBar::handleTickEvent()
{	
	if (_targetWidth == 0 || _maximum == 0 || _value == 0)
		return;

	int width = 0;

	if (Foreground.getWidth() >= _targetWidth)
		return;
	

	if (_easing)
	{	
		int distanceToTarget = _targetWidth - Foreground.getWidth();
		float accelerationFactor = (float)distanceToTarget / 50.0f;
		if (accelerationFactor > 3.0f)
			accelerationFactor = 3.0f;


		width = Foreground.getWidth() + 3 + (accelerationFactor * 5);
		//width = Foreground.getWidth() + 1 + (float)(pow(((float)distanceToTarget / 100), M_PI));
	}
	else
	{		
		width = Foreground.getWidth() + (float)(Background.getWidth() / 100);		
	}

	if (width > _targetWidth)
		width = _targetWidth;
						
	Foreground.setWidth(width);
	Foreground.invalidate();
}

void ProgressBar::updateProgress()
{
	//Can't divide by 0
	if (_maximum == 0)
		return;

	int valueWidth = getWidth() * ((float)_value / _maximum);

	

	int percentage = (((float)_value / (float)_maximum) * 100);	
	Unicode::snprintf(ProgressTextBuffer, PROGRESSTEXTBUFFER_SIZE, "%d%", percentage);
	ProgressText.invalidate();

	if (_animate && valueWidth > Foreground.getWidth())
	{
		if (_easing)
			_animationTicker = 0;

		_originWidth = Foreground.getWidth();
		_targetWidth = valueWidth;

	}
	else
	{				
		Foreground.setWidth(valueWidth);
		invalidate();
	}
}
