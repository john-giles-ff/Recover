#ifndef AUDITLOG_HPP
#define AUDITLOG_HPP

#include <gui_generated/containers/AuditLogBase.hpp>
#include <gui\LFT\LFT.hpp>
#include <gui\LFT\LFT_Logs.hpp>
#include <touchgfx/Unicode.hpp>
#include <BitmapDatabase.hpp>

class AuditLog : public AuditLogBase
{
public:
	AuditLog();    
    virtual ~AuditLog() {}
	
	void SetLog(RecoverLog * log);
	int Index() { return _log->Index;  }
	bool Exists() { return _log->Exists; }

	virtual void ToggleExpanded();
	virtual void Delete();
	virtual void Advanced();

	void SetExpanded(bool state);
	void SetLoaded(bool state);

	virtual void SetResizedEvent(GenericCallback< const AuditLog &> & callback) { resizedEvent = &callback; }
	virtual void SetDeleteClickedEvent(GenericCallback< const AuditLog &> & callback) { deleteClickedEvent = &callback; }

protected:

private:
	void UpdateExpandedState();


	RecoverLog *_log;
	colortype GetColorFromBool(bool state);

	bool _isLoaded = false;
	bool _isExpanded = false;
	const int ExpandedSize = 400;
	const int CollapsedSize = 80;

	GenericCallback< const AuditLog &> *resizedEvent;
	GenericCallback< const AuditLog &> *deleteClickedEvent;
};

#endif // AUDITLOG_HPP
