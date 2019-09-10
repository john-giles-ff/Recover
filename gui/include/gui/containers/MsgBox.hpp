#ifndef MSGBOX_HPP
#define MSGBOX_HPP

#include <gui_generated/containers/MsgBoxBase.hpp>

class MsgBox : public MsgBoxBase
{
public:
    MsgBox();
    virtual ~MsgBox() {}

    virtual void initialize();
	void SetReturnedEvent(GenericCallback< const MsgBox &, bool> & callback) { returnedEvent = &callback; }
	void SetText(TEXTS text);
	
	

protected:
	virtual void YesClicked();
	virtual void NoClicked();

private:	

	GenericCallback< const MsgBox &, bool> *returnedEvent;
};

#endif // MSGBOX_HPP
