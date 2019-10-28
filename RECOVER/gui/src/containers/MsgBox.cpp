#include <gui/containers/MsgBox.hpp>

MsgBox::MsgBox()
{

}

void MsgBox::initialize()
{
    MsgBoxBase::initialize();		
}

void MsgBox::SetText(TEXTS text)
{
	TxtQuery.setTypedText(TypedText(text));
	TxtQuery.setWideTextAction(WIDE_TEXT_WORDWRAP);
	TxtQuery.invalidate();
}

void MsgBox::YesClicked()
{
	if (returnedEvent && returnedEvent->isValid())
		returnedEvent->execute(*this, true);
}

void MsgBox::NoClicked()
{
	if (returnedEvent && returnedEvent->isValid())
		returnedEvent->execute(*this, false);
}

