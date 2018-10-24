#include <gui/containers/ChamberSelection.hpp>

ChamberSelection::ChamberSelection()
{
	setTouchable(true);

	TxtLeft.resizeHeightToCurrentText();
	TxtRight.resizeHeightToCurrentText();

	TxtLeft.setY((getHeight() / 2) - (TxtLeft.getHeight() / 2));
	TxtRight.setY((getHeight() / 2) - (TxtRight.getHeight() / 2));
}

void ChamberSelection::handleClickEvent(const ClickEvent & evt)
{
	if (evt.getType() != ClickEvent::RELEASED)
		return;

	_state = evt.getX() > (getWidth() / 2);			
	updateToState();
}

void ChamberSelection::handleDragEvent(const DragEvent & evt)
{
	int x = evt.getNewX();
	int halfSelected = BoxSelected.getWidth() / 2;

	if (x - halfSelected < 5)
		x = halfSelected + 5;

	if (x + halfSelected > getWidth() - 5)
		x = getWidth() - 5 - halfSelected;

	BoxSelected.setX(x - halfSelected);
	invalidate();
}

void ChamberSelection::updateToState()
{
	if (_state)
		BoxSelected.setX(getWidth() / 2);
	else
		BoxSelected.setX(5);		
	

	invalidate();
}


