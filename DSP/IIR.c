#include "main.h"


float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* 采样点 */
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* 滤波后的输出 */
float32_t IIRStateF32[4*numStages]; /* 状态缓存，大小numTaps + blockSize - 1*/
float32_t ScaleValue;

/* 巴特沃斯低通滤波器系数 80Hz*/ 
const float32_t IIRCoeffs32LP[5*numStages] = {
			1.0f, 2.0f, 1.0f, 1.949215958025841466394467715872451663017f, -0.953069895327890548308857887604972347617f, 
			1.0f, 2.0f, 1.0f, 1.886609582621506353561358082515653222799f, -0.890339736284024185763996683817822486162f 
};

/*
*********************************************************************************************************
* 函 数 名: arm_iir_f32_lp
* 功能说明: 调用函数arm_iir_f32_lp实现低通滤波器
* 形 参：无
* 返 回 值: 无
*********************************************************************************************************
*/
void arm_iir_f32_lp(void)
{
		uint32_t i;
		arm_biquad_casd_df1_inst_f32 S;
	
		/* 初始化 */
		arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32LP[0], (float32_t
		*)&IIRStateF32[0]);
		/* IIR滤波 */
		arm_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput, TEST_LENGTH_SAMPLES);
		/*放缩系数 */
		ScaleValue = 0.000963484325512290753178168412063087089f * 0.000932538415629474422112454856659269353f;//  0.008263796585715696765839233250972029055f * 0.007549433592825576314067070882174448343f;
		/* 打印滤波后结果 */
		for(i=0; i<TEST_LENGTH_SAMPLES; i++)
		{
//			printf("%f\r\n", testOutput[i]*ScaleValue);
		} 
}
