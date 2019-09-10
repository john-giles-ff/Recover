#ifndef GRAPHSCREEN_VIEW_HPP
#define GRAPHSCREEN_VIEW_HPP

#include <gui_generated/graphscreen_screen/GraphScreenViewBase.hpp>
#include <gui/graphscreen_screen/GraphScreenPresenter.hpp>
#include <gui/common/graph_widget/Graph.hpp>
#include <gui/common/ProgressBar.hpp>

class GraphScreenView : public GraphScreenViewBase
{
public:
	GraphScreenView() { }
    virtual ~GraphScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
	virtual void handleTickEvent();
	virtual void TogglePressure();
	virtual void TogglePrecursor();
	virtual void ToggleBase();	
	virtual void Exit();

	void UpdateGraph();
protected:
	/*
		It makes absoloutly 0 sense, but adding this cover graph which is kept blank fixes the issue where the graph freezes!
	*/
	Graph coverGraph;



	//Graphs
	Graph pressureGraph;
	Graph baseGraph;
	Graph precursorGraph;	
	ProgressBar progressBar;

	//Background Boxes
	static const int modeCount = 5;
	Box backgroundBoxes[modeCount];
	colortype backgroundBoxesColors[modeCount]{
		Color::getColorFrom24BitRGB(0x5F, 0x5F, 0x7A),
		Color::getColorFrom24BitRGB(0xA5, 0x5F, 0x7A),
		Color::getColorFrom24BitRGB(0x61, 0x08, 0x08),
		Color::getColorFrom24BitRGB(0xA0, 0xA5, 0x7A),
		Color::getColorFrom24BitRGB(0x5F, 0xA5, 0x7A)
	};

	void SampleLoaded(int u);		
	

private:
	void UpdateRange(int width = -1);	
	int GetXRealPosition(int x);

	bool requiresInvalidate = false;

};

#endif // GRAPHSCREEN_VIEW_HPP
