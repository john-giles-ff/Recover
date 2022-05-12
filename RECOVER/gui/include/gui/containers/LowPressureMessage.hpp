#ifndef LOWPRESSUREMESSAGE_HPP
#define LOWPRESSUREMESSAGE_HPP

#include <gui_generated/containers/LowPressureMessageBase.hpp>

class LowPressureMessage : public LowPressureMessageBase
{
public:
    LowPressureMessage();
    virtual ~LowPressureMessage() {}

    virtual void initialize();
protected:
};

#endif // LOWPRESSUREMESSAGE_HPP
