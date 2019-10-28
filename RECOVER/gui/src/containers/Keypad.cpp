#include <gui/containers/Keypad.hpp>

Keypad::Keypad()
	: flexButtonCallback(this, &Keypad::flexButtonCallbackHandler)
{
	Btn0.setAction(flexButtonCallback);
	Btn1.setAction(flexButtonCallback);
	Btn2.setAction(flexButtonCallback);
	Btn3.setAction(flexButtonCallback);
	Btn4.setAction(flexButtonCallback);
	Btn5.setAction(flexButtonCallback);
	Btn6.setAction(flexButtonCallback);
	Btn7.setAction(flexButtonCallback);
	Btn8.setAction(flexButtonCallback);
	Btn9.setAction(flexButtonCallback);
	BtnDel.setAction(flexButtonCallback);
	BtnTick.setAction(flexButtonCallback);	
	BtnBackground.setAction(flexButtonCallback);	
}

void Keypad::initialize()
{
    KeypadBase::initialize();

	currentNumber = 0;
	UpdateNumber();
	UpdateOkButton();
}

int Keypad::GetNumber()
{
	return currentNumber;
}

void Keypad::UpdateNumber()
{
	Unicode::snprintf(TxtCurrentBuffer, TXTCURRENT_SIZE, "%d", currentNumber);
	TxtCurrent.invalidate();

}

void Keypad::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer & src)
{	
	if (&src != &BtnDel && &src != &BtnTick && &src != &BtnBackground)
	{
		if (currentNumber * 10 > 99999999)
			return;

		currentNumber *= 10;
		if (&src == &Btn0)
			currentNumber += 0;
		if (&src == &Btn1)
			currentNumber += 1;
		if (&src == &Btn2)
			currentNumber += 2;
		if (&src == &Btn3)
			currentNumber += 3;
		if (&src == &Btn4)
			currentNumber += 4;
		if (&src == &Btn5)
			currentNumber += 5;
		if (&src == &Btn6)
			currentNumber += 6;
		if (&src == &Btn7)
			currentNumber += 7;
		if (&src == &Btn8)
			currentNumber += 8;
		if (&src == &Btn9)
			currentNumber += 9;		
	}
	else
	{
		if (&src == &BtnDel)
		{
			currentNumber = currentNumber - (currentNumber % 10);
			currentNumber /= 10;
		}
		if (&src == &BtnTick)
		{
			if (returnedEvent)
				returnedEvent->execute(*this, true);
		}
		if (&src == &BtnBackground)
		{
			if (returnedEvent)
				returnedEvent->execute(*this, false);
		}
	}

	UpdateNumber();
	UpdateOkButton();
}

void Keypad::UpdateOkButton()
{
	if (currentNumber > 9999999)
	{
		BtnTick.setTouchable(true);
		BtnTick.setBoxWithBorderColors(
			touchgfx::Color::getColorFrom24BitRGB(0, 0, 0),
			touchgfx::Color::getColorFrom24BitRGB(46, 172, 98),
			touchgfx::Color::getColorFrom24BitRGB(46, 172, 98),
			touchgfx::Color::getColorFrom24BitRGB(46, 172, 98));
		BtnTick.invalidate();
	}
	else
	{
		BtnTick.setTouchable(false);
		BtnTick.setBoxWithBorderColors(
			touchgfx::Color::getColorFrom24BitRGB(0, 0, 0),
			touchgfx::Color::getColorFrom24BitRGB(46, 46, 46),
			touchgfx::Color::getColorFrom24BitRGB(46, 46, 46),
			touchgfx::Color::getColorFrom24BitRGB(46, 46, 46));
		BtnTick.invalidate();
	}


}

