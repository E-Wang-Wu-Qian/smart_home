#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"

void Key_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1; // 配置外部中断线 1 (EXTI Line 1)
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //// 设置触发方式为下降沿触发
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; // 配置外部中断 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 设置抢占优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // 设置子优先级为 2
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		//Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			if (led_info.Led_Status == LED_ON)
				LED_Set(LED_OFF);
			else
				LED_Set(LED_ON);
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

// uint8_t Key_Get(void)
// {
// 	uint8_t number = 0;
// 	if(GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_1) == 0)
// 	{
// 		 Delay_ms(20);
// 		 while( GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_1) == 0);
// 		 Delay_ms(20);
// 		 number = 1;
// 	}

// 	if(GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_11) == 0)
// 	{
// 		 Delay_ms(20);
// 		 while( GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_11) == 0);
// 		 Delay_ms(20);
// 		 number = 2;
// 	}

// 	return number;
// }
