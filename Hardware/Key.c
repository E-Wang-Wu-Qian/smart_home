#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "WH01.h"

void Key_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

	// 配置PB0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

	// 配置PB12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

	// // 配置PB13
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	// GPIO_Init(GPIOB, &GPIO_InitStructure);
	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

	EXTI_InitStructure.EXTI_Line = EXTI_Line1; // 配置外部中断线 1 (EXTI Line 1)
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);

	// 配置EXTI0（PB0）
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);

	// 配置EXTI12（PB12）
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);

	// 配置EXTI13（PB13）
	// EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	// EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
	// EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn; // 配置外部中断 1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 设置抢占优先级为 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  // 设置子优先级为 2
	NVIC_Init(&NVIC_InitStructure);

	// 配置EXTI0（PB0）
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);

	// 配置EXTI12（PB12）
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI1_IRQHandler(void)
{

	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		// Delay_ms(10);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
		{
			// Delay_us(100); // 消抖
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
			{

				if (led_info.Led_Status == LED_ON)
					LED_Set(LED_OFF);
				else
					LED_Set(LED_ON);
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

// PB0中断处理
void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			if (wh01_info.Wh01_Status == 0)
				Wh01_Set();
			else
				Wh01_Reset();
		}

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
// PB12
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET)
		{
			Wh01_Set();
		}
		else
		{
			Wh01_Reset();
		}

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	// else if (EXTI_GetITStatus(EXTI_Line13) != RESET)
	// {
	// 	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
	// 	{
	// 		Wh01_Reset();
	// 	}

	// 	EXTI_ClearITPendingBit(EXTI_Line13);
	// }
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
