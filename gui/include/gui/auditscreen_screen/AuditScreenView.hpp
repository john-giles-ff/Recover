#ifndef AUDITSCREEN_VIEW_HPP
#define AUDITSCREEN_VIEW_HPP

#include <gui_generated/auditscreen_screen/AuditScreenViewBase.hpp>
#include <gui/auditscreen_screen/AuditScreenPresenter.hpp>
#include <gui/containers/AuditLog.hpp>
#include <gui/containers/MsgBox.hpp>

#include <gui/common/ProgressBar.hpp>
#include <gui/LFT/LFT_Logs.hpp>

class AuditScreenView : public AuditScreenViewBase
{
public:
	AuditScreenView() :
		LogResizedCallback(this, &AuditScreenView::AuditLogResized),
		LogDeleteClickedCallback(this, &AuditScreenView::AuditLogDeleteClicked),
		DeleteLogMsgBoxReturnedCallback(this, &AuditScreenView::DeleteLogMsgBoxReturned),
		DeleteAllMsgBoxReturnedCallback(this, &AuditScreenView::DeleteAllMsgBoxReturned)
	{} 
    virtual ~AuditScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	virtual void handleTickEvent();			
	
	virtual void ExportAll();

	virtual void ShowConfirmDeleteAllLogs();	

protected:			
	AuditLog logs[LOG_MAX];	
	ProgressBar logLoadingBar;
	MsgBox confirmDeleteLog;
	MsgBox confirmDeleteAllLogs;
	


private:	
	int counter = 0;	

	bool _logsLoaded = false;

	void AuditLogResized(const AuditLog& u);
	touchgfx::Callback<AuditScreenView, const AuditLog&, void> LogResizedCallback;

	void AuditLogDeleteClicked(const AuditLog& u);
	touchgfx::Callback<AuditScreenView, const AuditLog&, void> LogDeleteClickedCallback;

	void DeleteLogMsgBoxReturned(const MsgBox& u, bool state);
	touchgfx::Callback<AuditScreenView, const MsgBox&, bool> DeleteLogMsgBoxReturnedCallback;

	void DeleteAllMsgBoxReturned(const MsgBox& u, bool state);
	touchgfx::Callback<AuditScreenView, const MsgBox&, bool> DeleteAllMsgBoxReturnedCallback;	

	int _logToDeleteIndex;
	/*
	*/
};

#endif // AUDITSCREEN_VIEW_HPP
