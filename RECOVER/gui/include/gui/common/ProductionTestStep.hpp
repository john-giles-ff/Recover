#ifndef PRODUCTION_TEST_STEP_HPP
#define PRODUCTION_TEST_STEP_HPP

#include <touchgfx\containers\Container.hpp>
#include <touchgfx\widgets\canvas\Canvas.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <texts\TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/common/String.hpp>
#include <ctgmath>
#include <cstdlib>
#include <cmath>

using namespace touchgfx;

enum ProductionTestStepState
{
	TEST_STATE_WAITING,
	TEST_STATE_IN_PROGRESS,
	TEST_STATE_USER_REQUIRED,
	TEST_STATE_SUCCESS,
	TEST_STATE_FAILURE
};

class ProductionTestStep : public Container
{
public:
	ProductionTestStep(TEXTS text);
	virtual ~ProductionTestStep();
	
	void SetState(ProductionTestStepState state);
	void SetState(ProductionTestStepState state, int failureDigit);
	int GetTextID();
		
	TypedText Information;
private:			
	ProductionTestStepState _state;	
	int _textID = 0;
	int _failureDigit = 0;	
	
	TextAreaWithOneWildcard TxtInformation;
	static const uint16_t GENERIC_BUFFER_SIZE = 16;
	touchgfx::Unicode::UnicodeChar TxtInformationBuffer[GENERIC_BUFFER_SIZE];

};

#endif