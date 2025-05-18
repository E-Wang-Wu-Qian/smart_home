#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Usart.h"
#include "esp8266.h"
#include "LED.h"
#include "Key.h"
#include "dht11.h"
#include "WH01.h"
#include "Timer.h"
#include "onenet.h"
#include <string.h>

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

int main(void)
{

	unsigned short timeCount = 0; // 发送间隔变量
	unsigned char *dataPtr = NULL;

	OLED_Init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断控制器分组设置

	Usart1_Init(115200);
	Usart2_Init(115200);
	Usart3_Init(115200);

	UsartPrintf(USART1, " USART init OK\r\n");

	Key_Init();
	LED_Init();

	Wh01_Init();

	Timer2_Init(10000 - 1, 7200 - 1);
	Timer3_Init(60 - 1, 1 - 1);

	while (DHT11_Init())
	{
		UsartPrintf(USART1, "DHT11 Error \r\n");
		Delay_ms(1000);
	}
	/*
		while (1)
		{

			Wh01_Set();
			Delay_ms(5000); // 延时5s

			UsartPrintf(USART1, "DEBUG...\r\n");

			Wh01_Reset();
			Delay_ms(5000); // 延时5s
		}
		*/
	ESP8266_Init();

	UsartPrintf(USART1, "Connect MQTT Server...\r\n");
	while (ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		Delay_ms(500);
	UsartPrintf(USART1, "Connect MQTT Server Success\r\n");

	// 设备登录
	UsartPrintf(USART1, "Device Login...\r\n");
	while (OneNet_DevLink())
	{
		ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
		Delay_ms(500);
	}

	UsartPrintf(USART1, "Device Login Success\r\n");

	OneNET_Subscribe();

	while (1)
	{
		if (++timeCount >= 300) // 发送间隔5s
		{
			DHT11_Read_Data(&temp, &humi);

			UsartPrintf(USART1, "temp:%d,humi:%d\r\n", temp, humi);

			TIM_Set();

			OLED_ShowString(1, 1, "Temp:");
			OLED_ShowNum(1, 7, temp, 2);
			OLED_ShowString(1, 9, "C"); // ℃

			OLED_ShowString(2, 1, "Humi:");
			OLED_ShowNum(2, 7, humi, 2);
			OLED_ShowString(2, 9, "%");

			OLED_ShowString(3, 1, "LED:");
			if (led_info.Led_Status == LED_ON)
				OLED_ShowString(3, 5, "ON");
			else
				OLED_ShowString(3, 5, "OFF");

			OLED_ShowString(4, 1, "WH:");
			if (wh01_info.Wh01_Status == 1)
				OLED_ShowString(4, 4, "ON");
			else
				OLED_ShowString(4, 4, "OFF");

			OneNet_SendData(); // 发送数据

			ESP8266_Clear();

			timeCount = 0;
		}

		dataPtr = ESP8266_GetIPD(0); // 接收平台数据

		if (dataPtr != NULL)
			OneNet_RevPro(dataPtr); // 解析返回数据

		Delay_ms(10);
	}
}
