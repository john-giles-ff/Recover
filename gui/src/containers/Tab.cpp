#include <gui/containers/Tab.hpp>

Tab::Tab()
{

}

void Tab::initialize()
{
    TabBase::initialize();
}

void Tab::SetText(TypedText text)
{
	TxtLabel.setTypedText(text);
	TxtLabel.invalidate();
}

void Tab::SetSelected(bool state)
{
	//Selected = FF2EAC62
	//Unselected = FF696969
	colortype selected = touchgfx::Color::getColorFrom24BitRGB(0x2E, 0xAC, 0x62);
	colortype unselected = touchgfx::Color::getColorFrom24BitRGB(0x69, 0x69, 0x69);
	colortype white = touchgfx::Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF);



	if (state)
	{
		TxtLabel.setColor(selected);

		Top.setColor(selected);
		Bottom.setVisible(false);		
		Left.setColor(selected);
		Right.setColor(selected);
	}
	else
	{
		TxtLabel.setColor(white);		

		Top.setColor(unselected);
		Bottom.setVisible(true);
		Left.setColor(unselected);
		Right.setColor(unselected);
	}

	isSelected = state;
	invalidate();
}

bool Tab::GetSelected()
{
	return isSelected;
}

void Tab::handleClickEvent(const ClickEvent & evt)
{	
	if (evt.getType() == ClickEvent::RELEASED)
	{		
		if (ClickedCallback)
		{
			ClickedCallback->execute(*this);			
		}
	}
}

void Tab::SetClickedAction(GenericCallback<const Tab&>& callback)
{
	ClickedCallback = &callback;
}

