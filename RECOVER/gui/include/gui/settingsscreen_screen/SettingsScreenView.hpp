#ifndef SETTINGSSCREEN_VIEW_HPP
#define SETTINGSSCREEN_VIEW_HPP

#include <gui_generated/settingsscreen_screen/SettingsScreenViewBase.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>
#include <gui/common/TextRadioButton.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>
#include <gui\containers\Keypad.hpp>
#include <gui\common\Build.hpp>

#include <gui/containers/ViewDiagnostics.hpp>

constexpr int LANGUAGE_COUNT = 8;

constexpr int LANGUAGE_HEIGHT = 75;
constexpr int LANGUAGE_WIDTH = 390;

enum Tab_t
{
	TAB_GENERAL,
	TAB_LANGUAGE,
	TAB_TIME
};


class SettingsScreenView : public SettingsScreenViewBase
{
public:
	SettingsScreenView() :
		LanguageClickedCallback(this, &SettingsScreenView::LanguageClicked),
		MsgBoxReturnedCallback(this, &SettingsScreenView::MsgBoxReturned),		
		IdleScreenOptionCallback(this, &SettingsScreenView::IdleScreenOptionClicked),
		TabClickedCallback(this, &SettingsScreenView::TabClicked)
	{}
    
	virtual ~SettingsScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

	virtual void StartAutoClean();

	virtual void ToggleChamberReadyBeep();			
	virtual void ToggleCipherEnabled();

	virtual void OpenViewDiagnostics();
protected:

	touchgfx::Callback<SettingsScreenView, const TextRadioButton&, void> LanguageClickedCallback;
	void LanguageClicked(const TextRadioButton& u);

	void MsgBoxReturned(const MsgBox& u, bool state);
	touchgfx::Callback<SettingsScreenView, const MsgBox&, bool> MsgBoxReturnedCallback;
	
	void IdleScreenOptionClicked(const touchgfx::AbstractButtonContainer& src);
	touchgfx::Callback<SettingsScreenView, const touchgfx::AbstractButtonContainer&> IdleScreenOptionCallback;

	void TabClicked(const Tab & src);
	touchgfx::Callback<SettingsScreenView, const Tab&> TabClickedCallback;

	TextRadioButton LanguageTexts[LANGUAGE_COUNT]
	{
		TextRadioButton(TypedText(T_LANGUAGE_EN_GB), GB),		
		TextRadioButton(TypedText(T_LANGUAGE_FR), FR),
		TextRadioButton(TypedText(T_LANGUAGE_ES_ES), ES),
		TextRadioButton(TypedText(T_LANGUAGE_IT), IT),
		TextRadioButton(TypedText(T_LANGUAGE_PT), PT),
		TextRadioButton(TypedText(T_LANGUAGE_DE), DE),
		TextRadioButton(TypedText(T_LANGUAGE_RU), RU),
		TextRadioButton(TypedText(T_LANGUAGE_ZN_HANS), ZHS),
	};

private:
	Tab_t currentTab = TAB_GENERAL;
	int selectedIdleMode = 0;


	void UpdateToCurrentTab();
	void UpdateToSelectedIdleMode();

	ViewDiagnostics ViewDiagnosticsWindow;

	
};

#endif // SETTINGSSCREEN_VIEW_HPP
