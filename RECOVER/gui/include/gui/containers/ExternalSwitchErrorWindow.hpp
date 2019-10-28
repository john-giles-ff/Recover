#ifndef EXTERNALSWITCHERRORWINDOW_HPP
#define EXTERNALSWITCHERRORWINDOW_HPP

#include <gui_generated/containers/ExternalSwitchErrorWindowBase.hpp>

class ExternalSwitchErrorWindow : public ExternalSwitchErrorWindowBase
{
public:
    ExternalSwitchErrorWindow();
    virtual ~ExternalSwitchErrorWindow() {}
	
	virtual void AllowCancel(bool state);
	virtual void Cancel();

    virtual void initialize();
protected:

private:
	bool _allowCancel;
};

#endif // EXTERNALSWITCHERRORWINDOW_HPP
