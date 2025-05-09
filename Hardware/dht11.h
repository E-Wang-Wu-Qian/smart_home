#ifndef __DHT11_H
#define __DHT11_H


#include "stm32f10x.h"


extern uint8_t temp, humi;

//PA11-->PA0
#define DHT11_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8;}    //PA0读取数据
#define DHT11_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3;} 
//IO操作函数   
#define	DHT11_DQ_OUT(X)  GPIO_WriteBit(GPIOA, GPIO_Pin_0, X)
#define	DHT11_DQ_IN  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取数据
uint8_t DHT11_Read_Byte(void);//读取一个字节
uint8_t DHT11_Read_Bit(void);//读取一位
uint8_t DHT11_Check(void);//检测DHT11
void DHT11_Rst(void);//复位DHT11   








#endif
