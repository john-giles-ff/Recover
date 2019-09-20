#ifndef USAGEINSTRUCTIONS_HPP
#define USAGEINSTRUCTIONS_HPP

#include <gui_generated/containers/UsageInstructionsBase.hpp>
#include <BitmapDatabase.hpp>

struct Instruction
{	

	Instruction(TEXTS newText, uint16_t newImage)
	{
		textID = newText;
		imageID = newImage;
	}

	TEXTS textID;
	uint16_t imageID;
};

class UsageInstructions : public UsageInstructionsBase
{
public:
    UsageInstructions();
    virtual ~UsageInstructions() {}

	void SetInstructions(Instruction* instructions, int count);

	virtual void NextPage();
	virtual void PreviousPage();
	virtual void Hide();
protected:


private:
	int currentPage = 0;

	Instruction *currentInstructions;	
	int instructionCount;

	void updatePage();

	int pageCount() { return (int)ceil((float)instructionCount / 2.0f) -1; }

};

#endif // USAGEINSTRUCTIONS_HPP
