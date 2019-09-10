#ifndef FREDA_PRESENTER_HPP
#define FREDA_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <string>

using namespace touchgfx;

class FREDAView;

class FREDAPresenter : public Presenter, public ModelListener
{
public:
    FREDAPresenter(FREDAView& v);

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

    virtual ~FREDAPresenter() {};

	void SendAndRead(unsigned char command[], int commandSize, unsigned char* result, int& resultLength) { model->SendAndRead(command, commandSize, result, resultLength); }
	
	void Test()
	{

		model->Test();

	}

private:
    FREDAPresenter();
    FREDAView& view;
};


#endif // FREDA_PRESENTER_HPP
