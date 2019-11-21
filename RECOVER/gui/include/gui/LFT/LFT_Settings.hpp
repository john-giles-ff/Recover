#ifndef LFT_SETTINGS_HPP
#define LFT_SETTINGS_HPP


#include <gui\model\Model.hpp>
#include <gui\LFT\LFT_Information.hpp>
#include <gui\common\Utils.hpp>
#include <gui\common\LFTDebug.hpp>
#include <gui\common\String.hpp>

#define CIPHER_COUNT 5

class LFT_Settings
{
public:	
	LFT_Settings(LFT_Information * information);

	void SetModel(Model * model);

	//Language Setting
	void SetLanguage(touchgfx::LanguageId languageID);
	touchgfx::LanguageId ReadLanguage();
	touchgfx::LanguageId GetLanguage();

	//Beep When Ready Setting
	void SetBeepWhenReadySetting(bool state);
	bool ReadBeepWhenReadySetting();
	bool GetBeepWhenReadySetting();

	//Recovery State
	void SetRecoveryState(int value);
	int ReadRecoveryState();
	int GetRecoveryState();

	//Use Purge Fans
	void SetUsePurgeFans(bool state);
	bool ReadUsePurgeFans();
	bool GetUsePurgeFans();

	//PC Mode
	void SetPCMode(int value);
	int ReadPCMode();
	int GetPCMode();

	//Lock Mode
	void SetLockMode(int value);
	int ReadLockMode();
	int GetLockMode();	

	//Ciphers
	void SetCiphers(int values[]);
	int* ReadCiphers();
	int* GetCiphers();

private:	
	void WriteInt(int location, int value);
	int ReadInt(int location, int defaultValue);
	int ReadInt(int location);
	void WriteString(int location, String value);
	String ReadString(int location);

	enum SETTING_LOCATIONS
	{
		LANGUAGE_SETTING_LOCATION = 0,
		BEEP_WHEN_READY_SETTING_LOCATION,
		PC_MODE,
		RECOVERY_STATE,
		USE_PURGE_FANS,
		LOCK_MODE,
		LOG_OVERWRITE_WARNING,
		CIPHERS
	};

	const touchgfx::LanguageId DEFAULT_LANGUAGE = GB;
	const bool DEFAULT_BEEP_WHEN_READY = true;
	const int DEFAULT_RUN_COUNTER = 0;
	const int DEFAULT_RECOVERY_STATE = 0;
	const bool DEFAULT_USE_PURGE_FANS = true;
	const int DEFAULT_PC_MODE = 0;
	const int DEFAULT_LOCK_MODE = 1;
	const bool DEFAULT_LOG_OVERWRITE_WARNING = true;

	ThreadSafe<touchgfx::LanguageId> _languageSetting;
	ThreadSafe<bool> _beepWhenReadySetting;
	ThreadSafe<int> _runCounterSetting;
	ThreadSafe<int> _recoveryState;
	ThreadSafe<bool> _usePurgeFans;
	ThreadSafe<int> _pcMode;
	ThreadSafe<int> _lockMode;
	ThreadSafe<bool> _logOverwriteWarning;
	int _ciphers[CIPHER_COUNT];



	Model * _model;
	LFT_Information * _information;
};


#endif /* MODEL_HPP */
