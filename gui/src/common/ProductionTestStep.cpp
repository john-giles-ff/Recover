#include <gui\common\ProductionTestStep.hpp>

ProductionTestStep::ProductionTestStep(TEXTS text)
{
	setTouchable(false);

	
	_textID = text;
	
	
	TxtInformation.setPosition(0, 0, 550, 30);	
	TxtInformation.setLinespacing(0);
	TxtInformation.setTypedText(touchgfx::TypedText(text));
	TxtInformationBuffer[0] = 0;
	TxtInformation.setWildcard(TxtInformationBuffer);
	add(TxtInformation);			

	setWidth(550);
	setHeight(30);
}

ProductionTestStep::~ProductionTestStep()
{
}

int a = 0;

void ProductionTestStep::SetState(ProductionTestStepState state)
{
	_state = state;

	
	
	switch (_state)
	{
	case TEST_STATE_WAITING:
		touchgfx::Unicode::snprintf(TxtInformationBuffer, GENERIC_BUFFER_SIZE, "WAITING");
		TxtInformation.setColor(Color::getColorFrom24BitRGB(0, 0, 0));
		break;
	case TEST_STATE_IN_PROGRESS:
		touchgfx::Unicode::snprintf(TxtInformationBuffer, GENERIC_BUFFER_SIZE, "IN PROGRESS");
		TxtInformation.setColor(Color::getColorFrom24BitRGB(0, 0, 0));
		break;
	case TEST_STATE_USER_REQUIRED:
		touchgfx::Unicode::snprintf(TxtInformationBuffer, GENERIC_BUFFER_SIZE, "USER REQUIRED");
		TxtInformation.setColor(Color::getColorFrom24BitRGB(0, 0, 0));
		break;
	case TEST_STATE_SUCCESS:
		touchgfx::Unicode::snprintf(TxtInformationBuffer, GENERIC_BUFFER_SIZE, "SUCCESS");
		TxtInformation.setColor(Color::getColorFrom24BitRGB(106, 186, 68));
		break;
	case TEST_STATE_FAILURE:
		touchgfx::Unicode::snprintf(TxtInformationBuffer, GENERIC_BUFFER_SIZE, "(%d)FAILIURE", _failureDigit);
		TxtInformation.setColor(Color::getColorFrom24BitRGB(255, 0, 0));
		break;
	}
	
		
	invalidate();
}

void ProductionTestStep::SetState(ProductionTestStepState state, int failureDigit)
{
	_failureDigit = failureDigit;
	SetState(state);
}

int ProductionTestStep::GetTextID()
{
	return _textID;
}
