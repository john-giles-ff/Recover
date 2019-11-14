#pragma once
struct ProgressStage
{
public:
	double ValueStart;
	double ValueEnd;
	double ProgressRange;

	ProgressStage(double valueStart, double valueEnd, double progressRange) 
	{
		ValueStart = valueStart;
		ValueEnd = valueEnd;
		ProgressRange = progressRange;
	}
};


class ProgressStages
{
public:
	int TranslatePressureToPercentage(double pressure, double previousValue);
	static constexpr int CHAMBER_CONDITIONING_RANGE_COUNT = 9;





};

