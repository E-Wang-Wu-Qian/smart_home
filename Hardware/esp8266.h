#ifndef __ESP8266_H
#define __ESP8266_H

#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志


// extern volatile uint8_t usart2_cnt_flag;

void ESP8266_Clear(void);
_Bool ESP8266_WaitRecive(void);
_Bool ESP8266_SendCmd(char* cmd, char* res);
void ESP8266_Init(void);

unsigned char *ESP8266_GetIPD(unsigned short timeOut);
void ESP8266_SendData(unsigned char *data, unsigned short len);


#endif
