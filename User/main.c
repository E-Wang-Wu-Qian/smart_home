#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Usart.h"
#include "esp8266.h"

// C库
#include <string.h>
int main(void)
{

	// OLED_Init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断控制器分组设置


	Usart1_Init(115200);
	Usart2_Init(115200);

	UsartPrintf(USART1, " USART init OK\r\n");


	ESP8266_Init();

	while (1)
	{
		// if (usart2_cnt_flag == 1)
		// {
		// 	UsartPrintf(USART1, "CNT++\r\n");
		// 	usart2_cnt_flag = 0;
		// }

		// Delay_ms(1000);
	}
}
