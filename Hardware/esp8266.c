#include "stm32f10x.h"

#include "Usart.h"
#include "esp8266.h"
#include "Delay.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ESP8266_WIFI_INFO "AT+CWJAP=\"TCP_Server\",\"0987654321\"\r\n" // AT+ CWJAP =<ssid>,< password>   ##设置ESP8266加入ssid，密码是password

// volatile uint8_t usart2_cnt_flag = 0;

uint8_t esp8266_buf[512];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;

void ESP8266_Clear(void)
{
    memset(esp8266_buf, 0, sizeof(esp8266_buf));
    esp8266_cnt = 0;
}

_Bool ESP8266_WaitRecive(void)
{
    if (esp8266_cnt == 0)
    {
        // UsartPrintf(USART1, "esp8266_cnt == 0\r\n");
        return REV_WAIT;
    }

    if (esp8266_cnt == esp8266_cntPre)
    {
        // UsartPrintf(USART1, "esp8266_cnt == esp8266_cntPre\r\n");
        esp8266_cnt = 0;
        return REV_OK;
    }

    // UsartPrintf(USART1, "return REV_WAIT\r\n");
    esp8266_cntPre = esp8266_cnt;

    return REV_WAIT;
}

/// @brief 发送命令
/// @param cmd 发送的命令
/// @param res 需要检查的返回命令
/// @return
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeout = 200;

    Usart_SendString(USART2, (unsigned char *)cmd, strlen((const char *)cmd));

    while (timeout--)
    {
        if (ESP8266_WaitRecive() == REV_OK)
        {
    
            if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
            {
                // UsartPrintf(USART1, "ESP8266 Wait OK...\r\n");
                //  Usart_SendString(USART1, esp8266_buf, sizeof(esp8266_buf));

                ESP8266_Clear();

                return 0;
            }
        }

        Delay_ms(10);
    }

    return 1;
}

void ESP8266_Init(void)
{
    ESP8266_Clear();

    UsartPrintf(USART1, "1. AT\r\n");
    while (ESP8266_SendCmd("AT\r\n", "OK")) // 测试AT是否OK
        Delay_ms(500);

    UsartPrintf(USART1, "2. CWMODE\r\n");
    while (ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK")) // 设置ESP-01S为station模式(客户端模式)
        Delay_ms(500);

    UsartPrintf(USART1, "3. AT+CWDHCP\r\n");
    while (ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK")) // 设置 DHCP 开  AT+ CWDHCP= <mode>,<en>
        Delay_ms(500);

    UsartPrintf(USART1, "4. CWJAP\r\n");
    while (ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
        Delay_ms(500);

    UsartPrintf(USART1, "5. ESP8266 Init OK\r\n");
}

void USART2_IRQHandler(void)
{

    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {

        if (esp8266_cnt >= sizeof(esp8266_buf))
            esp8266_cnt = 0;
        
        esp8266_buf[esp8266_cnt++] = USART_ReceiveData(USART2);

        USART_ClearFlag(USART2, USART_FLAG_RXNE);
        // USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}



//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		Usart_SendString(USART2, data, len);		//发送设备连接请求数据
	}

}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART1, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		Delay_ms(5);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}
