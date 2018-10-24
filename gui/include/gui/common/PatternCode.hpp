#ifndef PATTERN_CODE_HPP
#define PATTERN_CODE_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/AnimatedImage.hpp>
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/Callback.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/Color.hpp>


#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#define CANVAS_BUFFER_SIZE (92000)



using namespace touchgfx;

class PatternCode : public Container
{
public:

	PatternCode();
    virtual ~PatternCode();
	
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);	
	

	void SetPatternEnteredAction(GenericCallback<const PatternCode &> &callback);
	void SetExitAction(GenericCallback< const AbstractButton & > &callback);

	int * GetPath() { return path; }
	int GetPathSize() { return pathLength + 1; }


protected:		
	GenericCallback<const PatternCode &> *PatternEnteredCallback;
		
	PainterRGB565 painter;
	ButtonWithIcon btnExit;

	ToggleButton points[9];
	
	Line pathLines[8];	

	int path[9];
	int pathLength = 0;

	const int CollisionPadding = 15;


	const int RowA = 75,
		RowB = 175,
		RowC = 275,
		Column1 = 50,
		Column2 = 150,
		Column3 = 250;

	bool CheckCollide(int boxX, int boxY, int boxWidth, int boxHeight, int positionX, int positionY);
		
   
};


#endif /* CIRCULARPROGRESS_HPP */
