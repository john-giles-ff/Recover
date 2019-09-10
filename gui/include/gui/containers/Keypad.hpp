#ifndef KEYPAD_HPP
#define KEYPAD_HPP

#include <gui_generated/containers/KeypadBase.hpp>

class Keypad : public KeypadBase
{
public:
    Keypad();
    virtual ~Keypad() {}

    virtual void initialize();

	void SetReturnedEvent(GenericCallback< const Keypad &, bool> & callback) { returnedEvent = &callback; }
	int GetNumber();
protected:

private:
	void UpdateNumber();
	int currentNumber;

	GenericCallback< const Keypad &, bool> *returnedEvent;

	void flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src);
	touchgfx::Callback<Keypad, const touchgfx::AbstractButtonContainer&> flexButtonCallback;

	void UpdateOkButton();
};

#endif // KEYPAD_HPP
