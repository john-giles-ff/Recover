#ifndef FILTERSTATEMESSAGE_HPP
#define FILTERSTATEMESSAGE_HPP

#include <gui_generated/containers/FilterStateMessageBase.hpp>
#include <gui/LFT/LFT.hpp>

class FilterStateMessage : public FilterStateMessageBase
{
public:
    FilterStateMessage();
    virtual ~FilterStateMessage() {}

    virtual void initialize();
	virtual void Dismiss();
protected:
};

#endif // FILTERSTATEMESSAGE_HPP
