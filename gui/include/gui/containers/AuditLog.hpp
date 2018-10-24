#ifndef AUDITLOG_HPP
#define AUDITLOG_HPP

#include <gui_generated/containers/AuditLogBase.hpp>

class AuditLog : public AuditLogBase
{
public:
    AuditLog();
    virtual ~AuditLog() {}

	void SetText(int text);
protected:
};

#endif // AUDITLOG_HPP
