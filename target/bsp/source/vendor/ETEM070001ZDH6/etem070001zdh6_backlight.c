/**********************************************************************************************************/
//
//  Handle Backlight PWM Control
//
//
/**********************************************************************************************************/

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"
#include "etem070001zdh6_backlight.h"
#include "stm32f7xx_hal.h"


void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse)
{
	HAL_TIM_PWM_Stop(&timer, channel);                      // stop generation of pwm
	TIM_OC_InitTypeDef sConfigOC;
	timer.Init.Period = period;                             // set the period duration

	HAL_TIM_PWM_Init(&timer);                               // reinititialise with new period value

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;                                // set the pulse duration
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, channel);

	HAL_TIM_PWM_Start(&timer, channel);                     // start pwm generation
}
