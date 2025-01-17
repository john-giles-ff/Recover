#include <gui/common/TextRadioButton.hpp>
#include <texts/TextKeysAndLanguages.hpp>


TextRadioButton::TextRadioButton(TypedText text, touchgfx::LanguageId languageID)
{
	_languageID = languageID;
	Text.setTypedText(text);	
	setTouchable(true);
	add(Text);
}

TextRadioButton::~TextRadioButton()
{

}


void TextRadioButton::handleClickEvent(const ClickEvent& evt)
{   	
	if (evt.getType() != ClickEvent::RELEASED)
		return;

	if (clickEvent && clickEvent->isValid())
		clickEvent->execute(*this);
}

void TextRadioButton::handleDragEvent(const DragEvent& evt)
{	
}
