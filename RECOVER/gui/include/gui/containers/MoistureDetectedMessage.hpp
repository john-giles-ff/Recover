#ifndef MOISTUREDETECTEDMESSAGE_HPP
#define MOISTUREDETECTEDMESSAGE_HPP

#include <gui_generated/containers/MoistureDetectedMessageBase.hpp>


class MoistureDetectedMessage : public MoistureDetectedMessageBase
{
public:
    MoistureDetectedMessage();
    virtual ~MoistureDetectedMessage() {}

    virtual void initialize();
    virtual void Dismiss();

    virtual void StartDrying();
    
protected:
    virtual void handleTickEvent();

private:

    int _tick = 0;
    bool _running = false;
};

#endif // MOISTUREDETECTEDMESSAGE_HPP
