#include <gui/containers/MoistureDetectedMessage.hpp>
#include <gui/LFT/LFT.hpp>

MoistureDetectedMessage::MoistureDetectedMessage()
{
    txtDrying.setWideTextAction(touchgfx::WideTextAction::WIDE_TEXT_WORDWRAP);    

    Application::getInstance()->registerTimerWidget(this);

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
    if (_running)
        return;

    setVisible(true);
    pbrDrying.setValue(0);
    invalidate();
    
    _running = true;
    

}

void MoistureDetectedMessage::handleTickEvent()
{
    //Only run this tick every 4 seconds
    _tick++;
    _tick %= 60 * 4;
    if (_tick != 0)
        return;


    //Get Percentage
    int dryingPercentage = LFT::Auto.ReadDryingPercentage();
    
    //Print
    pbrDrying.setValue(dryingPercentage);
    pbrDrying.invalidate();

    //Finish
    if (dryingPercentage > 99)
    {        
        Dismiss();
    }


}
