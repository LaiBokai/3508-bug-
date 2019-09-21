#ifndef __FIR_H__
#define __FIR_H__

#define TEST_LENGTH_SAMPLES 32 /* 采样点数 */
#define BLOCK_SIZE 32 /* 调用一次arm_fir_f32处理的采样点个数 */
#define NUM_TAPS 29 /* 滤波器系数个数 */
#define FIR_DELAY 14
void arm_fir_f32_lp(void);

#endif
