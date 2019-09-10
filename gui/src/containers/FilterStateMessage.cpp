#include <gui/containers/FilterStateMessage.hpp>

FilterStateMessage::FilterStateMessage()
{
	TxtFilterState.setWideTextAction(WIDE_TEXT_WORDWRAP);

	int left = FILTER_BAD_BAND - LFT::Information.FilterCounter;
	int percentage = (int)(((float)left / (float)FILTER_BAD_BAND) * 100);	
	if (percentage < 0)
		percentage = 0;


	Unicode::snprintf(TxtFilterStateBuffer, TXTFILTERSTATE_SIZE, "%d", percentage);

	TxtFilterState.invalidate();
}

void FilterStateMessage::initialize()
{
    FilterStateMessageBase::initialize();	
}

void FilterStateMessage::Dismiss()
{
	setVisible(false);
	invalidate();
}

