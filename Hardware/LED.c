#include "stm32f10x.h"                  // Device header
#include "LED.h"

LED_INFO led_info = {0};

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure; 
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	
	
}

void LED_Set(_Bool status)
{
	if(status == LED_ON)
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	else if(status == LED_OFF)
		GPIO_SetBits(GPIOC, GPIO_Pin_13);

	led_info.Led_Status = status;
}

// void LED1_ON(void)
// {
// 	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
// }

// void LED1_OFF(void)
// {
// 	GPIO_SetBits(GPIOA, GPIO_Pin_1);
// }

// void LED1_turn(void)
// {
// 	if(GPIO_ReadOutputDataBit (GPIOA, GPIO_Pin_1) == 0)
// 		GPIO_SetBits(GPIOA, GPIO_Pin_1);
//     else
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
// }


// void LED2_turn(void)
// {
// 	if(GPIO_ReadOutputDataBit (GPIOA, GPIO_Pin_2) == 0)
// 		GPIO_SetBits(GPIOA, GPIO_Pin_2);
//     else
// 		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
// }

// void LED2_ON(void)
// {
// 	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
// }

// void LED2_OFF(void)
// {
// 	GPIO_SetBits(GPIOA, GPIO_Pin_2);
// }
