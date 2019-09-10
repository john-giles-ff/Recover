#include <gui/containers/UsageInstructions.hpp>


UsageInstructions::UsageInstructions()
{
	Background.setAlpha(140);

	TxtInstruction1.setWideTextAction(WIDE_TEXT_WORDWRAP);
	TxtInstruction2.setWideTextAction(WIDE_TEXT_WORDWRAP);
}

void UsageInstructions::SetInstructions(Instruction * instructions, int count)
{
	currentPage = 0;

	currentInstructions = instructions;
	instructionCount = count;	

	updatePage();
}

void UsageInstructions::NextPage()
{
	//If it's the last page, don't go to next page
	if (currentPage == pageCount())
		return;

	currentPage++;
	updatePage();

}

void UsageInstructions::PreviousPage()
{
	//If it's the first page, don't go back a page
	if (currentPage == 0)
		return;

	currentPage--;
	updatePage();
}

void UsageInstructions::Hide()
{
	//Hide the entire control
	setVisible(false);
	invalidate();
}

void UsageInstructions::updatePage()
{		
	//If there's no instructions, return
	if (instructionCount == 0)
		return;
	


	//Set Next/Previous Pages being visible
	BtnPreviousPage.setVisible(currentPage != 0);
	BtnNextPage.setVisible(currentPage != pageCount());
		
	//Load instructions
	int pageOffset = currentPage * 2;
	
	Instruction instruction1 = currentInstructions[pageOffset + 0];	
	ImgInstruction1.setBitmap(Bitmap(instruction1.imageID));
	TxtInstruction1.setTypedText(TypedText(instruction1.textID));		
	Unicode::snprintf(TxtInstructionCount1Buffer, TXTINSTRUCTIONCOUNT1_SIZE, "%d", pageOffset + 1);

	//If it's not the last page, or if it's not even, show the second instruction
	if (currentPage != pageCount() || instructionCount % 2 == 0)
	{		
		ImgInstruction2.setVisible(true);
		TxtInstruction2.setVisible(true);
		TxtInstructionCount2.setVisible(true);

		Instruction instruction2 = currentInstructions[pageOffset + 1];		
		ImgInstruction2.setBitmap(Bitmap(instruction2.imageID));
		TxtInstruction2.setTypedText(TypedText(instruction2.textID));		

		Unicode::snprintf(TxtInstructionCount2Buffer, TXTINSTRUCTIONCOUNT2_SIZE, "%d", pageOffset + 2);
	}	
	else
	{		
		ImgInstruction2.setVisible(false);
		TxtInstruction2.setVisible(false);
		TxtInstructionCount2.setVisible(false);
	}
	

	invalidate();	
}
