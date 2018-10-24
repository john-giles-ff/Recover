#ifndef IDLESCREEN_PRESENTER_HPP
#define IDLESCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class IdleScreenView;

class IdleScreenPresenter : public Presenter, public ModelListener
{
public:
    IdleScreenPresenter(IdleScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

	void DisallowScreensaver();
	void SetBacklight(float value);

	int ReadBaseTemperature() { return model->GetBaseTemp(); }
	int ReadPreTemperature() { return model->GetPreTemp(); }
	float ReadPressure() { return model->GetPressure(); }
	DateTime ReadTime() { return model->GetTime(); }
	int GetProgress() { return model->GetProgress(); }
	int GetStatus() { return model->GetStatus(); }	
	float lastError() { return model->error; }
	float reads() { return model->reads; }
	void UpdateLFTDebug(LFTDebug *control) { model->UpdateLFTDebug(control); }

	bool GetRefreshValuesRequired() { return model->GetRefreshValuesRequired(); }
	int GetRefreshProgress() { return model->GetRefreshProgress(); }
	int GetRefreshMaximum() { return model->GetRefreshMaximum(); }
	


    virtual ~IdleScreenPresenter() {};

private:
    IdleScreenPresenter();

    IdleScreenView& view;
};


#endif // IDLESCREEN_PRESENTER_HPP
