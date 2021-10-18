#ifndef __DRV_MOTOR_H__
#define __DRV_MOTOR_H__
#include <stdlib.h>
#include "string.h"
#include "stdint.h"



typedef enum
{
	right_motor =0,
	left_motor =1,
	
	
}motor_right_left;



void motor_pin_config(void);
void motor_driver_pin_init(void);
void motor_init(void);
void timer0_config(motor_right_left motor,uint32_t pwm);
void timer1_config(motor_right_left motor,uint32_t pwm);
void motor_star(void);








#endif