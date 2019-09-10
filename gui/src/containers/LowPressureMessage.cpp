#include <gui/containers/LowPressureMessage.hpp>

LowPressureMessage::LowPressureMessage()
{

}

void LowPressureMessage::initialize()
{
    LowPressureMessageBase::initialize();
}

void LowPressureMessage::Dismiss()
{
	setVisible(false);
	invalidate();
}

