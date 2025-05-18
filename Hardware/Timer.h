#ifndef __TIMER_H
#define __TIMER_H
extern uint16_t Num;
extern uint16_t Num2;
extern uint8_t Time_Status;
extern uint16_t x_timing;
void Timer2_Init(uint16_t arr, uint16_t psc);
void Timer3_Init(uint16_t arr, uint16_t psc);
void TIM_Set(void);

#endif
