#ifndef TAB_HPP
#define TAB_HPP

#include <gui_generated/containers/TabBase.hpp>

class Tab : public TabBase
{
public:
    Tab();
    virtual ~Tab() {}

    virtual void initialize();

	void SetText(TypedText text);

	void SetSelected(bool state);
	bool GetSelected();

	virtual void handleClickEvent(const ClickEvent& evt);

	void SetClickedAction(GenericCallback<const Tab &> &callback);
protected:

private: 
	bool isSelected;
	GenericCallback<const Tab &> *ClickedCallback;

	
};

#endif // TAB_HPP
