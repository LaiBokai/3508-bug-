#include "main.h"


rmc620_t chassis_motor[4];
int can = 0;



/**
  * @brief  配置CAN1
  * @param  None
  * @retval None
  */
void CAN1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* 打开GPIO时钟、AFIO时钟，CAN时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


	/* CAN1 RX PB8 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* CAN1 TX PB9 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);  // CAN1 remap

	/* CAN1 Enabling interrupt */	
	NVIC_Config(CAN1_RX0_IRQn, 0, 0);									
	NVIC_Config(CAN1_TX_IRQn, 1, 0);
	
	/* CAN  BaudRate = RCC_APB1PeriphClock/(CAN_SJW+CAN_BS1+CAN_BS2)/CAN_Prescaler */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);   

	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=ENABLE;
	CAN_InitStructure.CAN_AWUM=ENABLE;
	CAN_InitStructure.CAN_NART=ENABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=ENABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;   
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;  
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;	
	CAN_InitStructure.CAN_Prescaler=4;

	CAN_Init(CAN1,&CAN_InitStructure);	// CAN1											

	CAN_FilterInitStructure.CAN_FilterNumber=0;	 
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	 // 标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   // 32位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;			// 过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;				
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		// 过滤器屏蔽标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	 // FIFO0指向过滤器
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
//	CAN1->IER = 0x03;

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}
/**
  * @brief  CAN1接收中断
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg rx_message;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		
		switch(rx_message.StdId)
        {
            case 0x201:                                                          
                chassis_motor[0].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                chassis_motor[0].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
								chassis_motor[0].flow  = (rx_message.Data[4] << 8) | rx_message.Data[5];
						
								can ++;
            break;
            
            case 0x202:
                chassis_motor[1].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                chassis_motor[1].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
            break;
            
            case 0x203:
                chassis_motor[2].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                chassis_motor[2].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
            break;
            
            case 0x204:
                chassis_motor[3].angle = (rx_message.Data[0] << 8) | rx_message.Data[1];
                chassis_motor[3].speed = (rx_message.Data[2] << 8) | rx_message.Data[3];
            break;    
			
		
			default:
				break;
		}
		
		
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	}
}	
/**
  * @brief  CAN1发送数据,内带电流限幅 -16384 ~ 0 ~ 16384
  * @param  a：0x201电流给定
            b：0x202电流给定
            c：0x203电流给定
            d：0x204电流给定
  * @retval None
  */
void chassis_current_send(int a, int b, int c, int d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
	
    a = LIMIT_MAX_MIN(a, Current_Max, -Current_Max);
    b = LIMIT_MAX_MIN(b, Current_Max, -Current_Max);
    c = LIMIT_MAX_MIN(c, Current_Max, -Current_Max);
    d = LIMIT_MAX_MIN(d, Current_Max, -Current_Max);
	
    tx_message.Data[0] = (unsigned char)((a >> 8) & 0xff);
    tx_message.Data[1] = (unsigned char)(a & 0xff);  
    tx_message.Data[2] = (unsigned char)((b >> 8) & 0xff);
    tx_message.Data[3] = (unsigned char)(b & 0xff);
    tx_message.Data[4] = (unsigned char)((c >> 8) & 0xff);
    tx_message.Data[5] = (unsigned char)(c & 0xff);
    tx_message.Data[6] = (unsigned char)((d >> 8) & 0xff); 
    tx_message.Data[7] = (unsigned char)(d & 0xff);

    CAN_Transmit(CAN1, &tx_message);
}
