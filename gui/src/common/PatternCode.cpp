#include <gui/common/PatternCode.hpp>
#include <texts/TextKeysAndLanguages.hpp>


PatternCode::PatternCode()
{
    setTouchable(true);
			
	painter = PainterRGB565(Color::getColorFrom24BitRGB(0x32, 0xa7, 0x61));	

	//Setup Exit Button
	btnExit.setBitmaps(Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_TRANSPARENT_ID), Bitmap(BITMAP_X_ID), Bitmap(BITMAP_X_ID));	
	btnExit.setXY(10, 10);					

	//Setup points	
	for (int i = 0; i < 9; i++)
	{
		points[i].setBitmaps(Bitmap(BITMAP_POINT_ID), Bitmap(BITMAP_POINT_SELECTED_ID));
		points[i].setTouchable(false);
	}		
	points[0].setXY(Column1, RowA);
	points[1].setXY(Column2, RowA);
	points[2].setXY(Column3, RowA);
	points[3].setXY(Column1, RowB);
	points[4].setXY(Column2, RowB);
	points[5].setXY(Column3, RowB);
	points[6].setXY(Column1, RowC);
	points[7].setXY(Column2, RowC);
	points[8].setXY(Column3, RowC);	



	static uint8_t canvasBuffer[CANVAS_BUFFER_SIZEP];
	CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZEP);
	//CanvasWidgetRenderer::setWriteMemoryUsageReport(true);

	//Setup Lines
	for (int i = 0; i < 8; i++)
	{
		pathLines[i].setPainter(painter);
		pathLines[i].setPosition(0, 0, 350, 400);	
		pathLines[i].setLineWidth(15);
		pathLines[i].setLineEndingStyle(touchgfx::Line::ROUND_CAP_ENDING);
		pathLines[i].setVisible(false);
	}

	
	//Set Widgth and Height
	setWidth(350);
	setHeight(400);

	//Add Everything
	//add(background);
	add(btnExit);
	for (int i = 0; i < 9; i++)	
		add(points[i]);	
	for (int i = 0; i < 8; i++)
		add(pathLines[i]);
			

	//Redraw
	invalidate();
}

PatternCode::~PatternCode()
{

}

void PatternCode::handleClickEvent(const ClickEvent& evt)
{    
	//Check if finger is over exit button
	if (Utils::IsPointWithinRectagle(btnExit.getX(), btnExit.getY(), btnExit.getWidth(), btnExit.getHeight(), evt.getX(), evt.getY()))
		btnExit.handleClickEvent(evt);

	//Clear when first pressed
	if (evt.getType() == ClickEvent::PRESSED)
	{		
		ClearLine();
	}

	//Call callback with pattern when released
	if (evt.getType() == ClickEvent::RELEASED)
	{					
		
		//Remove last bit of the path (Point to finger)
		if (pathLength != -1 && pathLength != 8)
			pathLines[pathLength].setVisible(false);

		
		if (pathLength == 0)
			for (int i = 0; i < 9; i++)
				points[i].forceState(false);

		//Return pattern entered
		if (PatternEnteredCallback && PatternEnteredCallback->isValid())
			PatternEnteredCallback->execute(*this);		
		invalidate();
		
	}
}

void PatternCode::handleDragEvent(const DragEvent& evt)
{	
	//Check for collision with any of the points
	for (int i = 0; i < 9; i++)
	{
		if (!points[i].getState() && CheckCollide(
			points[i].getX() - CollisionPadding, 
			points[i].getY() - CollisionPadding, 
			points[i].getWidth() + CollisionPadding * 2, 
			points[i].getHeight() + CollisionPadding * 2, 
			evt.getNewX(), evt.getNewY()))
		{	
			//Check Point
			points[i].forceState(true);
			points[i].invalidate();

			//End the part of the path
			if (pathLength != -1)
				pathLines[pathLength].updateEnd(points[i].getX() + (points[i].getWidth() / 2), points[i].getY() + (points[i].getHeight() / 2));						
				

			//Update Path
			pathLength++;										
			path[pathLength] = i;			

			if (pathLength != 8)
			{
				pathLines[pathLength].setVisible(true);
				pathLines[pathLength].setStart(points[i].getX() + (points[i].getWidth() / 2), points[i].getY() + (points[i].getHeight() / 2));
			}
		}
	}

	//Update path to point
	if (pathLength != -1 && pathLength != 8)
		pathLines[pathLength].setEnd(evt.getNewX(), evt.getNewY());

	invalidate();
}

void PatternCode::SetPatternEnteredAction(GenericCallback<const PatternCode &> &callback)
{
	PatternEnteredCallback = &callback;
}

void PatternCode::SetExitAction(GenericCallback< const AbstractButton & > &callback)
{	
	btnExit.setAction(callback);
}

bool PatternCode::CheckCollide(int boxX, int boxY, int boxWidth, int boxHeight, int positionX, int positionY)
{
	if (positionX < boxX || positionX > boxX + boxWidth) return false;
	if (positionY < boxY || positionY > boxY + boxHeight) return false;

	return true;
}

void PatternCode::ClearLine()
{	
	for (int i = 0; i < 8; i++)
		pathLines[i].setVisible(false);
	for (int i = 0; i < 9; i++)
		points[i].forceState(false);

	pathLength = -1;

	invalidate();
}

