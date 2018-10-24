#include <gui/containers/ErrorBox.hpp>

ErrorBox::ErrorBox()
{
	Backdrop.setAlpha(140);	
}

void ErrorBox::DismissError()
{
	setVisible(false);
	invalidate();
}

void ErrorBox::SetText(TypedText text)
{
	TxtErrorMessage.setTypedText(text);
	TxtErrorMessage.setWideTextAction(touchgfx::WIDE_TEXT_WORDWRAP);
	TxtErrorMessage.invalidate();	
}
