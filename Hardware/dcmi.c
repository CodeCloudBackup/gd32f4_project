#include "dcmi.h" 
#include "led.h" 
#include "ov2640.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DCMI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

u8 ov_frame=0;  						//֡��
extern void jpeg_data_process(void);
//DCMI�жϷ�����
void DCI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)//����һ֡ͼ��
	{
		jpeg_data_process(); 	//jpeg���ݴ���	
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//���֡�ж�
		LED_B=!LED_B;
		ov_frame++;  
	}
} 
//DCMI DMA����
//memaddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��    @defgroup DMA_memory_data_size :DMA_MemoryDataSize_Byte/DMA_MemoryDataSize_HalfWord/DMA_MemoryDataSize_Word
//DMA_MemoryInc:�洢��������ʽ  @defgroup DMA_memory_incremented_mode  /** @defgroup DMA_memory_incremented_mode : DMA_MemoryInc_Enable/DMA_MemoryInc_Disable
 
void DCMI_DMA_Init(u32 DMA_Memory0BaseAddr,u32 DMA_Memory1BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc)
{ 
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_DMA1,ENABLE);//DMA2ʱ��ʹ�� 
	DMA_DeInit(DMA1_Stream1);//�ȴ�DMA1_Stream1
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE){}//�ȴ�DMA1_Stream1������ 
	
  /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  //ͨ��1 DCMIͨ�� 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;//�����ַΪ:DCMI->DR
  DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
  DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;//�洢�����ݳ��� 
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
  DMA_Init(DMA1_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream
		
	if(DMA_Memory1BaseAddr)
  {
		DMA_DoubleBufferModeCmd(DMA1_Stream1,ENABLE);//˫����ģʽ
	  DMA_MemoryTargetConfig(DMA1_Stream1,DMA_Memory1BaseAddr,DMA_Memory_1);//����Ŀ���ַ1
		DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);//������������ж�
		
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	}
		
	
} 

void (*dcmi_rx_callback)(void);//DCMI DMA���ջص�����
//DMA1_Stream1�жϷ�����(��˫����ģʽ���õ�)
void DMA1_Stream1_IRQHandler(void)
{        
	if(DMA_GetFlagStatus(DMA1_Stream1,DMA_FLAG_TCIF1)==SET)//DMA2_Steam1,������ɱ�־
	{  
		 DMA_ClearFlag(DMA1_Stream1,DMA_FLAG_TCIF1);//�����������ж�
     dcmi_rx_callback();	//ִ������ͷ���ջص�����,��ȡ���ݵȲ����������洦��  
	}    											 
}  

//DCMI��ʼ��
void MyDCMI_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;		
	
  RCU_AHB1PeriphClockCmd(RCU_AHB1Periph_GPIOA|RCU_AHB1Periph_GPIOC|RCU_AHB1Periph_GPIOD|RCU_AHB1Periph_GPIOE|RCU_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOA B C E ʱ��
	RCU_AHB2PeriphClockCmd(RCU_AHB2Periph_DCMI,ENABLE);
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;//PA4/6  HSYNC/PLCLK  ���ù������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù������
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHzS
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_10|GPIO_Pin_12;//PC6/7/8   D0/1/2  ���ù������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PD3 D5  ���ù������ 
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;//PE4/5/6 D4/6/7 ���ù������
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;//PG9/11 VSYNC/D3���ù������ 
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��	

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PB4,AF13  DCMI_D0 
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI); //PC5,AF13  DCMI_D1  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI); //PC6,AF13  DCMI_D2 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_DCMI); //PC5,AF13  DCMI_D8  
 	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_DCMI); //PC6,AF13  DCMI_D9 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource3,GPIO_AF_DCMI); //PD3,AF13  DCMI_D5
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource4,GPIO_AF_DCMI); //PE4,AF13  DCMI_D4
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_DCMI); //PE5,AF13 DCMI_D6
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7 
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource11,GPIO_AF_DCMI);//PG11,AF13  DCMI_D3
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_DCMI); //PG96,AF13  DCMI_VSYNC

	
	DCMI_DeInit();//���ԭ�������� 
 
 
  DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_10b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_Low;//VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 
	
	DCMI_Cmd(ENABLE);	//DCMIʹ��

  NVIC_InitStructure.NVIC_IRQChannel = DCI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
 
} 
//DCMI,��������
void DCMI_Start(void)
{  
	DMA_Cmd(DMA1_Stream1, ENABLE);//����DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��  
}
//DCMI,�رմ���
void DCMI_Stop(void)
{ 
  DCMI_CaptureCmd(DISABLE);//DCMI����ʹ�ر�	
	
	while(DCMI->CR&0X01);		//�ȴ�������� 
	 	
	DMA_Cmd(DMA1_Stream1,DISABLE);//�ر�DMA2,Stream1
} 
////////////////////////////////////////////////////////////////////////////////
//������������,��usmart����,���ڵ��Դ���

//DCMI������ʾ����
//sx,sy;LCD����ʼ����
//width,height:LCD��ʾ��Χ.
void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{
	DCMI_Stop(); 
	OV2640_OutSize_Set(width,height);
 
	DMA_Cmd(DMA1_Stream1,ENABLE);	//����DMA2,Stream1 
	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ�� 
	
}
   
//ͨ��usmart����,����������.
//pclk/hsync/vsync:�����źŵ����޵�ƽ����
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync)
{
	DCMI_InitTypeDef DCMI_InitStructure;	
	DCMI_DeInit();//���ԭ�������� 
 
  DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
	DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
	DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
	DCMI_InitStructure.DCMI_HSPolarity= hsync<<6;//HSYNC �͵�ƽ��Ч
	DCMI_InitStructure.DCMI_PCKPolarity= pclk<<5;//PCLK ��������Ч
	DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
	DCMI_InitStructure.DCMI_VSPolarity=vsync<<7;//VSYNC �͵�ƽ��Ч
	DCMI_Init(&DCMI_InitStructure);

	DCMI_CaptureCmd(ENABLE);//DCMI����ʹ�� 
	DCMI_Cmd(ENABLE);	//DCMIʹ��
  
}








