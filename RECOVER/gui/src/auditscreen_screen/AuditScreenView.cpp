#include <gui/auditscreen_screen/AuditScreenView.hpp>
#include <texts\TextKeysAndLanguages.hpp>


void AuditScreenView::setupScreen()
{	
	//Confirm log delete message box
	confirmDeleteLog.SetText(T_CONFIRMDELETELOG);
	confirmDeleteLog.SetReturnedEvent(DeleteLogMsgBoxReturnedCallback);
	confirmDeleteLog.setVisible(false);
	add(confirmDeleteLog);	

	confirmDeleteAllLogs.SetText(T_CONFIRMDELETEALLLOGS);
	confirmDeleteAllLogs.SetReturnedEvent(DeleteAllMsgBoxReturnedCallback);
	confirmDeleteAllLogs.setVisible(false);
	add(confirmDeleteAllLogs);

	//Initialisation bar		
	logLoadingBar.setXY(100, 215);
	logLoadingBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255, 255, 255));
	logLoadingBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	logLoadingBar.setWidth(600);
	logLoadingBar.setHeight(25);
	logLoadingBar.SetMaximum(LOG_MAX);
	logLoadingBar.SetValue(0);
	logLoadingBar.SetTextVisible(false);	
	logLoadingBar.setVisible(true);
	add(logLoadingBar);		
	
	logsExportingWindow.setVisible(false);	

	TxtNoLogs.setVisible(false);

	//Fetch all the logs
#ifdef SIMULATOR
	LFT::Logs.GetAllHeaders();	
#else
	LFT::Logs.QueGetAllHeaders();	
#endif

	BtnExport.setVisible(false);
	BtnDeleteAll.setVisible(false);

	_logsLoaded = false;

	
	//Russian text is on 2 lines!
	if (Texts::getLanguage() == RU)
		BtnExport.setTextXY(2, 0);
}

void AuditScreenView::tearDownScreen()
{			
	LogList.removeAll();
	for (int i = 0; i < LOG_MAX; i++)
		logs[i].SetLoaded(false);
		
}

void AuditScreenView::handleTickEvent()
{	
	//Update Export Button every 60 Ticks
	counter++;
	if (counter > 60)
	{		
		counter = 0;
	}

	if (logsExportingWindow.isVisible())
		logsExportingWindow.handleTickEvent();

	//Update Progress Bar if visible
	if (logLoadingBar.isVisible())
	{
		logLoadingBar.handleTickEvent();
		if (logLoadingBar.GetValue() != LFT::Logs.GetLogsProgress)
			logLoadingBar.SetValue(LFT::Logs.GetLogsProgress);			
	}
	
	//Wait for logs to finish loading && logs loaded to be false
	if (LFT::Logs.GetLogsProgress < LOG_MAX || _logsLoaded)
		return;
	
	//Hide Progress bar and set logs loaded to true so this only happens once
	_logsLoaded = true;
	logLoadingBar.setVisible(false);
		
		
	//Load logs
	int count = 0;
	for (int i = 0; i < LOG_MAX; i++)
	{
		if (LFT::Logs.LoadedLogs[i].Exists)
		{			
			//Setup main Logs
			logs[i].SetLog(&LFT::Logs.LoadedLogs[i]);
			logs[i].SetDeleteClickedEvent(LogDeleteClickedCallback);
			logs[i].SetResizedEvent(LogResizedCallback);						
			LogList.add(logs[i]);
			
			logs[i].SetLoaded(true);
			count++;
		}
				
	}

	//If there are no logs, show the "No logs" text
	if (count == 0)
	{
		TxtNoLogs.setVisible(true);
		TxtNoLogs.invalidate();
	}

	//Show Delete Button + Export Button
	BtnDeleteAll.setVisible(true);
	BtnExport.setVisible(true);
	
	//Invalidate/resize everything
	AuditLogResized(logs[0]);
	LogScrollableContainer.invalidate();
	logLoadingBar.invalidate();		
}

void AuditScreenView::ExportAll()
{	
#ifndef SIMULATOR
	LFT::Logs.QueExportAll();
#else
	LFT::Logs.ExportAll();
#endif
	


	logsExportingWindow.Reset();
	logsExportingWindow.setVisible(true);
	logsExportingWindow.invalidate();	
}

void AuditScreenView::ShowConfirmDeleteAllLogs()
{
	confirmDeleteAllLogs.setVisible(true);
	confirmDeleteAllLogs.invalidate();
}

void AuditScreenView::AuditLogResized(const AuditLog & u)
{
	//Required to reposition the events after they are expanded.
	//Bit hacky but no other way of doing this that I can see.		
	LogList.setDirection(EAST);
	LogList.setDirection(SOUTH);
	LogScrollableContainer.invalidate();		
}

	
void AuditScreenView::AuditLogDeleteClicked(const AuditLog & u)
{		
	for (int i = 0; i < LOG_MAX; i++)	
		if (&u == &logs[i])
		{
			_logToDeleteIndex = i;
			break;
		}

	confirmDeleteLog.setVisible(true);
	confirmDeleteLog.invalidate();	
	
}



void AuditScreenView::DeleteLogMsgBoxReturned(const MsgBox & u, bool state)
{		


	confirmDeleteLog.setVisible(false);
	confirmDeleteLog.invalidate();

	if (!state)
		return;

	LFT::Logs.DeleteLog(logs[_logToDeleteIndex].Index());	
	LogList.remove(logs[_logToDeleteIndex]);
	AuditLogResized(logs[_logToDeleteIndex]);	
}

void AuditScreenView::DeleteAllMsgBoxReturned(const MsgBox & u, bool state)
{
	confirmDeleteAllLogs.setVisible(false);
	confirmDeleteAllLogs.invalidate();

	if (!state)
		return;

	//Delete All Logs
	LFT::Information.FormatData();

	//Remove all logs from screen
	LogList.removeAll();

	//Show no logs text
	TxtNoLogs.setVisible(true);
	TxtNoLogs.invalidate();


}


	
