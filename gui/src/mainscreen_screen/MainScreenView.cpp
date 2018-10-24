#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>


MainScreenView::MainScreenView()
{

}

void MainScreenView::setupScreen()
{		
	if (application().isEngineerModeOn)
	{
		TxtEngineerWindow.setTypedText(TypedText(T_ENGINEERMODE));
		TxtEngineerWindow.setXY(100, 100);
		TxtEngineerWindow.resizeToCurrentText();
		TxtEngineerWindow.setColor(Color::getColorFrom24BitRGB(255, 255, 255));

		add(TxtEngineerWindow);
	}
}

void MainScreenView::tearDownScreen()
{

}
