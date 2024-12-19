#include "stm32f10x.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/// @brief 打印信息
/// @param baud
void Usart1_Init(uint32_t baud)
{
    GPIO_InitTypeDef gpio_initstruct;
    USART_InitTypeDef usart_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // PA9 TXD
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_9;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // PA10 RXD
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    usart_initstruct.USART_BaudRate = baud;
    usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    usart_initstruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 接收和发送
    usart_initstruct.USART_Parity = USART_Parity_No;                             // 无奇偶校验
    usart_initstruct.USART_StopBits = USART_StopBits_1;                          // 1位停止位
    usart_initstruct.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
    USART_Init(USART1, &usart_initstruct);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断

    nvic_initstruct.NVIC_IRQChannel = USART1_IRQn;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&nvic_initstruct);
}

/// @brief 驱动ESP-01S
/// @param baud
void Usart2_Init(uint32_t baud)
{
    GPIO_InitTypeDef gpio_initstruct;
    USART_InitTypeDef usart_initstruct;
    NVIC_InitTypeDef nvic_initstruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // PA2 TXD
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_2;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    // PA3 RXD
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_initstruct);

    usart_initstruct.USART_BaudRate = baud;
    usart_initstruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    usart_initstruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;                 // 接收和发送
    usart_initstruct.USART_Parity = USART_Parity_No;                             // 无奇偶校验
    usart_initstruct.USART_StopBits = USART_StopBits_1;                          // 1位停止位
    usart_initstruct.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
    USART_Init(USART2, &usart_initstruct);

    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能接收中断

    nvic_initstruct.NVIC_IRQChannel = USART2_IRQn;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&nvic_initstruct);
}

/// @brief 串口发送字符串
/// @param USARTx
/// @param str
/// @param len
// void Usart_SendString(USART_TypeDef *USARTx, uint8_t *str, uint16_t len)
// {
//     uint16_t i = 0;
//     for (; i < len; i++)
//     {
//         USART_SendData(USARTx, *str++);
//         // TODO:
//         while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//             ;
//     }
// }


void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

	unsigned short count = 0;
	
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}

}

/// @brief 串口格式化打印
/// @param USARTx
/// @param fmt
/// @param
// void UsartPrintf(USART_TypeDef *USARTx, char *fmt, ...)
// {
//     unsigned char UsartPrintfBuf[256];
//     va_list arg;
//     va_start(arg, fmt);
//     vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, arg);
//     va_end(arg);

//     unsigned char *pStr = UsartPrintfBuf;
//     while (*pStr != 0)
//     {
//         USART_SendData(USARTx, *pStr++);
//         while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
//             ;
//     }
// }

void UsartPrintf(USART_TypeDef *USARTx, char *fmt, ...)
{

    unsigned char UsartPrintfBuf[296];
    va_list ap;
    unsigned char *pStr = UsartPrintfBuf;

    va_start(ap, fmt);
    vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap); // 格式化
    va_end(ap);

    while (*pStr != 0)
    {
        USART_SendData(USARTx, *pStr++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}



void USART1_IRQHandler(void)
{

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断
    {
        USART_ClearFlag(USART1, USART_FLAG_RXNE);
    }
}
