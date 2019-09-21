#ifndef __IIR_H__
#define __IIR_H__

#include "main.h"

#define numStages 2 /* 2阶IIR滤波的个数 */
#define TEST_LENGTH_SAMPLES 100 /* 采样点数 */

void arm_iir_f32_lp(void);

#endif
