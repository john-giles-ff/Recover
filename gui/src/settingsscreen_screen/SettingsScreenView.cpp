#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

void SettingsScreenView::setupScreen()
{
	Unicode::snprintf(TxtBuildBuffer, TXTBUILD_SIZE, "%d", Build::Number);
	
	//Load Languages onto screen
	int row = 0;
	int column = 0;
	for (int i = 0; i < LANGUAGE_COUNT; i++)
	{		
		//Setup Click Event
		LanguageTexts[i].SetClickEvent(LanguageClickedCallback);		

		//If current language, show as being selected (green text)
		touchgfx::LanguageId selectedId = Texts::getLanguage();
		if (LanguageTexts[i].GetLanguageID() == selectedId)
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(46, 172, 98));
		else
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
		

		//Setup positioning
		int x = column * LANGUAGE_WIDTH;
		int y = row * LANGUAGE_HEIGHT;
		LanguageTexts[i].setXY(x, y);		
		LanguageTexts[i].setWidth(LANGUAGE_WIDTH);
		LanguageTexts[i].setHeight(LANGUAGE_HEIGHT);
		LanguageTexts[i].Text.setWidth(LANGUAGE_WIDTH);
		LanguageTexts[i].Text.setHeight(LANGUAGE_HEIGHT);

		//Increase Column
		column++;
		//If end of row, go to next one
		if (column == 2)
		{
			column = 0;
			row++;
		}		
		LanguageContainer.add(LanguageTexts[i]);
	}
	
	LanguageContainer.invalidate();
}

void SettingsScreenView::tearDownScreen()
{

}

void SettingsScreenView::LanguageClicked(const TextRadioButton & u)
{												
	//Cycle through language Texts, if it's the one that was clicked make it green and change the language to the corresponding language
	//Otherwise change it to white
	for (int i = 0; i < LANGUAGE_COUNT; i++)
	{
		if (&u == &LanguageTexts[i])
		{
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(46, 172, 98));			
			Texts::setLanguage(LanguageTexts[i].GetLanguageID());
		}
		else
			LanguageTexts[i].Text.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));					
	}	

	//Invalidate all the other text on screen
	LanguageContainer.invalidate();
	TxtBuild.invalidate();
	TxtLanguage.invalidate();	
}
