#include <gui/common/CustomKeyboard.hpp>

#include <touchgfx/Color.hpp>
#include <touchgfx/Utils.hpp>

CustomKeyboard::CustomKeyboard() : keyboard(),
    modeBtnTextArea1(),
	modeBtnTextArea2(),
    capslockPressed(this, &CustomKeyboard::capslockPressedHandler),
    backspacePressed(this, &CustomKeyboard::backspacePressedHandler),
    modePressed(this, &CustomKeyboard::modePressedHandler),
	enterPressed(this, &CustomKeyboard::enterPressedHandler),
    keyPressed(this, &CustomKeyboard::keyPressedhandler),
    alphaKeys(true),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    layout.callbackAreaArray[0].callback = &capslockPressed;
    layout.callbackAreaArray[1].callback = &backspacePressed;
    layout.callbackAreaArray[2].callback = &modePressed;
	layout.callbackAreaArray[3].callback = &modePressed;
	layout.callbackAreaArray[4].callback = &enterPressed;
	
    keyboard.setLayout(&layout);
    keyboard.setKeyListener(keyPressed);
    keyboard.setPosition(0, 0, 560, 280);
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.    
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = true;
    firstCharacterEntry = true;

    modeBtnTextArea1.setPosition(10, 235, 75, 45);
    modeBtnTextArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
	modeBtnTextArea2.setPosition(475, 235, 75, 45);
	modeBtnTextArea2.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));

    setKeyMappingList();
	

    add(keyboard);
    add(modeBtnTextArea1);
	add(modeBtnTextArea2);
}

void CustomKeyboard::setKeyMappingList()
{
    if (alphaKeys)
    {
        modeBtnTextArea1.setTypedText(TypedText(T_ALPHAMODE));
		modeBtnTextArea2.setTypedText(TypedText(T_ALPHAMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListAlphaUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListAlphaLower);
        }
    }
    else
    {
        modeBtnTextArea1.setTypedText(TypedText(T_NUMMODE));
		modeBtnTextArea2.setTypedText(TypedText(T_NUMMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListNumUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListNumLower);
        }
    }
}

void CustomKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        //Delete the previous entry in the buffer and decrement the position.
        buffer[pos - 1] = 0;
        keyboard.setBufferPosition(pos - 1);

        //Change keymappings if we have reached the first position.
        if (1 == pos)
        {
            firstCharacterEntry = true;
            uppercaseKeys = true;
            setKeyMappingList();
        }
    }
}

void CustomKeyboard::capslockPressedHandler()
{
    uppercaseKeys = !uppercaseKeys;
    setKeyMappingList();
}

void CustomKeyboard::modePressedHandler()
{
    alphaKeys = !alphaKeys;

    // if we have changed back to alpha and still has no chars in the buffer,
    // we show upper case letters.
    if (firstCharacterEntry && alphaKeys)
    {
        uppercaseKeys = true;
    }
    else
    {
        uppercaseKeys = false;
    }
    setKeyMappingList();
}

void CustomKeyboard::enterPressedHandler()
{
	if (enterEvent && enterEvent->isValid())
		enterEvent->execute(*this);
}

void CustomKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
    // After the first keypress, the keyboard will shift to lowercase.
    if (firstCharacterEntry && keyChar != 0)
    {
        firstCharacterEntry = false;
        uppercaseKeys = false;
        setKeyMappingList();
    }
}

void CustomKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

uint16_t* CustomKeyboard::getBuffer()
{		
	return keyboard.getBuffer();
}

int CustomKeyboard::getBufferSize()
{
	return keyboard.getBufferPosition();
}


