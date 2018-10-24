#ifndef SETTINGSSCREEN_VIEW_HPP
#define SETTINGSSCREEN_VIEW_HPP

#include <gui_generated/settingsscreen_screen/SettingsScreenViewBase.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>
#include <gui/common/TextRadioButton.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <gui\common\Build.hpp>

constexpr int LANGUAGE_COUNT = 2;

constexpr int LANGUAGE_HEIGHT = 50;
constexpr int LANGUAGE_WIDTH = 300;

class SettingsScreenView : public SettingsScreenViewBase
{
public:
	SettingsScreenView() : LanguageClickedCallback(this, &SettingsScreenView::LanguageClicked) {}
    virtual ~SettingsScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:

	touchgfx::Callback<SettingsScreenView, const TextRadioButton&, void> LanguageClickedCallback;
	void LanguageClicked(const TextRadioButton& u);

	TextRadioButton LanguageTexts[LANGUAGE_COUNT]
	{
		TextRadioButton(TypedText(T_LANGUAGE_EN_GB), GB),		
		TextRadioButton(TypedText(T_LANGUAGE_ZN_HANS), ZHS)
	};
	
};

#endif // SETTINGSSCREEN_VIEW_HPP
