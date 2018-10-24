#ifndef MAINSCREEN_VIEW_HPP
#define MAINSCREEN_VIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>
#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>
#include <touchgfx/widgets/TextArea.hpp>


class MainScreenView : public MainScreenViewBase
{
public:
    MainScreenView();
    virtual ~MainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();			
protected:	
	TextArea TxtEngineerWindow;
	

	
};

#endif // MAINSCREEN_VIEW_HPP
