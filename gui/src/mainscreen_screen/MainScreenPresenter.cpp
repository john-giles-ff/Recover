#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>



MainScreenPresenter::MainScreenPresenter(MainScreenView& v)
    : view(v)
{
}

void MainScreenPresenter::activate()
{	
	static_cast<FrontendApplication*>(Application::getInstance())->AllowIdle = true;
}

void MainScreenPresenter::deactivate()
{

}

void MainScreenPresenter::ChangeBacklight(int value)
{
	model->SetBacklight(value);
}
