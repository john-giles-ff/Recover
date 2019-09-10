#ifndef LOGSEXPORTING_HPP
#define LOGSEXPORTING_HPP

#include <gui_generated/containers/LogsExportingBase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui\LFT\LFT.hpp>

class LogsExporting : public LogsExportingBase
{
public:
    LogsExporting();
    virtual ~LogsExporting() {}
	virtual void handleTickEvent();
    virtual void initialize();

	void Reset();	
	virtual void HideWindow();
protected:
	void ShowFinished();
	void ShowError();
	int counter = 0;	

	int spinnerCounter = 0;
};

#endif // LOGSEXPORTING_HPP
