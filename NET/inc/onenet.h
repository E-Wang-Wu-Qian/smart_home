#ifndef __ONENET_H
#define __ONENET_H


_Bool OneNet_DevLink(void);
void OneNet_SendData(void);

void OneNET_Subscribe(void);

void OneNET_Publish(const char *topic, const char *msg);

void OneNet_RevPro(unsigned char *cmd);
#endif
