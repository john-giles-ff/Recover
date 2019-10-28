#include <gui/idlescreen_screen/IdleScreenView.hpp>
#include <gui/idlescreen_screen/IdleScreenPresenter.hpp>

IdleScreenPresenter::IdleScreenPresenter(IdleScreenView& v)
    : view(v)
{
}

void IdleScreenPresenter::activate()
{	
}

void IdleScreenPresenter::deactivate()
{

}

void IdleScreenPresenter::DisallowScreensaver()
{
	static_cast<FrontendApplication*>(Application::getInstance())->AllowIdle = false;
}

void IdleScreenPresenter::SetBacklight(float value)
{
	model->SetBacklight(value);
}


