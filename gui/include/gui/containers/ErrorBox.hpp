#ifndef ERRORBOX_HPP
#define ERRORBOX_HPP

#include <gui_generated/containers/ErrorBoxBase.hpp>

class ErrorBox : public ErrorBoxBase
{
public:
    ErrorBox();
    virtual ~ErrorBox() {}
	virtual void DismissError();

	void SetText(TypedText text);
protected:
};

#endif // ERRORBOX_HPP
