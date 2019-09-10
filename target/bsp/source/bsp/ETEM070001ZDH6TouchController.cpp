/******************************************************************************
 *
 * @author    EDT <http://www.edtc.com>
 *
 *****************************************************************************/
#include "bsp/ETEM070001ZDH6TouchController.hpp"
#include "ETEM070001ZDH6_lcd.h"
#include "ETEM070001ZDH6_ts.h"

using namespace touchgfx;

void ETEM070001ZDH6TouchController::init()
{
    if (BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()) == TS_OK)
    {
        isInitialized = true;
    }
}

bool ETEM070001ZDH6TouchController::sampleTouch(int32_t& x, int32_t& y)
{
    if (isInitialized)
    {
        TS_StateTypeDef state;
        BSP_TS_GetState(&state);
        if (state.touchDetected)
        {
            x = state.touchY[0];
            y = state.touchX[0];

            return true;
        }
    }
    return false;
}
