#ifndef TEXTRADIOBUTTON_HPP
#define TEXTRADIOBUTTON_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/Widgets/TextArea.hpp>
#include <BitmapDatabase.hpp>

using namespace touchgfx;

class TextRadioButton : public Container
{
public:

	TextRadioButton(TypedText text, touchgfx::LanguageId languageID);
    virtual ~TextRadioButton();

	TextArea Text;	
	
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);	

	touchgfx::LanguageId GetLanguageID() { return _languageID; }
	   
	virtual void SetClickEvent(GenericCallback< const TextRadioButton &> & callback) { clickEvent = &callback; }	

protected:	


private:
	touchgfx::LanguageId _languageID;
	GenericCallback< const TextRadioButton &> *clickEvent;	
	
};


#endif /* CIRCULARPROGRESS_HPP */
