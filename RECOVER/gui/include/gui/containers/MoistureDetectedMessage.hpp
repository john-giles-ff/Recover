#ifndef MOISTUREDETECTEDMESSAGE_HPP
#define MOISTUREDETECTEDMESSAGE_HPP

#include <gui_generated/containers/MoistureDetectedMessageBase.hpp>

enum class MSGMOISTURE_STATE
{
    DETECTED,
    DRYING
};

class MoistureDetectedMessage : public MoistureDetectedMessageBase
{
public:
    MoistureDetectedMessage();
    virtual ~MoistureDetectedMessage() {}

    void SetState(MSGMOISTURE_STATE state);

    virtual void initialize();
    virtual void Dismiss();

    virtual void StartDrying();
    
protected:
    virtual void handleTickEvent();

private:
    MSGMOISTURE_STATE _state;

    int _tick = 0;
};

#endif // MOISTUREDETECTEDMESSAGE_HPP
