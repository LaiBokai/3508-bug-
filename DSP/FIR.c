#include "main.h"

int Filter_Init_Flag = 0;

uint32_t blockSize = BLOCK_SIZE;
uint32_t numBlocks = TEST_LENGTH_SAMPLES/BLOCK_SIZE; /* ��Ҫ����arm_fir_f32�Ĵ��� */
float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* ������ */
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* �˲������� */
float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1]; /* ״̬���棬��СnumTaps + blockSize - 1*/
/* ��ͨ�˲���ϵ�� ͨ��fadtool��ȡ*/
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
* �� �� ��: arm_fir_f32_lp
* ����˵��: ���ú���arm_fir_f32_lpʵ�ֵ�ͨ�˲���
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void arm_fir_f32_lp(void)
{
	uint32_t i;
	arm_fir_instance_f32 S;
	float32_t *inputF32, *outputF32;
	/* ��ʼ�������������ָ�� */
	inputF32 = &testInput_f32_50Hz_200Hz[0];
	outputF32 = &testOutput[0];
	if(Filter_Init_Flag == 0)
	{
		/* ��ʼ���ṹ��S */
	  arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP[0], &firStateF32[0], blockSize);
//		Filter_Init_Flag = 1;
	}

	/* ʵ��FIR�˲� */
	for(i=0; i < numBlocks; i++)
	{
		arm_fir_f32(&S, inputF32 + (i * blockSize), outputF32 + (i * blockSize), blockSize);
	}
}
