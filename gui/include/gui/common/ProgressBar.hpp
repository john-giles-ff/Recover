#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <touchgfx\containers\Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <ctgmath>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace touchgfx;

class ProgressBar : public Container
{
public:
	ProgressBar();
	virtual ~ProgressBar();

	void SetBackgroundColor(touchgfx::colortype value);
	void SetForegroundColor(touchgfx::colortype value);
	void SetAnimation(bool state) { _animate = state; }
	void SetTextVisible(bool state) { ProgressText.setVisible(state); }
	
	int GetValue() { return _value; }
	void SetMaximum(int value);
	void SetValue(int value);
	void SetProgressText(TEXTS value);	
	void SetEasing(bool state);
	void SetAnimate(bool state);

	void setWidth(int16_t width);
	void setHeight(int16_t height);

	virtual void handleTickEvent();

protected:
	Box Background;
	Box Foreground;	

	TextAreaWithOneWildcard ProgressText;
	static const uint16_t PROGRESSTEXTBUFFER_SIZE = 10;
	touchgfx::Unicode::UnicodeChar ProgressTextBuffer[PROGRESSTEXTBUFFER_SIZE];

private:	
	void updateProgress();

	bool _animate = true;
	bool _easing = true;

	int _maximum;
	int _value;

	int _animationTicker;
	int _targetWidth;	
	int _originWidth;	
	
	TEXTS _progressText;
		

};

#endif