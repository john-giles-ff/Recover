#ifndef AUDITSCREEN_VIEW_HPP
#define AUDITSCREEN_VIEW_HPP

#include <gui_generated/auditscreen_screen/AuditScreenViewBase.hpp>
#include <gui/auditscreen_screen/AuditScreenPresenter.hpp>
#include <gui/containers/AuditLog.hpp>

class AuditScreenView : public AuditScreenViewBase
{
public:
    AuditScreenView();
    virtual ~AuditScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:	
	AuditLog logs[50];

};

#endif // AUDITSCREEN_VIEW_HPP
