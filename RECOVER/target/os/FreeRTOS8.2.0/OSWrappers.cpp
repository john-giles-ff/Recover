/******************************************************************************
 *
 * @brief     This file is part of the TouchGFX 4.8.0 evaluation distribution.
 *
 * @author    Draupner Graphics A/S <http://www.touchgfx.com>
 *
 ******************************************************************************
 *
 * @section Copyright
 *
 * Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
 * All rights reserved.
 *
 * TouchGFX is protected by international copyright laws and the knowledge of
 * this source code may not be used to write a similar product. This file may
 * only be used in accordance with a license and should not be re-
 * distributed in any way without the prior permission of Draupner Graphics.
 *
 * This is licensed software for evaluation use, any use must strictly comply
 * with the evaluation license agreement provided with delivery of the
 * TouchGFX software.
 *
 * The evaluation license agreement can be seen on www.touchgfx.com
 *
 * @section Disclaimer
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
 * no obligation to support this software. Draupner Graphics A/S is providing
 * the software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Draupner Graphics A/S can not be held liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this software.
 *
 *****************************************************************************/
#include <touchgfx/hal/OSWrappers.hpp>
#include "Source/Include/FreeRTOS.h"
#include "Source/Include/task.h"
#include "Source/Include/queue.h"
#include "Source/Include/semphr.h"
#include <touchgfx/hal/GPIO.hpp>
#include <touchgfx/hal/HAL.hpp>

using namespace touchgfx;

static xSemaphoreHandle frame_buffer_sem;
static xQueueHandle vsync_q = 0;

// Just a dummy value to insert in the VSYNC queue.
static uint8_t dummy = 0x5a;

void OSWrappers::initialize()
{
    vSemaphoreCreateBinary(frame_buffer_sem);
    // Create a queue of length 1
    vsync_q = xQueueGenericCreate(1, 1, 0);
}

void OSWrappers::takeFrameBufferSemaphore()
{
    xSemaphoreTake(frame_buffer_sem, portMAX_DELAY);
}
void OSWrappers::giveFrameBufferSemaphore()
{
    xSemaphoreGive(frame_buffer_sem);
}

void OSWrappers::tryTakeFrameBufferSemaphore()
{
    xSemaphoreTake(frame_buffer_sem, 0);
}

void OSWrappers::giveFrameBufferSemaphoreFromISR()
{
    // Since this is called from an interrupt, FreeRTOS requires special handling to trigger a
    // re-scheduling. May be applicable for other OSes as well.
    portBASE_TYPE px = pdFALSE;
    xSemaphoreGiveFromISR(frame_buffer_sem, &px);
    portEND_SWITCHING_ISR(px);
}

void OSWrappers::signalVSync()
{
    if (vsync_q)
    {
        // Since this is called from an interrupt, FreeRTOS requires special handling to trigger a
        // re-scheduling. May be applicable for other OSes as well.
        portBASE_TYPE px = pdFALSE;
        xQueueSendFromISR(vsync_q, &dummy, &px);
        portEND_SWITCHING_ISR(px);
    }
}

void OSWrappers::waitForVSync()
{
    // First make sure the queue is empty, by trying to remove an element with 0 timeout.
    xQueueReceive(vsync_q, &dummy, 0);

    // Then, wait for next VSYNC to occur.
    xQueueReceive(vsync_q, &dummy, portMAX_DELAY);
}

void OSWrappers::taskDelay(uint16_t ms)
{
    vTaskDelay(ms);
}

static portBASE_TYPE IdleTaskHook(void* p)
{
    if ((int)p) //idle task sched out
    {
        touchgfx::HAL::getInstance()->setMCUActive(true);
    }
    else //idle task sched in
    {
        touchgfx::HAL::getInstance()->setMCUActive(false);
    }
    return pdTRUE;
}

// FreeRTOS specific handlers
extern "C"
{
    void vApplicationStackOverflowHook(xTaskHandle xTask,
                                       signed portCHAR* pcTaskName)
    {
        while (1);
    }

    void vApplicationMallocFailedHook(xTaskHandle xTask,
                                      signed portCHAR* pcTaskName)
    {
        while (1);
    }

    void vApplicationIdleHook(void)
    {
        // Set task tag in order to have the "IdleTaskHook" function called when the idle task is
        // switched in/out. Used solely for measuring MCU load, and can be removed if MCU load
        // readout is not needed.
        vTaskSetApplicationTaskTag(NULL, IdleTaskHook);
    }
}
