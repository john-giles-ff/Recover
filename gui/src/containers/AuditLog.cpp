#include <gui/containers/AuditLog.hpp>

AuditLog::AuditLog()
{

}

void AuditLog::SetText(int text)
{
	TxtAuditLog.setTypedText(TypedText(text));
}