#include <gui/processscreen_screen/ProcessScreenView.hpp>
#include <gui/processscreen_screen/ProcessScreenPresenter.hpp>

ProcessScreenPresenter::ProcessScreenPresenter(ProcessScreenView& v)
    : view(v)
{
}

void ProcessScreenPresenter::activate()
{
	
}

void ProcessScreenPresenter::deactivate()
{

}


void ProcessScreenPresenter::SetScreensaverState(bool state)
{
	static_cast<FrontendApplication*>(Application::getInstance())->AllowIdle = state;
}