#include "stm32f10x.h" // Device header
#include "WH01.h"
#include "OLED.h"
#include "dht11.h"

#define WH GPIO_Pin_6 // PA6, 用于控制WH01的电源

WH01_INFO wh01_info = {0};

int32_t const_humi = 60;

int32_t x_timing = 0;

void Wh01_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = WH;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // GPIO_SetBits(GPIOA, WH);
}

void Wh01_Set(void)
{

    //GPIO_SetBits(GPIOA, WH);
    GPIO_ResetBits(GPIOA, WH);
    wh01_info.Wh01_Status = 1;

    // OLED_Clear();

    if (wh01_info.Wh01_Status == 1)
    {
        OLED_ShowString(4, 4, "     ");
        OLED_ShowString(4, 4, "ON");
    }

    else
    {
        OLED_ShowString(4, 4, "     ");
        OLED_ShowString(4, 4, "OFF");
    }
}

void Wh01_Reset(void)
{
   // GPIO_ResetBits(GPIOA, WH);
   GPIO_SetBits(GPIOA, WH);
    wh01_info.Wh01_Status = 0;
    // OLED_Clear();

    if (wh01_info.Wh01_Status == 1)
    {
        OLED_ShowString(4, 4, "        ");
        OLED_ShowString(4, 4, "ON");
    }

    else
    {
        OLED_ShowString(4, 4, "        ");
        OLED_ShowString(4, 4, "OFF");
    }
}
