#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "arm_math.h"
#include "math.h"

#include "nvic.h"
#include "can.h"

#include "Pid.h"
#include "Motor_Task.h"

#include "FIR.h"

#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
#define Current_Max 3000
#define Times 2

typedef struct
{
    s16 angle;					//�Ƕȷ���ֵ
	s16 last_angle;				//ǰһ�νǶȷ���ֵ
	s16 cycle;					//Ȧ��
	s32 real_posion;			//ʵ��λ��
    s16 speed;					//ʵʱ�ٶ�
	s16 flow;					//ʵʱ����
	s16 temperature;			//ʵʱ�¶�
}rmc620_t;


void SysTick_Configuration(void);

#endif
