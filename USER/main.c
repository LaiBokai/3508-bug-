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
  if (SysTick_Config(SystemCoreClock / 1000))                //SysTick配置函数 T = 1 / 1000 = 1ms
  { 
    /* Capture error */ 
    while (1);
  }  
 /* Configure the SysTick handler priority */
  NVIC_SetPriority(SysTick_IRQn, 0x0);                       //SysTick中断优先级设置
}
//关键在于SysTick_Config(SystemCoreClock / 100）的理解
//该函数的作用是打开计数器，设置计数周期，并使能中断。他所传入的参数，即计数周期。
//周期计算方法是：默认情况下处理器在初始化时会调用SystemInit（），把系统频率设为72Mhz，即SystemCoreClock=72000000；
//          可以简单理解为系统一秒数72000000次，那么如果我需要10ms的定时周期，只需要让重载数为720000，就可达到要求
//NVIC_SetPriority(SysTick_IRQn, 0x0); //设置优先级为最高，应根据自己的需要进行调整
