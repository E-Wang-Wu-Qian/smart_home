#ifndef __WH01_H
#define __WH01_H


typedef struct
{
    _Bool Wh01_Status;

} WH01_INFO;

extern WH01_INFO wh01_info;

extern int32_t  const_humi ;

extern int32_t x_timing;

void Wh01_Init(void);

void Wh01_Set(void);

void Wh01_Reset(void);


#endif
