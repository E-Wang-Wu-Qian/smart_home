#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Usart.h"
#include "esp8266.h"
#include "LED.h"
#include "Key.h"

#include <string.h>

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

int main(void)
{

	OLED_Init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断控制器分组设置

	Usart1_Init(115200);
	Usart2_Init(115200);

	UsartPrintf(USART1, " USART init OK\r\n");

	Key_Init();
	LED_Init();

	ESP8266_Init();

	UsartPrintf(USART1, "Connect MQTT Server...\r\n");
	while (ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		Delay_ms(500);
	UsartPrintf(USART1, "Connect MQTT Server Success\r\n");

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
