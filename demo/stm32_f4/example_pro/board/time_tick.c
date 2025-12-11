/*
 * @Author       : SuperYu 824229900@qq.com
 * @Date         : 2025-07-23 22:50
 * @LastEditors  : SuperYu 824229900@qq.com
 * @LastEditTime : 2025-08-03 01:03
 * @Description  : 
 */
#include "time_tick.h"

volatile uint32_t heart_tick_time = 0;

void tick_timer_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update)) {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        heart_tick_time++;
        __set_PRIMASK(primask);

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

uint32_t get_heart_tick_time(void)
{
    return heart_tick_time;
}
