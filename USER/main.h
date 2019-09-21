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
    s16 angle;					//角度返回值
	s16 last_angle;				//前一次角度返回值
	s16 cycle;					//圈数
	s32 real_posion;			//实际位置
    s16 speed;					//实时速度
	s16 flow;					//实时电流
	s16 temperature;			//实时温度
}rmc620_t;


void SysTick_Configuration(void);

#endif
