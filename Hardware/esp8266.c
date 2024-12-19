#include "stm32f10x.h"

#include "Usart.h"
#include "esp8266.h"
#include "Delay.h"
// C库
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

_Bool ESP8266_WaitReceive(void)
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
        if (ESP8266_WaitReceive() == REV_OK)
        {
            // UsartPrintf(USART1, "ESP8266_WaitReceive...\r\n");
            if (strstr((const char *)esp8266_buf, res) != NULL) // 如果检索到关键词
            {
                // UsartPrintf(USART1, "ESP8266 Wait OK...\r\n");
                //  Usart_SendString(USART1, esp8266_buf, sizeof(esp8266_buf));

                ESP8266_Clear();

                return 0;
            }
        }

        // UsartPrintf(USART1, "ESP8266_SendCmd...\r\n");
        //  TODO
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
        // UsartPrintf(USART1, "USART2_IRQHandler!!!\r\n");

        if (esp8266_cnt >= sizeof(esp8266_buf))
            esp8266_cnt = 0;
        

        esp8266_buf[esp8266_cnt++] = USART_ReceiveData(USART2);


        USART_ClearFlag(USART2, USART_FLAG_RXNE);
        // USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
