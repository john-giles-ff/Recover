#include <gui/containers/MoistureDetectedMessage.hpp>
#include <gui/LFT/LFT.hpp>

MoistureDetectedMessage::MoistureDetectedMessage()
{
    txtMessage.setWideTextAction(touchgfx::WideTextAction::WIDE_TEXT_WORDWRAP);
    txtDrying.setWideTextAction(touchgfx::WideTextAction::WIDE_TEXT_WORDWRAP);

    SetState(MSGMOISTURE_STATE::DETECTED);

    Application::getInstance()->registerTimerWidget(this);

}

void MoistureDetectedMessage::SetState(MSGMOISTURE_STATE state)
{
    _state = state;

    bool isAbleToDry = LFT::Information.ManifoldVersion > MANIFOLD_VERSION::V1;

    BtnStartDrying.setVisible(isAbleToDry);
    conStart.setVisible(state == MSGMOISTURE_STATE::DETECTED);
    conDrying.setVisible(state == MSGMOISTURE_STATE::DRYING);


    invalidate();   
}

void MoistureDetectedMessage::initialize()
{
    MoistureDetectedMessageBase::initialize();
}

void MoistureDetectedMessage::Dismiss()
{
    this->setVisible(false);
    this->invalidate();
}

void MoistureDetectedMessage::StartDrying()
{
    pbrDrying.setValue(0);
    pbrDrying.invalidate();


    LFT::Auto.StartDrying();

    SetState(MSGMOISTURE_STATE::DRYING);

}

void MoistureDetectedMessage::handleTickEvent()
{
    if (_state != MSGMOISTURE_STATE::DRYING)
        return;

    _tick++;
    _tick %= 60 * 4;

    if (_tick != 0)
        return;

    int dryingPercentage = LFT::Auto.ReadDryingPercentage();
    
    pbrDrying.setValue(dryingPercentage);
    pbrDrying.invalidate();

    if (dryingPercentage > 99)
    {
        SetState(MSGMOISTURE_STATE::DETECTED);
        Dismiss();
    }


}
