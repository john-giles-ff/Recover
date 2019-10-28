#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <touchgfx/Color.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>


MainScreenView::MainScreenView()
{

}

void MainScreenView::setupScreen()
{		
	if (LFT::Information.EngineeringMode)
	{
		TxtEngineerWindow.setTypedText(TypedText(T_ENGINEERMODE));
		TxtEngineerWindow.setXY(100, 100);
		TxtEngineerWindow.resizeToCurrentText();
		TxtEngineerWindow.setColor(Color::getColorFrom24BitRGB(255, 255, 255));

		add(TxtEngineerWindow);
	}

	FilterStateMessageWindow.setXY(0, 0);
	FilterStateMessageWindow.setVisible(false);
	add(FilterStateMessageWindow);

	//Setup Filter Control
	switch (LFT::Information.GetFilterValue())
	{
	case 0:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_GOOD_ID), Bitmap(BITMAP_FILTER_GOOD_ID));
		break;
	case 1:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_MEDIUM_ID), Bitmap(BITMAP_FILTER_MEDIUM_ID));
		break;
	case 2:
		BtnFilter.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_FILTER_BAD_ID), Bitmap(BITMAP_FILTER_BAD_ID));
		break;
	}

}

void MainScreenView::tearDownScreen()
{

}

void MainScreenView::ShowFilterState()
{
	FilterStateMessageWindow.setVisible(true);
	FilterStateMessageWindow.invalidate();
}
