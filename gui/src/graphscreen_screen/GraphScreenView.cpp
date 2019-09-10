#include <gui/graphscreen_screen/GraphScreenView.hpp>

void GraphScreenView::setupScreen()
{
    GraphScreenViewBase::setupScreen();

	//Setup Graph Controllers
	TglBase.forceState(true);
	TglPrecursor.forceState(true);
	TglPressure.forceState(true);

	//Setup Loading Bar
	progressBar.setXY(100, 215);
	progressBar.SetBackgroundColor(Color::getColorFrom24BitRGB(255, 255, 255));
	progressBar.SetForegroundColor(Color::getColorFrom24BitRGB(46, 172, 98));
	progressBar.setWidth(600);
	progressBar.setHeight(25);
	progressBar.SetMaximum(LFT::Logs.GetSampleQueLogs()->NumberOfSamples / 10);
	progressBar.SetValue(0);
	progressBar.SetTextVisible(false);
	progressBar.setVisible(true);
	add(progressBar);

	//Setup Graphs
	coverGraph.setXY(0, 69);
	pressureGraph.setXY(0, 69);
	precursorGraph.setXY(0, 69);
	baseGraph.setXY(0, 69);
	coverGraph.setup(800, 296, Color::getColorFrom24BitRGB(0x00, 0xFF, 0x00), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	pressureGraph.setup(800, 296, Color::getColorFrom24BitRGB(0x00, 0xFF, 0x00), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	precursorGraph.setup(800, 296, Color::getColorFrom24BitRGB(0xFF, 0x00, 0x00), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	baseGraph.setup(800, 296, Color::getColorFrom24BitRGB(0x00, 0x00, 0xFF), Color::getColorFrom24BitRGB(0x00, 0x00, 0x00));
	coverGraph.setVisible(false);
	pressureGraph.setVisible(false);
	precursorGraph.setVisible(false);
	baseGraph.setVisible(false);
	coverGraph.setDotsVisible(false);
	pressureGraph.setDotsVisible(false);
	precursorGraph.setDotsVisible(false);
	baseGraph.setDotsVisible(false);
	coverGraph.setDotsBackgroundVisible(false);
	pressureGraph.setDotsBackgroundVisible(false);
	precursorGraph.setDotsBackgroundVisible(false);
	baseGraph.setDotsBackgroundVisible(false);
	coverGraph.setAreaVisible(false);
	pressureGraph.setAreaVisible(false);
	precursorGraph.setAreaVisible(false);
	baseGraph.setAreaVisible(false);
	UpdateRange(LFT::Logs.GetSampleQueLogs()->NumberOfSamples);

	//Setup Background boxes
	for (int i = 0; i < modeCount; i++)
	{
		backgroundBoxes[i].setAlpha(0xA5);
		backgroundBoxes[i].setVisible(false);
		backgroundBoxes[i].setColor(backgroundBoxesColors[i]);
		backgroundBoxes[i].setXY(0, 69);
		backgroundBoxes[i].setHeight(296);
		backgroundBoxes[i].setWidth(0);
		add(backgroundBoxes[i]);
	}		
	
	add(coverGraph);
	add(pressureGraph);
	add(precursorGraph);
	add(baseGraph);

#ifdef SIMULATOR
	progressBar.setVisible(false);
	UpdateGraph();
#endif
		
}

void GraphScreenView::tearDownScreen()
{	
    GraphScreenViewBase::tearDownScreen();
}

void GraphScreenView::handleTickEvent()
{		
	if (progressBar.isVisible())
	{		
		progressBar.handleTickEvent();		
		SampleLoaded(LFT::Logs.LoadedSampleCount / 10);				
	}

	if (requiresInvalidate)
	{		
		requiresInvalidate = false;
		baseGraph.invalidate();		
	}
}

void GraphScreenView::TogglePressure()
{
	pressureGraph.setVisible(TglPressure.getState());
	pressureGraph.invalidate();
}

void GraphScreenView::TogglePrecursor()
{
	precursorGraph.setVisible(TglPrecursor.getState());
	precursorGraph.invalidate();
}

void GraphScreenView::ToggleBase()
{
	baseGraph.setVisible(TglBase.getState());
	baseGraph.invalidate();
}

void GraphScreenView::Exit()
{
	LFT::Logs.AbortSampleLoading();
	application().gotoAuditScreenNoTransition();
}

void GraphScreenView::UpdateGraph()
{
	coverGraph.setVisible(false);
	pressureGraph.setVisible(false);	
	precursorGraph.setVisible(false);	
	baseGraph.setVisible(false);

	pressureGraph.clear();
	precursorGraph.clear();
	baseGraph.clear();
	for (int i = 0; i < LFT::Logs.GetSampleQueLogs()->NumberOfSamples; i++)	
	{		
		if (!LFT::Logs.LoadedSamples[i].Exists)
			continue;

		int mode = LFT::Logs.LoadedSamples[i].Mode;
		int x = LFT::Logs.LoadedSamples[i].SampleNumber;
		int realX = GetXRealPosition(x);
		
		pressureGraph.addValue(x, (int)LFT::Logs.LoadedSamples[i].Pressure);				
		precursorGraph.addValue(x, (int)LFT::Logs.LoadedSamples[i].PrecursorTemperature);
		baseGraph.addValue(x, (int)LFT::Logs.LoadedSamples[i].BaseTemperature);
		if (mode != 0)
		{
			if (LFT::Logs.LoadedSamples[i].Mode != SAMPLE_UNKNOWN && LFT::Logs.LoadedSamples[i - 1].Mode != SAMPLE_UNKNOWN)
			{
				if (LFT::Logs.LoadedSamples[i - 1].Mode != mode)
				{
					Box previous = backgroundBoxes[mode - 1];
					backgroundBoxes[mode].setX(previous.getX() + previous.getWidth());
					
				}
			}
		}

		if (mode == SAMPLE_UNKNOWN)
			backgroundBoxes[i].setColor(Color::getColorFrom24BitRGB(255, 255, 255));

		backgroundBoxes[mode].setWidth(realX - backgroundBoxes[mode].getX());

	}
	
	//Set everything visibe and update it	
	coverGraph.setVisible(true);
	pressureGraph.setVisible(true);				
	precursorGraph.setVisible(true);	
	baseGraph.setVisible(true);	
	for (int i = 0; i < modeCount; i++)	
		backgroundBoxes[i].setVisible(true);		
		
	requiresInvalidate = true;
}

void GraphScreenView::SampleLoaded(int u)
{	
	progressBar.SetValue(u);

	if (!LFT::Logs.GetSamplesRequired)
	{
		progressBar.setVisible(false);
		progressBar.invalidate();
		UpdateGraph();
	}	
}

void GraphScreenView::UpdateRange(int width)
{
	if (width == -1)
		width = 1000;

	pressureGraph.setRange(0, width, 2100, 0);
	precursorGraph.setRange(0, width, 210, 0);
	baseGraph.setRange(0, width, 210, 0);
}

int GraphScreenView::GetXRealPosition(int x)
{
	return precursorGraph.WidgetX(x).to<int>();
}
