#include <gui/auditscreen_screen/AuditScreenView.hpp>
#include <texts\TextKeysAndLanguages.hpp>

AuditScreenView::AuditScreenView()
{

}

void AuditScreenView::setupScreen()
{
	LogScrollableContainer.enableHorizontalScroll(false);

	for (int i = 0; i < 50; i++)
	{
		if (i % 3 == 0)
			logs[i].SetText(T_AUDITLOGEXAMPLE1);
		if (i % 3 == 1)
			logs[i].SetText(T_AUDITLOGEXAMPLE2);
		if (i % 3 == 2)
			logs[i].SetText(T_AUDITLOGEXAMPLE3);

		LogList.add(logs[i]);
	}
}

void AuditScreenView::tearDownScreen()
{

}
