
#ifdef __cplusplus
 extern "C" {
#endif

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ETEM070001ZDH6_backlight_H
#define __ETEM070001ZDH6_backlight_H

void setPWM(TIM_HandleTypeDef timer, uint32_t channel, uint16_t period, uint16_t pulse);

#endif /* __ETEM070001zdh6_backlight_H */

#ifdef __cplusplus
}
#endif
