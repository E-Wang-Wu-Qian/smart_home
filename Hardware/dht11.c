/***************STM32F103C8T6**********************
 * 文件名  ：DHT11.c
 * 描述    ：DHT11传感器
 * 备注    : DHT11温度湿度传感器
 * 接口    ：PA11-DATA

********************LIGEN*************************/

#include "dht11.h"
#include "Delay.h"

#define DT GPIO_Pin_0

// 复位DHT11 ---->  向DHT11发起开始信号
void DHT11_Rst(void)
{
	DHT11_IO_OUT();	 // SET OUTPUT
	DHT11_DQ_OUT(0); // 拉低DQ
	Delay_ms(20);	 // 拉低至少18ms
	DHT11_DQ_OUT(1); // DQ=1
	Delay_us(30);	 // 主机拉高20~40us
}


// 等待DHT11的回应
// 返回1:未检测到DHT11的存在
// 返回0:存在
uint8_t DHT11_Check(void)
{
	uint8_t retry = 0;
	DHT11_IO_IN();					   // SET INPUT
	while (DHT11_DQ_IN && retry < 100) // DHT11会拉低40~80us
	{
		retry++;
		Delay_us(1);
	};

	// 读取到DHT11的响应信号或者超时
	if (retry >= 100)
		return 1;
	else
		retry = 0;
	while (!DHT11_DQ_IN && retry < 100) // DHT11拉低后会再次拉高40~80us
	{
		retry++;
		Delay_us(1);
	};

	if (retry >= 100)
		return 1;
	return 0;
}

// 从DHT11读取一个位
// 返回值：1/0
uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0;
	while (DHT11_DQ_IN && retry < 100) // 等待变为低电平  每一位数据以50us低电平开始
	{
		retry++;
		Delay_us(1);
	}
	retry = 0;
	while (!DHT11_DQ_IN && retry < 100) // 等待变高电平
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40); // 等待40us   26-28us为0，70us为1
	// 通过判断高电平的持续时间判断数据是1还是0
	if (DHT11_DQ_IN)
		return 1;
	else
		return 0;
}

// 从DHT11读取一个字节
// 返回值：读到的数据
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, dat;
	dat = 0;
	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}
	return dat;
}

/**
 * 数据格式：8bit湿度整数数据+8bit湿度小数数据+8bit温度整数数据+8bit温度小数数据+8bit校验和
 * 共40bit数据
 */


// 从DHT11读取一次数据
// temp:温度值(范围:0~50°)
// humi:湿度值(范围:20%~90%)
// 返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++) // 读取40位数据
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])  // 校验数据,buf[4]存放的是校验和
		{
			*humi = buf[0];
			*temp = buf[2];
		}
	}
	else
	{
		return 1;
	}

	return 0;
}

// 初始化DHT11的IO口 DQ 同时检测DHT11的存在
// 返回1:不存在
// 返回0:存在
uint8_t DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PG端口时钟
	GPIO_InitStructure.GPIO_Pin = DT;					  // PG11端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化IO口
	GPIO_SetBits(GPIOA, DT);			   // PG11 输出高

	DHT11_Rst();		  // 复位DHT11
	return DHT11_Check(); // 等待DHT11的回应
}
