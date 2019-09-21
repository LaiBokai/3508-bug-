#include "main.h"


float32_t testInput_f32_50Hz_200Hz[TEST_LENGTH_SAMPLES]; /* ������ */
float32_t testOutput[TEST_LENGTH_SAMPLES]; /* �˲������� */
float32_t IIRStateF32[4*numStages]; /* ״̬���棬��СnumTaps + blockSize - 1*/
float32_t ScaleValue;

/* ������˹��ͨ�˲���ϵ�� 80Hz*/ 
const float32_t IIRCoeffs32LP[5*numStages] = {
			1.0f, 2.0f, 1.0f, 1.949215958025841466394467715872451663017f, -0.953069895327890548308857887604972347617f, 
			1.0f, 2.0f, 1.0f, 1.886609582621506353561358082515653222799f, -0.890339736284024185763996683817822486162f 
};

/*
*********************************************************************************************************
* �� �� ��: arm_iir_f32_lp
* ����˵��: ���ú���arm_iir_f32_lpʵ�ֵ�ͨ�˲���
* �� �Σ���
* �� �� ֵ: ��
*********************************************************************************************************
*/
void arm_iir_f32_lp(void)
{
		uint32_t i;
		arm_biquad_casd_df1_inst_f32 S;
	
		/* ��ʼ�� */
		arm_biquad_cascade_df1_init_f32(&S, numStages, (float32_t *)&IIRCoeffs32LP[0], (float32_t
		*)&IIRStateF32[0]);
		/* IIR�˲� */
		arm_biquad_cascade_df1_f32(&S, testInput_f32_50Hz_200Hz, testOutput, TEST_LENGTH_SAMPLES);
		/*����ϵ�� */
		ScaleValue = 0.000963484325512290753178168412063087089f * 0.000932538415629474422112454856659269353f;//  0.008263796585715696765839233250972029055f * 0.007549433592825576314067070882174448343f;
		/* ��ӡ�˲����� */
		for(i=0; i<TEST_LENGTH_SAMPLES; i++)
		{
//			printf("%f\r\n", testOutput[i]*ScaleValue);
		} 
}
