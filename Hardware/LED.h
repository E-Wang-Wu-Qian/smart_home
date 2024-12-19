#ifndef __LED_H
#define __LED_H

#define LED_ON  1

#define LED_OFF	0



typedef struct
{

	_Bool Led_Status;

} LED_INFO;


extern LED_INFO led_info;

void LED_Init(void);

void LED_Set(_Bool status);


// void LED1_ON(void) ;
// void LED1_OFF(void) ;
// void LED2_ON(void)   ;
// void LED2_OFF(void)   ;

// void LED1_turn(void);
// void LED2_turn(void) ;

#endif
