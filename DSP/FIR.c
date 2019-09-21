#include "main.h"

int Filter_Init_Flag = 0;

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE; /* 需要调用arm_fir_f32的次数 */
float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 滤波后的输出 */
float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* 状态缓存，大小numTaps + blockSize - 1*/
/* 低通滤波器系数 通过fadtool获取*/
const float32_t firCoeffs32LP[NUM_TAPS] = {
   0.001152354875, 0.001878723036, 0.003374658059, 0.006014463957,    0.010079992,
    0.01570816152,  0.02285434678,  0.03127748892,  0.04055009037,  0.05009286478,
     0.0592305623,  0.06726240367,  0.07353857905,  0.07753318548,  0.07890424877,
    0.07753318548,  0.07353857905,  0.06726240367,   0.0592305623,  0.05009286478,
    0.04055009037,  0.03127748892,  0.02285434678,  0.01570816152,    0.010079992,
   0.006014463957, 0.003374658059, 0.001878723036, 0.001152354875
};
/*
*********************************************************************************************************
* 函 数 名: arm_fir_f32_lp
* 功能说明: 调用函数arm_fir_f32_lp实现低通滤波器
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
void arm_fir_f32_lp(void)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t *inputF32, *outputF32;
	/* 初始化输入输出缓存指针 */
	inputF32 = &testInput_f32_50Hz_200Hz[0];
	outputF32 = &testOutput[0];
	if(Filter_Init_Flag == 0)
	{
		/* 初始化结构体S */
	  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
//		Filter_Init_Flag = 1;
	}

	/* 实现FIR滤波 */
	for(i=0; i < numBlocks; i++)
	{
		arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}
}
