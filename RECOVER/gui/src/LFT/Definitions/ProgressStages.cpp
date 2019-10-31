#include <gui/LFT/Definitions/ProgressStages.hpp>

int ProgressStages::TranslatePressureToPercentage(double pressure, double previousValue)
{
	ProgressStage chamberConditioningProgressRanges[CHAMBER_CONDITIONING_RANGE_COUNT] =
	{
		ProgressStage(18.286, 5.173, 10),
		ProgressStage(5.173, 2.489, 10),
		ProgressStage(2.489, 1.603, 10),
		ProgressStage(1.603, 1.227, 10),
		ProgressStage(1.227, 1.017, 10),
		ProgressStage(1.017, 0.900, 10),
		ProgressStage(0.900, 0.836, 10),
		ProgressStage(0.836, 0.790, 10),
		ProgressStage(0.801, 0.750, 9),
	};



	double offset = 10;
	for (int i = 0; i < CHAMBER_CONDITIONING_RANGE_COUNT; i++)
	{
		ProgressStage stage = chamberConditioningProgressRanges[i];

		if (pressure > stage.ValueEnd || i == CHAMBER_CONDITIONING_RANGE_COUNT - 1)
		{
			double basedValue = stage.ValueStart - pressure;
			double decimalValue = basedValue / (stage.ValueStart - stage.ValueEnd);
			double percentage = offset + (stage.ProgressRange * decimalValue);
						
			if (percentage < previousValue)
				percentage = previousValue;

			if (percentage > 100)
				return 100;

			return (int)percentage;
		}

		offset += stage.ProgressRange;
	}

	return 0;


	/*double offset = 100;
			double offset = 0;
			var last = sections.Last();
			foreach (var section in sections)
			{
				if (input > section.ValueEnd || section == last)
				{
					var basedValue = section.ValueStart - input;
					var decValue = basedValue / (section.ValueStart - section.ValueEnd);
					var adjustedPercentage = offset + (section.ProgressRange * decValue);

					if (lastNumber > adjustedPercentage)
						adjustedPercentage = lastNumber;
					lastNumber = adjustedPercentage;

					return adjustedPercentage;
				}

				offset += section.ProgressRange;
			}
	}*/
}
