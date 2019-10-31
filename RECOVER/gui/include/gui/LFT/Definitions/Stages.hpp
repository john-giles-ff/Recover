#ifndef STAGES_HPP
#define STAGES_HPP

//Only for automatic stages
constexpr int STAGE_COUNT = 7;
enum LFT_STAGE
{
	//Negative stages won't be automatically ran
	LFT_STAGE_TUNING = -1,

	LFT_STAGE_LID_CONTROL = 0,
	LFT_STAGE_PRECHECKS = 1,
	LFT_STAGE_CHAMBER_CONDITIONING = 2,
	LFT_STAGE_READY_TO_FUME = 3,
	LFT_STAGE_FUMING = 4,
	LFT_STAGE_COOLDOWN = 5,
	LFT_STAGE_FINISHED = 6,	
};


#endif /* STAGES_HPP */
