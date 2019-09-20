#include <gui/containers/LogsExporting.hpp>
#include <BitmapDatabase.hpp>

LogsExporting::LogsExporting()
{	
	TxtError.setWideTextAction(WIDE_TEXT_WORDWRAP);
	TxtError.setTypedText(TypedText(T_LOGSEXPORTINGERROR));
	Reset();	
}

void LogsExporting::handleTickEvent()
{	
	//Animate Spinner
	spinnerCounter++;

	float value = pow(cos(spinnerCounter / 19.0f), 2.0f);
	


	int degree = spinnerCounter * 8;
	int size = (int)(value * 60.0f);
	
	Spinner.setStartEndAngle(degree, degree + 10 + size);	
	Spinner.invalidate();	


	//Check if export has finished
	counter++;
	if (counter > 60)
	{
		counter = 0;

		//If logs have finished!
		if (!LFT::Logs.ExportAllRequired)
		{
			Spinner.setVisible(false);

			if (LFT::Logs.ExportLogsResult == 0)
				ShowFinished();
			else
				ShowError();

		}		
	}
}

void LogsExporting::initialize()
{
    LogsExportingBase::initialize();
}

void LogsExporting::Reset()
{
	BtnExitExporting.setVisible(false);
	ImgError.setVisible(false);
	ImgInformation.setVisible(true);
	TxtError.setVisible(false);
	TxtExportingLogs.setVisible(true);
	TxtExportingLogs.setTypedText(TypedText(T_LOGSEXPORTING));
	Spinner.setVisible(true);
	invalidate();
}

void LogsExporting::HideWindow()
{
	setVisible(false);
	invalidate();
}

void LogsExporting::ShowFinished()
{
	TxtExportingLogs.setTypedText(TypedText(T_LOGSEXPORTINGFINISHED));
	BtnExitExporting.setVisible(true);
	ExportWindowBorder.invalidate();
}

void LogsExporting::ShowError()
{	
	
	//TxtExportingLogs.setTypedText(TypedText(T_LOGSEXPORTINGERROR));
	Unicode::snprintf(TxtErrorBuffer, TXTERROR_SIZE, "%d", (int)LFT::Logs.ExportLogsResult);
	TxtExportingLogs.setVisible(false);
	TxtError.setVisible(true);
	ImgError.setVisible(true);
	ImgInformation.setVisible(false);
	BtnExitExporting.setVisible(true);
	ExportWindowBorder.invalidate();
}

