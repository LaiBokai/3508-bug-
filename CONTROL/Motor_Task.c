#include "main.h"

float Current_Change = 0;
float Current_f = 0;
float Current_f_10times = 0;
PID Pid_Current;
PID Pid_Speed;

float Flow_Original = 0;
float speed = 0;
float flow_valid = 0;

extern rmc620_t chassis_motor[4];
extern float flow_valid;
extern int Current;
extern float speed;
extern uint32_t blockSize;
extern uint32_t numBlocks; /* 需要调用arm_fir_f32的次数 */
extern float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
extern float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 滤波后的输出 */
extern float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* 状态缓存，大小numTaps + blockSize - 1*/

void Motor_Cal(void)
{
	
	
								
//	Pid_Current.SetPoint = PID_Calc(&Pid_Speed,speed);
	Current_Change = PID_Calc(&Pid_Current,flow_valid);
	Current_f += Current_Change;
	Current = (int)Current_f;
}

void Motor_Pid(void)
{
//电流的目标值越大，需要更大的I来保持稳态，但是I会拖延进入稳态的时间。
//必须在嵌套的条件下调参数
	Pid_Current.P = 0.026;
	Pid_Current.I = 0.00008;
	Pid_Current.D = 0.4;
	Pid_Current.IMax = 2500;
	Pid_Current.SetPoint = 0;
	
	Pid_Speed.P = 5;
	Pid_Speed.I = 0.03;
	Pid_Speed.D = 50;
	Pid_Speed.IMax = 50000;
	Pid_Speed.SetPoint = 0;
}
void Filter_Excu(void)
{
	int i = 0;
	
	speed = (float)chassis_motor[0].speed;
	Flow_Original = (float)chassis_motor[0].flow;
	
	for(i = 0;i < TEST_LENGTH_SAMPLES-1;i ++)
	{
		testInput_f32_50Hz_200Hz[i] = testInput_f32_50Hz_200Hz[i+1];
	}
	testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES-1] = Flow_Original;
	arm_fir_f32_lp();
	flow_valid = testOutput[TEST_LENGTH_SAMPLES-1];
}
