#ifndef __FIR_H__
#define __FIR_H__

#define TEST_LENGTH_SAMPLES 32 /* �������� */
#define BLOCK_SIZE 32 /* ����һ��arm_fir_f32����Ĳ�������� */
#define NUM_TAPS 29 /* �˲���ϵ������ */
#define FIR_DELAY 14
void arm_fir_f32_lp(void);

#endif
