#ifndef ERRORBOX_HPP
#define ERRORBOX_HPP

#include <gui_generated/containers/ErrorBoxBase.hpp>
#include <gui/common/Utils.hpp>

class ErrorBox : public ErrorBoxBase
{
public:
    ErrorBox();
    virtual ~ErrorBox() {}
	virtual void DismissError();

	void SetMessage(TEXTS error);
	void SetMessages(ErrorMessage * messages, int count);
protected:
};

#endif // ERRORBOX_HPP
