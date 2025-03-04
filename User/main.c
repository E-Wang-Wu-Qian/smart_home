#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "OLED.h"
#include "Usart.h"
#include "esp8266.h"
#include "LED.h"
#include "Key.h"
#include "dht11.h"
#include "WH01.h"

#include "onenet.h"

#include <string.h>

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

uint8_t temp, humi;

int main(void)
{

	unsigned short timeCount = 0; // 发送间隔变量
	unsigned char *dataPtr = NULL;

	OLED_Init();

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断控制器分组设置

	Usart1_Init(115200);
	Usart2_Init(115200);

	UsartPrintf(USART1, " USART init OK\r\n");

	Key_Init();
	LED_Init();

	Wh01_Init();

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
			if (++timeCount >= 200) // 发送间隔5s
			{
				DHT11_Read_Data(&temp, &humi);

				UsartPrintf(USART1, "temp:%d,humi:%d\r\n", temp, humi);

				OneNet_SendData(); // 发送数据

				timeCount = 0;
				ESP8266_Clear();
			}

			dataPtr = ESP8266_GetIPD(0);

			if (dataPtr != NULL)
				OneNet_RevPro(dataPtr);

			Delay_ms(10);
		}
		
}
