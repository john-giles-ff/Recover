#ifndef INTERNALSWITCHERROR_HPP
#define INTERNALSWITCHERROR_HPP

#include <gui_generated/containers/InternalSwitchErrorBase.hpp>

class InternalSwitchError : public InternalSwitchErrorBase
{
public:
    InternalSwitchError();	
    virtual ~InternalSwitchError() {}

    virtual void initialize();
	void SetState(bool state);

	virtual void Cancel();
protected:
	bool _state;
};

#endif // INTERNALSWITCHERROR_HPP
