#include <gui/containers/ErrorBox.hpp>

ErrorBox::ErrorBox()
{
	Backdrop.setAlpha(140);			
}

void ErrorBox::DismissError()
{
	setVisible(false);
	invalidate();
}

void ErrorBox::SetMessage(TEXTS error)
{
	//Set Error Message
	TxtErrorMessage.setTypedText(TypedText(error));
	TxtErrorCode.setVisible(false);

	//Enable Wrapping on error message
	TxtErrorMessage.setWideTextAction(touchgfx::WIDE_TEXT_WORDWRAP);

	//Invalidate 
	TxtErrorMessage.invalidate();
	TxtErrorCode.invalidate();

}

void ErrorBox::SetMessages(ErrorMessage * messages, int count)
{
	//Check for actionable message and get error code
	ErrorMessage actionableMessage;
	String fullErrorCode = "";
	for (int i = 0; i < count; i++)
	{
		//Shouldn't happen, but just in case
		if (!messages[i].exists)
			continue;

		//Get first actionable message
		if (!actionableMessage.exists && messages[i].isActionable)		
			actionableMessage = messages[i];			

		//Get Error ID as padded string
		String errorCode = String(messages[i].ErrorID);
		while (errorCode.len() < 2)		
			errorCode = "0" + errorCode;

		//Add ErrorCode to full string
		fullErrorCode += errorCode;				
	}
	
	//Set Error Message
	if (actionableMessage.exists)	
		TxtErrorMessage.setTypedText(TypedText(actionableMessage.ErrorText));	
	else	
		TxtErrorMessage.setTypedText(TypedText(T_ERROR_GENERIC));


	//Convert Error Code and put it on screen
	uint16_t destination[20];
	Unicode::fromUTF8((const uint8_t *)fullErrorCode.c_str(), destination, TXTERRORCODE_SIZE);
	Unicode::snprintf(TxtErrorCodeBuffer, TXTERRORCODE_SIZE, "%s", destination);
	

	//Enable Wrapping on error message
	TxtErrorMessage.setWideTextAction(touchgfx::WIDE_TEXT_WORDWRAP);

	//Invalidate 
	TxtErrorMessage.invalidate();
	TxtErrorCode.invalidate();

	
}
