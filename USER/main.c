#include "main.h"

int cnt = 0;

int Current = 0;
float Sine_Current = 0;
RCC_ClocksTypeDef rcc;

extern PID Pid_Current;

int main()
{
	
	RCC_GetClocksFreq(&rcc);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	CAN1_Config();
	SysTick_Configuration();

	Motor_Pid();
	
	while(1)
	{
		
	}
}

void SysTick_Handler(void)
{
	cnt ++;
	if(cnt == Times * 100) cnt = 0;
//////	Pid_Current.SetPoint = 800 * sin(cnt * 3.1415926535 / 1000);
	if(cnt % Times == 0) Filter_Excu();
	Motor_Cal();
//	chassis_current_send(Current,0,0,0);
	chassis_current_send(0,0,0,0);
}

void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 10 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))                //SysTick���ú��� T = 1 / 1000 = 1ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);                       //SysTick�ж����ȼ�����
}
//�ؼ�����SysTick_Config(SystemCoreClock / 100�������
//�ú����������Ǵ򿪼����������ü������ڣ���ʹ���жϡ���������Ĳ��������������ڡ�
//���ڼ��㷽���ǣ�Ĭ������´������ڳ�ʼ��ʱ�����SystemInit��������ϵͳƵ����Ϊ72Mhz����SystemCoreClock=72000000��
//          ���Լ����Ϊϵͳһ����72000000�Σ���ô�������Ҫ10ms�Ķ�ʱ���ڣ�ֻ��Ҫ��������Ϊ720000���ͿɴﵽҪ��
//NVIC_SetPriority(SysTick_IRQn, 0x0); //�������ȼ�Ϊ��ߣ�Ӧ�����Լ�����Ҫ���е���
