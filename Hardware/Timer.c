#include "stm32f10x.h"

extern uint16_t Num;
extern uint16_t Num2;
void Timer2_Init(uint16_t arr, uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_InternalClockConfig(TIM2);

    // 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = arr;               // 计数周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;             // 预分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//设置更新事件，触发输出
    //TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // 配置更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;

    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

void Timer3_Init(uint16_t arr, uint16_t psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_SelectInputTrigger(TIM3, TIM_TS_ITR1);//TIM3的触发输入
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_External1);//选择TIM3的从模式

    // 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 时钟分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period = arr;               // 计数周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;             // 预分频
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    
    // 配置更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        Num++;
    }
}
uint16_t x = 0;
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if(x == 0)
        {
            x = 1;
        }
        else
        {
            Num2++;
            x = 1;
        }
        
    }
}
