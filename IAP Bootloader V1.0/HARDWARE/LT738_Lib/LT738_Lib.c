#include "LT738_Lib.h"
#include "usart.h"

unsigned char CCLK;    // LT738的内核时钟频率    
unsigned char MCLK;    // SDRAM的时钟频率
unsigned char SCLK;    // LCD的扫描时钟频率

//---------------------------------------------------------------------------------------------------------------------------------

//复位LT738
void LT738_HW_Reset(void)
{	
	
	RCC->AHB1ENR|=1<<6;     	//使能PG时钟 	
  GPIO_Set(GPIOG,PIN6,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);	  
	LCD_RST=1;	
	Delay_ms(10);
	LCD_RST=0;
	Delay_ms(100);				   
	LCD_RST=1;	
	Delay_ms(200);  //LT738复位
}

//检查LT738系统
void System_Check_Temp(void)
{
	unsigned char i=0;
	unsigned char temp=0;
	unsigned char system_ok=0;
	do
	{
		if((LCD_StatusRead()&0x02)==0x00)    
		{
			Delay_ms(3);                  //若MCU 速度太快，必要時使用
			LCD_CmdWrite(0x01);
			Delay_ms(3);                  //若MCU 速度太快，必要時使用
			temp =LCD_DataRead();
			if((temp & 0x80)==0x80)       //检测CCR寄存器PLL是否准备好
			{
				system_ok=1;
				i=0;
			}
			else
			{
				Delay_ms(3); //若MCU 速度太快，必要時使用//1ms
				LCD_CmdWrite(0x01);
				Delay_ms(3); //若MCU 速度太快，必要時使用//1ms
				LCD_DataWrite(0x80);
			}
		}
		else
		{
			system_ok=0;
			i++;
		}
		if(system_ok==0 && i==5)
		{
			LT738_HW_Reset(); //note1
			i=0;
		}
	}while(system_ok==0);
}

void LT738_PLL_Initial(void) 
{    
	unsigned int  temp = 0,temp1 =0 ;
	
	unsigned short lpllOD_sclk, lpllOD_cclk, lpllOD_mclk;
	unsigned short lpllR_sclk, lpllR_cclk, lpllR_mclk;
	unsigned short lpllN_sclk, lpllN_cclk, lpllN_mclk;
	
	temp = (LCD_HBPD + LCD_HFPD + LCD_HSPW + LCD_XSIZE_TFT) * (LCD_VBPD + LCD_VFPD + LCD_VSPW+LCD_YSIZE_TFT) * 60;   
	
	temp1=(temp%1000000)/100000;
	if(temp1>=5) 
		 temp = temp / 1000000 + 1;
	else temp = temp / 1000000;
	
	
	SCLK = temp;
	temp = temp * 3;
	MCLK = temp;
	CCLK = temp;
	
	if(CCLK > 100)	CCLK = 100;
	if(MCLK > 100)	MCLK = 100;
	if(SCLK > 65)	SCLK = 65;

#if XI_4M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 3;
	lpllOD_mclk = 3;
	lpllR_sclk  = 2;
	lpllR_cclk  = 2;
	lpllR_mclk  = 2;
	lpllN_mclk  = 2*MCLK;      
	lpllN_cclk  = 2*CCLK;    
	lpllN_sclk  = 2*SCLK; 
	
#endif

#if XI_8M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 3;
	lpllOD_mclk = 3;
	lpllR_sclk  = 2;
	lpllR_cclk  = 2;
	lpllR_mclk  = 2;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = SCLK; 
	
#endif

#if XI_10M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 3;
	lpllOD_mclk = 3;
	lpllR_sclk  = 5;
	lpllR_cclk  = 5;
	lpllR_mclk  = 5;
	lpllN_mclk  = 2*MCLK;      
	lpllN_cclk  = 2*CCLK;    
	lpllN_sclk  = 2*SCLK; 
	
#endif

#if XI_12M 	
	
	lpllOD_sclk = 3;
	lpllOD_cclk = 3;
	lpllOD_mclk = 3;
	lpllR_sclk  = 3;
	lpllR_cclk  = 3;
	lpllR_mclk  = 3;
	lpllN_mclk  = MCLK;      
	lpllN_cclk  = CCLK;    
	lpllN_sclk  = SCLK; 
	
#endif

	LCD_CmdWrite(0x05);
	LCD_DataWrite((lpllOD_sclk<<6) | (lpllR_sclk<<1) | ((lpllN_sclk>>8)&0x1));
	LCD_CmdWrite(0x07);
	LCD_DataWrite((lpllOD_mclk<<6) | (lpllR_mclk<<1) | ((lpllN_mclk>>8)&0x1));
	LCD_CmdWrite(0x09);
	LCD_DataWrite((lpllOD_cclk<<6) | (lpllR_cclk<<1) | ((lpllN_cclk>>8)&0x1));

	LCD_CmdWrite(0x06);
	LCD_DataWrite(lpllN_sclk);
	LCD_CmdWrite(0x08);
	LCD_DataWrite(lpllN_mclk);
	LCD_CmdWrite(0x0a);
	LCD_DataWrite(lpllN_cclk);
      
	LCD_CmdWrite(0x00);
	Delay_us(1);
	LCD_DataWrite(0x80);

	Delay_ms(1);	//单PLL铆
}


void LT738_SDRAM_initail(unsigned char mclk)
{
	unsigned short sdram_itv;

	LCD_RegisterWrite(0xe0,0x20);
	LCD_RegisterWrite(0xe1,0x03);	//CAS:2=0x02CAS:3=0x03
	sdram_itv = (64000000 / 8192) / (1000/mclk) ;//
	sdram_itv-=2;

	LCD_RegisterWrite(0xe2,sdram_itv);
	LCD_RegisterWrite(0xe3,sdram_itv >>8);
	LCD_RegisterWrite(0xe4,0x01);
	Check_SDRAM_Ready();
	Delay_ms(1);
}


void Set_LCD_Panel(void)
{
	//**[01h]**//
	TFT_16bit();
	#if GD32_FSMC_8
	Host_Bus_8bit();    //主机总线8bit
	#else
	Host_Bus_16bit();	//主机总线16bit
	#endif     
      
	//**[02h]**//
	RGB_16b_16bpp();
  //Panel_Synchronous_Signals();
	
	MemWrite_Left_Right_Top_Down();	
  

	Graphic_Mode();
	Memory_Select_SDRAM();
	PCLK_Falling();	       	//REG[12h]:下降沿 

	//PCLK_Rising();
	
	VSCAN_T_to_B();	        //REG[12h]:从上到下
	//VSCAN_B_to_T();				//从下到上
// 	HSCAN_L_to_R();
	
	PDATA_Set_RGB();        //REG[12h]:Select RGB output
	//PDATA_Set_RBG();
	//PDATA_Set_GRB();
	//PDATA_Set_GBR();
	//PDATA_Set_BRG();
	//PDATA_Set_BGR();

	//HSYNC_Low_Active();     //REG[13h]:		  
	HSYNC_High_Active();
	
	//VSYNC_Low_Active();     //REG[13h]:			
	VSYNC_High_Active();
	
	DE_High_Active();       //REG[13h]:	
	//DE_Low_Active();
 
	LCD_HorizontalWidth_VerticalHeight(LCD_XSIZE_TFT ,LCD_YSIZE_TFT);	
	LCD_Horizontal_Non_Display(LCD_HBPD);	                            
	LCD_HSYNC_Start_Position(LCD_HFPD);	                              
	LCD_HSYNC_Pulse_Width(LCD_HSPW);		                            	
	LCD_Vertical_Non_Display(LCD_VBPD);	                                
	LCD_VSYNC_Start_Position(LCD_VFPD);	                              
	LCD_VSYNC_Pulse_Width(LCD_VSPW);
	


	Memory_XY_Mode();	//Block mode (X-Y coordination addressing);块模式
	Memory_16bpp_Mode();	
}

void LT738_initial(void)
{
	LT738_PLL_Initial();
	LT738_SDRAM_initail(MCLK);
	Set_LCD_Panel();
}


void LT738_Init(void)
{
	Delay_ms(100);                    //delay for LT738 power on
	LT738_HW_Reset();                 //LT738复位
	test_LCDIO();
	System_Check_Temp();	            //检测复位是否成功
	Delay_ms(100);	
	while(LCD_StatusRead()&0x02);	    //Initial_Display_test	and  set SW2 pin2 = 1
	Delay_ms(100);
	LT738_initial();
	Delay_ms(300);
}


//--------------------------------------------------------------------------------------------------------------------------------------------

void MPU8_8bpp_Memory_Write
(
 unsigned short x           // x坐标
,unsigned short y           // y坐标
,unsigned short w           // 宽度
,unsigned short h           // 高度
,const unsigned char *data  // 数据首地址
)
{														  
	unsigned short i,j;
	Graphic_Mode();
	Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);	
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{	   
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}	 

			
void MPU8_16bpp_Memory_Write
(
 unsigned short x           // x坐标
,unsigned short y           // y坐标
,unsigned short w           // 宽度
,unsigned short h           // 高度
,const unsigned char *data  // 数据首地址
)
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}		 


void MPU8_24bpp_Memory_Write 
(
 unsigned short x           // x坐标
,unsigned short y           // y坐标
,unsigned short w           // 宽度
,unsigned short h           // 高度
,const unsigned char *data  // 数据首地址
)

{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}



#if 1
void MPU16_16bpp_Memory_Write 
(
 unsigned short x            // x坐标
,unsigned short y            // y坐标
,unsigned short w            // 宽度
,unsigned short h            // 高度
,const unsigned short *data  // 数据首地址
)			
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}
#endif 


void MPU16_24bpp_Mode1_Memory_Write 
(
 unsigned short x            // x坐标
,unsigned short y            // y坐标
,unsigned short w            // 宽度
,unsigned short h            // 高度
,const unsigned short *data  // 数据首地址
)	
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w/2;j++)
		{
		 LCD_DataWrite(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		 LCD_DataWrite(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		 LCD_DataWrite(*data);
		 Check_Mem_WR_FIFO_not_Full();
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}


void MPU16_24bpp_Mode2_Memory_Write
(
 unsigned short x            // x坐标
,unsigned short y            // y坐标
,unsigned short w            // 宽度
,unsigned short h            // 高度
,const unsigned short *data  // 数据首地址
)	
{
	unsigned short i,j;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h); 					
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{	
		for(j=0;j< w;j++)
		{
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(*data);
		 data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
}





//--------------------------------------------------------------------------------------------------------------------------------------------



//------------------------------------- 线段 -----------------------------------------
void LT738_DrawLine
(
 unsigned short X1        // X1坐标
,unsigned short Y1        // Y1坐标
,unsigned short X2        // X2坐标
,unsigned short Y2        // Y2坐标
,unsigned long  LineColor // 线段颜色
)
{
	Foreground_color_65k(LineColor);
	Line_Start_XY(X1,Y1);
	Line_End_XY(X2,Y2);
	Start_Line();
	Check_2D_Busy();
}

void LT738_DrawLine_Width
(
 unsigned short X1        // X1坐标
,unsigned short Y1        // Y1坐标
,unsigned short X2        // X2坐标
,unsigned short Y2        // Y2坐标
,unsigned long  LineColor // 线段颜色
,unsigned short Width     // 线段宽度
)
{
	unsigned short  i = 0;
	signed  short x = 0, y = 0;
	double temp = 0;
	x = X2 - X1;
	y = Y2 - Y1;
	if(x == 0) temp = 2;
	else temp = -((double)y/(double)x);
	if(temp>=-1&&temp<=1)
		{
				while(Width--)
			{
				LT738_DrawLine(X1,Y1+i,X2,Y2+i,LineColor);
				i++;
			}	
		}
		
	else 
		{
				while(Width--)
			{
				LT738_DrawLine(X1+i,Y1,X2+i,Y2,LineColor);
				i++;
			}	
		}
}


//------------------------------------- 圆 -----------------------------------------
void LT738_DrawCircle
(
 unsigned short XCenter           // 圆心X位置
,unsigned short YCenter           // 圆心Y位置
,unsigned short R                 // 半径
,unsigned long CircleColor        // 画线颜色
)
{
	Foreground_color_65k(CircleColor);
	Circle_Center_XY(XCenter,YCenter);
	Circle_Radius_R(R);
	Start_Circle_or_Ellipse();
	Check_2D_Busy(); 
}



void LT738_DrawCircle_Fill
(
 unsigned short XCenter           // 圆心X位置
,unsigned short YCenter           // 圆心Y位置
,unsigned short R                 // 半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Circle_Center_XY(XCenter,YCenter);
	Circle_Radius_R(R);
	Start_Circle_or_Ellipse_Fill();
	Check_2D_Busy(); 
}



void LT738_DrawCircle_Width
(
 unsigned short XCenter          // 圆心X位置
,unsigned short YCenter          // 圆心Y位置
,unsigned short R                // 半径
,unsigned long CircleColor       // 画线颜色
,unsigned long ForegroundColor   // 背景颜色
,unsigned short Width            // 线宽
)
{
	LT738_DrawCircle_Fill(XCenter,YCenter,R+Width,CircleColor);
	LT738_DrawCircle_Fill(XCenter,YCenter,R,ForegroundColor);
}


//------------------------------------- 椭圆 -----------------------------------------
void LT738_DrawEllipse
(
 unsigned short XCenter          // 椭圆心X位置
,unsigned short YCenter          // 椭圆心Y位置
,unsigned short X_R              // 宽半径
,unsigned short Y_R              // 长半径
,unsigned long EllipseColor      // 画线颜色
)
{
	Foreground_color_65k(EllipseColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Circle_or_Ellipse();
	Check_2D_Busy(); 
}

void LT738_DrawEllipse_Fill
(
 unsigned short XCenter           // 椭圆心X位置
,unsigned short YCenter           // 椭圆心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Circle_or_Ellipse_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawEllipse_Width
(
 unsigned short XCenter           // 椭圆心X位置
,unsigned short YCenter           // 椭圆心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long EllipseColor       // 画线颜色
,unsigned long ForegroundColor    // 背景颜色
,unsigned short Width             // 线宽
)
{
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R+Width,Y_R+Width,EllipseColor);
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
}



//------------------------------------- 矩形 -----------------------------------------
void LT738_DrawSquare
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned long SquareColor        // 画线颜色
)
{
	Foreground_color_65k(SquareColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2);
	Start_Square();
	Check_2D_Busy(); 
}


#if 1
void LT738_DrawSquare_Fill
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2);
	Start_Square_Fill();
	Check_2D_Busy();
}

#endif


#if 0
void LT738_DrawSquare_Fill
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_16M(ForegroundColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2);
	Start_Square_Fill();
	Check_2D_Busy();
}

#endif


void LT738_DrawSquare_Width
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned long SquareColor        // 画线颜色
,unsigned long ForegroundColor    // 背景颜色
,unsigned short Width             // 线宽
)
{
	LT738_DrawSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,SquareColor);
	LT738_DrawSquare_Fill(X1,Y1,X2,Y2,ForegroundColor);
}


//------------------------------------- 圆角矩形 -----------------------------------------
void LT738_DrawCircleSquare
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CircleSquareColor  // 画线颜色
)
{
	Foreground_color_65k(CircleSquareColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2); 
	Circle_Square_Radius_RxRy(X_R,Y_R);
	Start_Circle_Square();
	Check_2D_Busy();
}



void LT738_DrawCircleSquare_Fill
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Square_Start_XY(X1,Y1);
	Square_End_XY(X2,Y2); 
	Circle_Square_Radius_RxRy(X_R,Y_R);
	Start_Circle_Square_Fill();
	Check_2D_Busy(); 
}



void LT738_DrawCircleSquare_Width
(
 unsigned short X1                // X1位置
,unsigned short Y1                // Y1位置
,unsigned short X2                // X2位置
,unsigned short Y2                // Y2位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CircleSquareColor  // 画线颜色
,unsigned long ForegroundColor    // 画线颜色
,unsigned short Width             // 宽度
)
{
	LT738_DrawCircleSquare_Fill(X1-Width,Y1-Width,X2+Width,Y2+Width,X_R,Y_R,CircleSquareColor);
	LT738_DrawCircleSquare_Fill(X1,Y1,X2,Y2,X_R,Y_R,ForegroundColor);
}


//------------------------------------- 三角形 -----------------------------------------
void LT738_DrawTriangle
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned long TriangleColor    // 画线颜色
)
{
	Foreground_color_65k(TriangleColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Start_Triangle();
	Check_2D_Busy(); 
}



void LT738_DrawTriangle_Fill
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Start_Triangle_Fill();
	Check_2D_Busy();
}

void LT738_DrawTriangle_Frame
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned long TriangleColor    // 画线颜色
,unsigned long ForegroundColor  // 背景颜色
)
{
	LT738_DrawTriangle_Fill(X1,Y1,X2,Y2,X3,Y3,ForegroundColor);
	LT738_DrawTriangle(X1,Y1,X2,Y2,X3,Y3,TriangleColor);
}



//------------------------------------- 曲线 -----------------------------------------
void LT738_DrawLeftUpCurve
( 
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Up_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawLeftDownCurve
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Down_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawRightUpCurve
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Up_Curve();
	Check_2D_Busy(); 
}


void LT738_DrawRightDownCurve
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
)
{
	Foreground_color_65k(CurveColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Down_Curve();
	Check_2D_Busy(); 
}


void LT738_SelectDrawCurve
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
,unsigned short  Dir              // 方向
)
{
	switch(Dir)
	{
		case 0:LT738_DrawLeftDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 1:LT738_DrawLeftUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 2:LT738_DrawRightUpCurve(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 3:LT738_DrawRightDownCurve(XCenter,YCenter,X_R,Y_R,CurveColor);	break;
		default:																															break;
	}
}


//------------------------------------- 1/4实心椭圆 -----------------------------------------
void LT738_DrawLeftUpCurve_Fill
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Up_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawLeftDownCurve_Fill
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Left_Down_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawRightUpCurve_Fill
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Up_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_DrawRightDownCurve_Fill
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long ForegroundColor    // 背景颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Ellipse_Center_XY(XCenter,YCenter);
	Ellipse_Radius_RxRy(X_R,Y_R);
	Start_Right_Down_Curve_Fill();
	Check_2D_Busy(); 
}


void LT738_SelectDrawCurve_Fill
(
 unsigned short XCenter           // 曲心X位置
,unsigned short YCenter           // 曲心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned long CurveColor         // 画线颜色
,unsigned short  Dir              // 方向
)
{
	switch(Dir)
	{
		case 0:LT738_DrawLeftDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		case 1:LT738_DrawLeftUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 2:LT738_DrawRightUpCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);			break;
		case 3:LT738_DrawRightDownCurve_Fill(XCenter,YCenter,X_R,Y_R,CurveColor);		break;
		default:																																		break;
	}
}



//------------------------------------- 四边形 -----------------------------------------

void LT738_DrawQuadrilateral
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned short X4              // X4位置
,unsigned short Y4              // Y4位置
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0x8d);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


void LT738_DrawQuadrilateral_Fill
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned short X4              // X4位置
,unsigned short Y4              // Y4位置
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0xa7);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}



//------------------------------------- 五边形 -----------------------------------------

void LT738_DrawPentagon
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned short X4              // X4位置
,unsigned short Y4              // Y4位置
,unsigned short X5              // X5位置
,unsigned short Y5              // Y5位置
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);
	Ellipse_Center_XY(X5,Y5);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0x8F);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


void LT738_DrawPentagon_Fill
(
 unsigned short X1              // X1位置
,unsigned short Y1              // Y1位置
,unsigned short X2              // X2位置
,unsigned short Y2              // Y2位置
,unsigned short X3              // X3位置
,unsigned short Y3              // Y3位置
,unsigned short X4              // X4位置
,unsigned short Y4              // Y4位置
,unsigned short X5              // X5位置
,unsigned short Y5              // Y5位置
,unsigned long ForegroundColor  // 画线颜色
)
{
	Foreground_color_65k(ForegroundColor);
	Triangle_Point1_XY(X1,Y1);
	Triangle_Point2_XY(X2,Y2);
	Triangle_Point3_XY(X3,Y3);
	Ellipse_Radius_RxRy(X4,Y4);
	Ellipse_Center_XY(X5,Y5);

	LCD_CmdWrite(0x67);
	LCD_DataWrite(0xa9);
	Check_Busy_Draw();

	Check_2D_Busy(); 
}


//------------------------------------- 圆柱 -----------------------------------------
unsigned char LT738_DrawCylinder
(
 unsigned short XCenter           // 椭圆心X位置
,unsigned short YCenter           // 椭圆心Y位置
,unsigned short X_R               // 宽半径
,unsigned short Y_R               // 长半径
,unsigned short H                 // 高度
,unsigned long CylinderColor      // 画线颜色
,unsigned long ForegroundColor    // 背景颜色
)
{
	if(YCenter < H)	return 1;
	
	//底面椭圆
	LT738_DrawEllipse_Fill(XCenter,YCenter,X_R,Y_R,ForegroundColor);
	LT738_DrawEllipse(XCenter,YCenter,X_R,Y_R,CylinderColor);
	
	//中间矩形
	LT738_DrawSquare_Fill(XCenter-X_R,YCenter-H,XCenter+X_R,YCenter,ForegroundColor);
	
	//顶面椭圆
	LT738_DrawEllipse_Fill(XCenter,YCenter-H,X_R,Y_R,ForegroundColor);
	LT738_DrawEllipse(XCenter,YCenter-H,X_R,Y_R,CylinderColor);
	
	LT738_DrawLine(XCenter-X_R,YCenter,XCenter-X_R,YCenter-H,CylinderColor);
	LT738_DrawLine(XCenter+X_R,YCenter,XCenter+X_R,YCenter-H,CylinderColor);
	
	return 0;
}


//------------------------------------- 四棱柱 -----------------------------------------
void LT738_DrawQuadrangular
(
 unsigned short X1
,unsigned short Y1
,unsigned short X2
,unsigned short Y2
,unsigned short X3
,unsigned short Y3
,unsigned short X4
,unsigned short Y4
,unsigned short X5
,unsigned short Y5
,unsigned short X6
,unsigned short Y6
,unsigned long QuadrangularColor   // 画线颜色
,unsigned long ForegroundColor     // 背景颜色
)
{
	LT738_DrawSquare_Fill(X1,Y1,X5,Y5,ForegroundColor);
	LT738_DrawSquare(X1,Y1,X5,Y5,QuadrangularColor);
	
	LT738_DrawQuadrilateral_Fill(X1,Y1,X2,Y2,X3,Y3,X4,Y4,ForegroundColor);
	LT738_DrawQuadrilateral(X1,Y1,X2,Y2,X3,Y3,X4,Y4,QuadrangularColor);
	
	LT738_DrawQuadrilateral_Fill(X3,Y3,X4,Y4,X5,Y5,X6,Y6,ForegroundColor);
	LT738_DrawQuadrilateral(X3,Y3,X4,Y4,X5,Y5,X6,Y6,QuadrangularColor);
}


//----------------------------------------------------------------------表格-------------------------------------------------------------------
void LT738_MakeTable
(
	unsigned short X1,                  // 起始位置X1
	unsigned short Y1,                  // 起始位置X2
	unsigned short W,                   // 宽度
	unsigned short H,                   // 高度
	unsigned short Line,                // 行数
	unsigned short Row,                 // 列数
	unsigned long  TableColor,          // 线框颜色C1
	unsigned long  ItemColor,  					// 项目拦背景色C2
	unsigned long  ForegroundColor,     // 内部窗口背景色C3
	unsigned short width1,              // 内框宽度
	unsigned short width2,              // 外框宽度
	unsigned char  mode                 // 0：项目栏纵向   1：项目栏横向 
)
{
	unsigned short i = 0;
	unsigned short x2,y2;
	x2 = X1 + W * Row;
	y2 = Y1 + H * Line;
	
	LT738_DrawSquare_Width(X1,Y1,x2,y2,TableColor,ForegroundColor,width2);  
	
	if(mode == 0)	      LT738_DrawSquare_Fill(X1,Y1,X1+W,y2,ItemColor);  
	else if(mode == 1)	LT738_DrawSquare_Fill(X1,Y1,x2,Y1+H,ItemColor); 
	
	for(i = 0 ; i < Line ; i++)
	{
		LT738_DrawLine_Width(X1,Y1+i*H,x2,Y1+i*H,TableColor,width1);
	}
	
	for(i = 0 ; i < Row ; i++)
	{
		LT738_DrawLine_Width(X1+i*W,Y1,X1+i*W,y2,TableColor,width1);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------

void LT738_Color_Bar_ON(void)
{
	Color_Bar_ON();
}

void LT738_Color_Bar_OFF(void)
{
	Color_Bar_OFF();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
/* 选择内部集成字库初始化 */
void LT738_Select_Internal_Font_Init
(
 unsigned char Size         // 设置字体大小  16：16*16     24:24*24    32:32*32
,unsigned char XxN          // 字体的宽度放大倍数：1~4
,unsigned char YxN          // 字体的高度放大倍数：1~4
,unsigned char ChromaKey    // 0：字体背景色透明    1：可以设置字体的背景色
,unsigned char Alignment    // 0：不字体不对齐      1：字体对齐
)
{
	if(Size==16)	Font_Select_8x16_16x16();
	if(Size==24)	Font_Select_12x24_24x24();
	if(Size==32)	Font_Select_16x32_32x32();

	//(*)
	if(XxN==1)	Font_Width_X1();
	if(XxN==2)	Font_Width_X2();
	if(XxN==3)	Font_Width_X3();
	if(XxN==4)	Font_Width_X4();

	//(*)
	if(YxN==1)	Font_Height_X1();
	if(YxN==2)	Font_Height_X2();
	if(YxN==3)	Font_Height_X3();
	if(YxN==4)	Font_Height_X4();

	//(*)
	if(ChromaKey==0)	Font_Background_select_Color();	
	if(ChromaKey==1)	Font_Background_select_Transparency();	

	//(*)
	if(Alignment==0)	Disable_Font_Alignment();
	if(Alignment==1)	Enable_Font_Alignment();
}


/* 显示内部集成字体 */
void LT738_Print_Internal_Font_String
(
 unsigned short x               // 字体开始显示的x位置
,unsigned short y               // 字体开始显示的y位置
,unsigned long FontColor        // 字体的颜色
,unsigned long BackGroundColor  // 字体的背景色（注意：当字体背景初始化成透明时，设置该值无效）
,char *c                        // 数据缓冲的首地址
)
{
	Text_Mode();
	CGROM_Select_Internal_CGROM();
	Foreground_color_65k(FontColor);
	Background_color_65k(BackGroundColor);
	Goto_Text_XY(x,y);
	Show_String(c);
}

u8 FONTBUFF[1024] = {0};

#if 0
/* 选择外部集成字库初始化 */
void LT738_Select_Outside_Font_Init
(
 unsigned char SCS           // 选择外挂的SPI   : SCS：0       SCS：1
,unsigned char Clk           // SPI时钟分频参数 : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long FlashAddr     // 源地址(Flash)
,unsigned long MemoryAddr    // 初始化图层地址(SDRAM)
,unsigned long DisplayAddr	 // 显示图层地址
,unsigned long Num           // 字库的数据量大小
,unsigned char Size          // 设置字体大小  16：16*16     24:24*24    32:32*32
,unsigned char XxN           // 字体的宽度放大倍数：1~4
,unsigned char YxN           // 字体的高度放大倍数：1~4
,unsigned char ChromaKey     // 1：字体背景色透明    0：可以设置字体的背景色
,unsigned char Alignment     // 0：不字体不对齐      1：字体对齐
)
{
	u16 i,/*j,*/k,m,temp;
	if(Size==16)	Font_Select_8x16_16x16();
	if(Size==24)	Font_Select_12x24_24x24();
	if(Size==32)	Font_Select_16x32_32x32();

	//(*)
	if(XxN==1)	Font_Width_X1();
	if(XxN==2)	Font_Width_X2();
	if(XxN==3)	Font_Width_X3();
	if(XxN==4)	Font_Width_X4();

	//(*)
	if(YxN==1)	Font_Height_X1();
	if(YxN==2)	Font_Height_X2();
	if(YxN==3)	Font_Height_X3();
	if(YxN==4)	Font_Height_X4();

	//(*)
	if(ChromaKey==0)	Font_Background_select_Color();	
	if(ChromaKey==1)	Font_Background_select_Transparency();	

	//(*)
	if(Alignment==0)	Disable_Font_Alignment();
	if(Alignment==1)	Enable_Font_Alignment();	
	
	i = Num/1024;
	Canvas_Image_Start_address(MemoryAddr);
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(k=0;k<i;k++)
	{
		W25X_Read_Data(FONTBUFF,FlashAddr+k*1024,1024);
		for(m = 0 ; m < 1024 ; m+=2)
		{
			if((m%32)==0)	Check_Mem_WR_FIFO_not_Full();
			temp = ((FONTBUFF[m+1]<<8)+FONTBUFF[m]);
			LCD_DataWrite_Pixel(temp);
		}
	}
	Check_Mem_WR_FIFO_Empty();
	CGRAM_Start_address(MemoryAddr);
	Canvas_Image_Start_address(DisplayAddr);
}

#endif 

/* 显示外部及内部集成字体 */
void LT738_Print_Outside_Font_String
(
 unsigned short x               // 字体开始显示的x位置
,unsigned short y               // 字体开始显示的y位置
,unsigned long FontColor        // 字体的颜色
,unsigned long BackGroundColor  // 字体的背景色（注意：当字体背景初始化成透明时，设置该值无效）
,unsigned char *c               // 数据缓冲的首地址
)
{
	unsigned short temp_H = 0;
	unsigned short temp_L = 0;
	unsigned short temp = 0;
	unsigned long i = 0;
	
	Text_Mode();
	Font_Select_UserDefine_Mode();
	Foreground_color_65k(FontColor);
	Background_color_65k(BackGroundColor);
	Goto_Text_XY(x,y);
	
	while(c[i] != '\0')
	{ 
		if(c[i] < 0xa1)
		{
			CGROM_Select_Internal_CGROM();   // 内部CGROM为字符来源
			LCD_CmdWrite(0x04);
			LCD_DataWrite(c[i]);
			Check_Mem_WR_FIFO_not_Full();  
			i += 1;
		}
		else
		{
			Font_Select_UserDefine_Mode();   // 自定义字库
			LCD_CmdWrite(0x04);
			temp_H = ((c[i] - 0xa1) & 0x00ff) * 94;
			temp_L = c[i+1] - 0xa1;
			temp = temp_H + temp_L + 0x8000;
			LCD_DataWrite((temp>>8)&0xff);
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(temp&0xff);
			Check_Mem_WR_FIFO_not_Full();
			i += 2;		
		}
	}
	
  Check_2D_Busy();

  Graphic_Mode(); //back to graphic mode;图形模式
}

/*显示48*48、72*72字体*/
void LT738_BTE_Memory_Copy_ColorExpansion_8
(
 unsigned long S0_Addr             // SO图像的内存起始地址
,unsigned short XS0                // S0图像的左上方Y坐标
,unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
,unsigned short XDes               // 目的图像的左上方X坐标(显示窗口的起始x坐标)
,unsigned short YDes               // 目的图像的左上方Y坐标(显示窗口的起始y坐标)
,unsigned short X_W                // 显示窗口的宽度
,unsigned short Y_H                // 显示窗口的长度
,unsigned long Foreground_color
,unsigned long Background_color
)
{
	Foreground_color_256(Foreground_color);
	Background_color_256(Background_color);
	BTE_ROP_Code(7);
	
	BTE_S0_Color_8bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(Des_W);
	BTE_S0_Window_Start_XY(XS0,0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
   
	BTE_Operation_Code(0x0e);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}

void LT738_BTE_Memory_Copy_ColorExpansion_Chroma_key_8
(
 unsigned long S0_Addr             // SO图像的内存起始地址
,unsigned short XS0                // S0图像的左上方Y坐标
,unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
,unsigned short XDes               // 目的图像的左上方X坐标(显示窗口的起始x坐标)
,unsigned short YDes               // 目的图像的左上方Y坐标(显示窗口的起始y坐标)
,unsigned short X_W                // 显示窗口的宽度
,unsigned short Y_H                // 显示窗口的长度
,unsigned long Foreground_color
)
{
	Foreground_color_256(Foreground_color);
	BTE_ROP_Code(7);
	
	BTE_S0_Color_8bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(Des_W);
	BTE_S0_Window_Start_XY(XS0,0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
   
	BTE_Operation_Code(0x0f);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}

u8 FONTBUFF1[10] = {0};

#if 0

void LT738_Print_OutsideFont_Giant
(
 unsigned char SCS           		// 选择外挂的SPI   : SCS：0       SCS：1
,unsigned char Clk           		// SPI时钟分频参数 : SPI Clock = System Clock /{(Clk+1)*2}
,unsigned long FlashAddr     		// 字库源地址(Flash)
,unsigned long MemoryAddr    		// 目的地址(SDRAM)
,unsigned long ShowAddr             // 显示层的地址
,unsigned short width               // 显示层的宽度
,unsigned char Size          		// 设置字体大小  48：48*48     72:72*72
,unsigned char ChromaKey     		// 0：字体背景色透明    1：可以设置字体的背景色
,unsigned short x                   // 字体开始显示的x位置
,unsigned short y                   // 字体开始显示的y位置
,unsigned long FontColor            // 字体的颜色
,unsigned long BackGroundColor      // 字体的背景色（注意：当字体背景初始化成透明时，设置该值无效）
,unsigned short w				// 字体粗细：0：不加粗  1：加粗1级  2：加粗2级
,unsigned short s                   // 行距
,unsigned char *c                   // 数据缓冲的首地址
)
{
	unsigned short temp_H = 0;
	unsigned short temp_L = 0;
	unsigned short temp = 0;
	unsigned long i = 0;
	unsigned short j = 0;
	unsigned short k = 0;
	unsigned short h = 0;
	unsigned short n = 0;
	unsigned short m = 0;
	unsigned short g = 0;
	unsigned short f = 0;
	unsigned short a = 0;
	unsigned short b = 0;
	
	h = x; k = y;
	Memory_8bpp_Mode();//使用8位色深来存储图片
 	Canvas_Image_Start_address(MemoryAddr);
 	Canvas_image_width(width);
	while(c[i] != '\0')
	{
		temp_H = (c[i] - 0xa1) * 94;
		temp_L = c[i+1] - 0xa1;
		temp = temp_H + temp_L;
		for(a=0;a<Size;a++)//一共写a行
		{
			W25X_Read_Data(FONTBUFF1,FlashAddr+temp*((Size*Size)/8)+a*(Size/8),Size/8);//读一行的数据
			Goto_Pixel_XY(Size*j/8,a);//指向一行的起始位置
			LCD_CmdWrite(0x04);
			for(b = 0 ; b < Size/8 ; b++)
			{
				LCD_DataWrite(FONTBUFF1[b]);
			}
		}
		Check_Mem_WR_FIFO_Empty();
		i+=2;
		j++;
	}
	
	Memory_16bpp_Mode();
	Canvas_Image_Start_address(ShowAddr);
	Canvas_image_width(width);
	j = 0; i = 0;
	
	if(w>2)	w = 2;
	for(g=0;g<w+1;g++)
	{
		while(c[i] != '\0')
		{
			if((f == m)&&((x+Size*j+Size)>(width*(n+1)))) {m++;n++;y=y+Size-1+s;x = x+((width*n)-(x+Size*j))+g;f=n;}
			if(ChromaKey==1)
			{
				LT738_BTE_Memory_Copy_ColorExpansion_8(MemoryAddr,Size*j/8,
										   ShowAddr,width,x+Size*j,y,
										   Size,Size,FontColor,BackGroundColor
										   );
			}
			if(ChromaKey==0)
			{
				LT738_BTE_Memory_Copy_ColorExpansion_Chroma_key_8(MemoryAddr,Size*j/8,   
												  ShowAddr,width,x+Size*j,y,
													Size,Size,FontColor
													);
			}
			i+=2;
			j++;
		}
		ChromaKey=0;i=0;j=0;m=0;n=0;f=0;x=h+g+1;y=k+g+1;
	}
}

#endif 

void LT738_Text_cursor_Init
(
 unsigned char On_Off_Blinking         // 0：禁止光标闪烁   1：使能光标闪烁
,unsigned short Blinking_Time          // 设置文字光标闪烁时间
,unsigned short X_W                    // 文字光标水平大小
,unsigned short Y_W                    // 文字光标垂直大小
)
{
	if(On_Off_Blinking == 0)	Disable_Text_Cursor_Blinking();
	if(On_Off_Blinking == 1)	Enable_Text_Cursor_Blinking();

	Blinking_Time_Frames(Blinking_Time); 

	Text_Cursor_H_V(X_W,Y_W);

	Enable_Text_Cursor();
}


void LT738_Enable_Text_Cursor(void)
{
	Enable_Text_Cursor();
}


void LT738_Disable_Text_Cursor(void)
{
	Disable_Text_Cursor();
}


void LT738_Graphic_cursor_Init
(
 unsigned char Cursor_N                  // 选择光标   1:光标1   2:光标2   3:光标3  4:光标4
,unsigned char Color1                    // 颜色1
,unsigned char Color2                    // 颜色2
,unsigned short X_Pos                    // 显示坐标X
,unsigned short Y_Pos                    // 显示坐标Y
,unsigned char *Cursor_Buf               // 光标数据的缓冲首地址
)
{
	unsigned int i ;
	
	Memory_Select_Graphic_Cursor_RAM(); 
	Graphic_Mode();
	
	switch(Cursor_N)
	{
		case 1:	Select_Graphic_Cursor_1();	break;
		case 2:	Select_Graphic_Cursor_2();	break;
		case 3:	Select_Graphic_Cursor_3();	break;
		case 4:	Select_Graphic_Cursor_4();	break;
		default:break;
	}
	
	LCD_CmdWrite(0x04);
	for(i=0;i<256;i++)
	{					 
		LCD_DataWrite(Cursor_Buf[i]);
	}
	
	Memory_Select_SDRAM();//写完后切回SDRAM
	Set_Graphic_Cursor_Color_1(Color1);
	Set_Graphic_Cursor_Color_2(Color2);
	Graphic_Cursor_XY(X_Pos,Y_Pos);

	Enable_Graphic_Cursor();
}


void LT738_Set_Graphic_cursor_Pos
(
 unsigned char Cursor_N                  // 选择光标   1:光标1   2:光标2   3:光标3  4:光标4
,unsigned short X_Pos                    // 显示坐标X
,unsigned short Y_Pos                    // 显示坐标Y
)
{
	Graphic_Cursor_XY(X_Pos,Y_Pos);
	switch(Cursor_N)
	{
		case 1:	Select_Graphic_Cursor_1();	break;
		case 2:	Select_Graphic_Cursor_2();	break;
		case 3:	Select_Graphic_Cursor_3();	break;
		case 4:	Select_Graphic_Cursor_4();	break;
		default:
		break;
	}
}


void LT738_Enable_Graphic_Cursor(void)
{
	Enable_Graphic_Cursor();
}


void LT738_Disable_Graphic_Cursor(void)
{
	Disable_Graphic_Cursor();
}


//-----------------------------------------------------------------------------------------------------------------------------

void LT738_PIP_Init
(
 unsigned char On_Off         // 0 : 禁止 PIP    1 : 使能 PIP    2 : 保持原来的状态
,unsigned char Select_PIP     // 1 : 使用 PIP1   2 : 使用 PIP2
,unsigned long PAddr          // PIP的开始地址
,unsigned short XP            // PIP窗口的X坐标,必须被4整除
,unsigned short YP            // PIP窗口的Y坐标,必须被4整除
,unsigned long ImageWidth     // 底图的宽度
,unsigned short X_Dis         // 显示窗口的X坐标
,unsigned short Y_Dis         // 显示窗口的Y坐标
,unsigned short X_W           // 显示窗口的宽度，必须被4整除
,unsigned short Y_H           // 显示窗口的长度，必须被4整除
)
{
	if(Select_PIP == 1 )  
	{
		Select_PIP1_Window_16bpp();
		Select_PIP1_Parameter();
	}
	if(Select_PIP == 2 )  
	{
		Select_PIP2_Window_16bpp();
		Select_PIP2_Parameter();
	}
	
	PIP_Display_Start_XY(X_Dis,Y_Dis);
	PIP_Image_Start_Address(PAddr);
	PIP_Image_Width(ImageWidth);
	PIP_Window_Image_Start_XY(XP,YP);
	PIP_Window_Width_Height(X_W,Y_H);
	

	if(On_Off == 0)
	{
		if(Select_PIP == 1 )  Disable_PIP1();	
		if(Select_PIP == 2 )  Disable_PIP2();
	}

	if(On_Off == 1)
	{
		if(Select_PIP == 1 )  Enable_PIP1();	
		if(Select_PIP == 2 )  Enable_PIP2();
	}
}


void LT738_Set_DisWindowPos
(
 unsigned char On_Off         // 0 : 禁止 PIP, 1 : 使能 PIP, 2 : 保持原来的状态
,unsigned char Select_PIP     // 1 : 使用 PIP1 , 2 : 使用 PIP2
,unsigned short X_Dis         // 显示窗口的X坐标
,unsigned short Y_Dis         // 显示窗口的Y坐标
)
{
	if(Select_PIP == 1 )  Select_PIP1_Parameter();
	if(Select_PIP == 2 )  Select_PIP2_Parameter();
	
	if(On_Off == 0)
	{
		if(Select_PIP == 1 )  Disable_PIP1();	
		if(Select_PIP == 2 )  Disable_PIP2();
	}

	if(On_Off == 1)
	{
		if(Select_PIP == 1 )  Enable_PIP1();	
		if(Select_PIP == 2 )  Enable_PIP2();
	}
	
	PIP_Display_Start_XY(X_Dis,Y_Dis);
	
}




//-----------------------------------------------------------------------------------------------------------------------------



void BTE_Solid_Fill
(
 unsigned long Des_Addr           // 填充的目的地址 
,unsigned short Des_W             // 目的地址图片宽度
,unsigned short XDes              // x坐标 
,unsigned short YDes              // y坐标 
,unsigned short color             // 填充的颜色 
,unsigned short X_W               // 填充的长度 
,unsigned short Y_H               // 填充的宽度 
)            
{
	BTE_Destination_Color_16bpp();

	BTE_Destination_Memory_Start_Address(Des_Addr);

	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);
	BTE_Window_Size(X_W,Y_H);

	Foreground_color_65k(color);
	BTE_Operation_Code(0x0c);
	BTE_Enable();
	Check_BTE_Busy();     
}

/*  结合光栅操作的BTE内存复制 */
void LT738_BTE_Memory_Copy
(
 unsigned long S0_Addr     // SO图像的内存起始地址
,unsigned short S0_W       // S0图像的宽度
,unsigned short XS0        // S0图像的左上方X坐标
,unsigned short YS0        // S0图像的左上方Y坐标
,unsigned long S1_Addr     // S1图像的内存起始地址
,unsigned short S1_W       // S1图像的宽度
,unsigned short XS1        // S1图像的左上方X坐标
,unsigned short YS1        // S1图像的左上方Y坐标
,unsigned long Des_Addr    // 目的图像的内存起始地址
,unsigned short Des_W      // 目的图像的宽度
,unsigned short XDes       // 目的图像的左上方X坐标
,unsigned short YDes       // 目的图像的左上方Y坐标
,unsigned int ROP_Code     // 光栅操作模式
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W       // 活动窗口的宽度
,unsigned short Y_H       // 活动窗口的长度
)
{
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x02); //BTE Operation: Memory copy (move) with ROP.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}


/*  结合 Chroma Key 的内存复制（不含 ROP） */
void LT738_BTE_Memory_Copy_Chroma_key
(
 unsigned long S0_Addr             // SO图像的内存起始地址
,unsigned short S0_W               // S0图像的宽度
,unsigned short XS0                // S0图像的左上方X坐标
,unsigned short YS0                // S0图像的左上方Y坐标
,unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
,unsigned short XDes               // 目的图像的左上方X坐标
,unsigned short YDes               // 目的图像的左上方X坐标
,unsigned long Background_color    // 透明色
,unsigned short X_W                // 活动窗口的宽度
,unsigned short Y_H                // 活动窗口的长度
)
{
	Background_color_65k(Background_color); 

	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);	

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Operation_Code(0x05);	//BTE Operation: Memory copy (move) with chroma keying (w/o ROP)
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}


void LT738_BTE_Pattern_Fill
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // SO图像的内存起始地址
,unsigned short S0_W               // S0图像的宽度
,unsigned short XS0                // S0图像的左上方X坐标
,unsigned short YS0                // S0图像的左上方Y坐标
,unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
, unsigned short XDes              // 目的图像的左上方X坐标
,unsigned short YDes               // 目的图像的左上方X坐标
,unsigned int ROP_Code             // 光栅操作模式
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W                // 活动窗口的宽度
,unsigned short Y_H                // 活动窗口的长度
)
{
	if(P_8x8_or_16x16 == 0)
	{
		Pattern_Format_8X8();
	}
	if(P_8x8_or_16x16 == 1)
	{														    
		Pattern_Format_16X16();
	}	
	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x06); //BTE Operation: Pattern Fill with ROP.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}



void LT738_BTE_Pattern_Fill_With_Chroma_key
(
 unsigned char P_8x8_or_16x16      // 0 : use 8x8 Icon , 1 : use 16x16 Icon.
,unsigned long S0_Addr             // SO图像的内存起始地址
,unsigned short S0_W               // S0图像的宽度
,unsigned short XS0                // S0图像的左上方X坐标
,unsigned short YS0                // S0图像的左上方Y坐标
,unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
,unsigned short XDes               // 目的图像的左上方X坐标
,unsigned short YDes               // 目的图像的左上方Y坐标
,unsigned int ROP_Code             // 光栅操作模式
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned long Background_color   // 透明色
,unsigned short X_W               // 活动窗口的宽度
,unsigned short Y_H               // 活动窗口的长度
)
{
	Background_color_65k(Background_color);
	
	if(P_8x8_or_16x16 == 0)
	{
		Pattern_Format_8X8();
	}
	if(P_8x8_or_16x16 == 1)
	{														    
		Pattern_Format_16X16();
	}	  

	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);	

	BTE_ROP_Code(ROP_Code);	
	BTE_Operation_Code(0x07); //BTE Operation: Pattern Fill with Chroma key.
	BTE_Window_Size(X_W,Y_H); 
	BTE_Enable();
	Check_BTE_Busy();
}



void LT738_BTE_MCU_Write_MCU_16bit
(
 unsigned long S1_Addr              // S1图像的内存起始地址
,unsigned short S1_W                // S1图像的宽度
,unsigned short XS1                 // S1图像的左上方X坐标
,unsigned short YS1                 // S1图像的左上方Y坐标
,unsigned long Des_Addr             // 目的图像的内存起始地址
,unsigned short Des_W               // 目的图像的宽度
,unsigned short XDes                // 目的图像的左上方X坐标
,unsigned short YDes                // 目的图像的左上方Y坐标
,unsigned int ROP_Code              // 光栅操作模式 
/*ROP_Code :
   0000b		0(Blackness)
   0001b		~S0!E~S1 or ~(S0+S1)
   0010b		~S0!ES1
   0011b		~S0
   0100b		S0!E~S1
   0101b		~S1
   0110b		S0^S1
   0111b		~S0 + ~S1 or ~(S0 + S1)
   1000b		S0!ES1
   1001b		~(S0^S1)
   1010b		S1
   1011b		~S0+S1
   1100b		S0
   1101b		S0+~S1
   1110b		S0+S1
   1111b		1(whiteness)*/
,unsigned short X_W                 // 活动窗口的宽度
,unsigned short Y_H                 // 活动窗口的长度
,const unsigned short *data         // S0的数据首地址
)
{
	unsigned short i,j;

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_ROP_Code(ROP_Code);
	BTE_Operation_Code(0x00);		//BTE Operation: MPU Write with ROP.
	BTE_Enable();
	
	BTE_S0_Color_16bpp();
	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port
	
	for(i=0;i< Y_H;i++)
	{
		for(j=0;j< (X_W);j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel((*data));
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}



void LT738_BTE_MCU_Write_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr                 // 目的图像的内存起始地址
,unsigned short Des_W                   // 目的图像的宽度
,unsigned short XDes                    // 目的图像的左上方X坐标
,unsigned short YDes                    // 目的图像的左上方Y坐标
,unsigned long Background_color         // 透明色
,unsigned short X_W                     // 活动窗口的宽度
,unsigned short Y_H                     // 活动窗口的长度
,const unsigned short *data             // S0的数据收地址
)
{
	unsigned int i,j;
	
	Background_color_65k(Background_color);
	
	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x04);		//BTE Operation: MPU Write with chroma keying (w/o ROP)
	BTE_Enable();

	BTE_S0_Color_16bpp();
	LCD_CmdWrite(0x04);			//Memory Data Read/Write Port
	
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< (X_W);j++)
	  {
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel((*data));
			data++;
	  }
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}


/* 结合扩展色彩的 MPU 寫入 */
void LT738_BTE_MCU_Write_ColorExpansion_MCU_16bit
(
 unsigned long Des_Addr               // 目的图像的内存起始地址
,unsigned short Des_W                 // 目的图像的宽度
,unsigned short XDes                  // 目的图像的左上方X坐标
,unsigned short YDes                  // 目的图像的左上方Y坐标
,unsigned short X_W                   // 活动窗口的宽度
,unsigned short Y_H                   // 活动窗口的长度
,unsigned long Foreground_color       // 前景色
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,unsigned long Background_color       // 背景色
/*Background_color : The source (1bit map picture) map data 0 translate to Background color by color expansion*/
,const unsigned short *data           // 数据缓存首地址
)
{
	unsigned short i,j;
	
	RGB_16b_16bpp();
	Foreground_color_65k(Foreground_color);
	Background_color_65k(Background_color);
	BTE_ROP_Code(15);
	
	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x8);		//BTE Operation: MPU Write with Color Expansion (w/o ROP)
	BTE_Enable();
	
	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port  
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< X_W/16;j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(*data);  
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}

/* 结合扩展色彩与 Chroma key 的 MPU 寫入 */
void LT738_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit
(
 unsigned long Des_Addr            // 目的图像的内存起始地址
,unsigned short Des_W              // 目的图像的宽度
,unsigned short XDes               // 目的图像的左上方X坐标
,unsigned short YDes               // 目的图像的左上方Y坐标
,unsigned short X_W                // 活动窗口的宽度
,unsigned short Y_H                // 活动窗口的长度
,unsigned long Foreground_color    // 前景色
/*Foreground_color : The source (1bit map picture) map data 1 translate to Foreground color by color expansion*/
,const unsigned short *data        // 数据缓存首地址
)
{
	unsigned short i,j;

	RGB_16b_16bpp();
	Foreground_color_65k(Foreground_color);
	BTE_ROP_Code(15);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);


	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x9);		//BTE Operation: MPU Write with Color Expansion and chroma keying (w/o ROP)
	BTE_Enable();

	LCD_CmdWrite(0x04);				 		//Memory Data Read/Write Port  
	for(i=0;i< Y_H;i++)
	{	
		for(j=0;j< X_W/16;j++)
		{
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(*data);  
			data++;
		}
	}
	Check_Mem_WR_FIFO_Empty();
	Check_BTE_Busy();
}

/* 结合透明度的内存复制 */
void BTE_Alpha_Blending
(
 unsigned long S0_Addr         // SO图像的内存起始地址
 ,unsigned short S0_W          // S0图像的宽度
 ,unsigned short XS0           // S0图像的左上方X坐标
 ,unsigned short YS0           // S0图像的左上方Y坐标
 ,unsigned long S1_Addr        // S1图像的内存起始地址
 ,unsigned short S1_W          // S1图像的宽度
 ,unsigned short XS1           // S1图像的左上方X坐标
 ,unsigned short YS1           // S1图像的左上方Y坐标
 ,unsigned long Des_Addr       // 目的图像的内存起始地址
 ,unsigned short Des_W         // 目的图像的宽度
 ,unsigned short XDes          // 目的图像的左上方X坐标
 ,unsigned short YDes          // 目的图像的左上方X坐标
 ,unsigned short X_W           // 活动窗口的宽度
 ,unsigned short Y_H           // 活动窗口的长度
 ,unsigned char alpha          // 透明度等级（32等级）
)
{	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bpp();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x0A);		//BTE Operation: Memory write with opacity (w/o ROP)
	BTE_Alpha_Blending_Effect(alpha);
	BTE_Enable();
	Check_BTE_Busy();
}


void BTE_PNG
(
 unsigned long S0_Addr         // 底图的内存起始地址
 ,unsigned short S0_W          // 底图的宽度
 ,unsigned short XS0           // 底图的左上方X坐标
 ,unsigned short YS0           // 底图的左上方Y坐标
 ,unsigned long S1_Addr        // png图像的内存起始地址
 ,unsigned short S1_W          // png图像的宽度
 ,unsigned short XS1           // png图像的左上方X坐标
 ,unsigned short YS1           // png图像的左上方Y坐标
 ,unsigned long Des_Addr       // 显示的内存起始地址
 ,unsigned short Des_W         // 显示的宽度
 ,unsigned short XDes          // 显示的左上方X坐标
 ,unsigned short YDes          // 显示的左上方X坐标
 ,unsigned short X_W           // 显示的宽度
 ,unsigned short Y_H           // 显示的长度
)
{	
	BTE_S0_Color_16bpp();
	BTE_S0_Memory_Start_Address(S0_Addr);
	BTE_S0_Image_Width(S0_W);
	BTE_S0_Window_Start_XY(XS0,YS0);

	BTE_S1_Color_16bit_Alpha();
	BTE_S1_Memory_Start_Address(S1_Addr);
	BTE_S1_Image_Width(S1_W); 
	BTE_S1_Window_Start_XY(XS1,YS1);

	BTE_Destination_Color_16bpp();
	BTE_Destination_Memory_Start_Address(Des_Addr);
	BTE_Destination_Image_Width(Des_W);
	BTE_Destination_Window_Start_XY(XDes,YDes);

	BTE_Window_Size(X_W,Y_H);
	BTE_Operation_Code(0x0A);		//BTE Operation: Memory write with opacity (w/o ROP)
	BTE_Enable();
	Check_BTE_Busy();
}

//----------------------------------------------------------------------------------------------------------------------------------

void LT738_PWM0_Init
(
 unsigned char on_off                       // 0：禁止PWM0    1：使能PWM0
,unsigned char Clock_Divided                // PWM时钟分频  取值范围 0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // 时钟分频     取值范围 1~256
,unsigned short Count_Buffer                // 设置PWM的输出周期
,unsigned short Compare_Buffer              // 设置占空比
)
{
	Select_PWM0();
	Set_PWM_Prescaler_1_to_256(Prescalar);

	if(Clock_Divided ==0)	Select_PWM0_Clock_Divided_By_1();
	if(Clock_Divided ==1)	Select_PWM0_Clock_Divided_By_2();
	if(Clock_Divided ==2)	Select_PWM0_Clock_Divided_By_4();
	if(Clock_Divided ==3) Select_PWM0_Clock_Divided_By_8();

	Set_Timer0_Count_Buffer(Count_Buffer);  
	Set_Timer0_Compare_Buffer(Compare_Buffer);	

	if (on_off == 1)	Start_PWM0(); 
	if (on_off == 0)	Stop_PWM0();
}


void LT738_PWM0_Duty(unsigned short Compare_Buffer)
{
	Set_Timer0_Compare_Buffer(Compare_Buffer);
}



void LT738_PWM1_Init
(
 unsigned char on_off                       // 0：禁止PWM0    1：使能PWM0
,unsigned char Clock_Divided                // PWM时钟分频  取值范围 0~3(1,1/2,1/4,1/8)
,unsigned char Prescalar                    // 时钟分频     取值范围 1~256
,unsigned short Count_Buffer                // 设置PWM的输出周期
,unsigned short Compare_Buffer              // 设置占空比
)
{
	Select_PWM1();
	Set_PWM_Prescaler_1_to_256(Prescalar);
 
	if(Clock_Divided ==0)	Select_PWM1_Clock_Divided_By_1();
	if(Clock_Divided ==1)	Select_PWM1_Clock_Divided_By_2();
	if(Clock_Divided ==2)	Select_PWM1_Clock_Divided_By_4();
	if(Clock_Divided ==3)	Select_PWM1_Clock_Divided_By_8();

	Set_Timer1_Count_Buffer(Count_Buffer); 
	Set_Timer1_Compare_Buffer(Compare_Buffer); 

	if (on_off == 1)	Start_PWM1(); 
	if (on_off == 0)	Stop_PWM1();
}


void LT738_PWM1_Duty(unsigned short Compare_Buffer)
{
	Set_Timer1_Compare_Buffer(Compare_Buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------

// LT738进入待命模式
void LT738_Standby(void)
{
	Power_Saving_Standby_Mode();
	Check_Power_is_Saving();
}
// 从待命模式中唤醒
void LT738_Wkup_Standby(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
}


// LT738进入暂停模式
void LT738_Suspend(void)
{
	LT738_SDRAM_initail(10);
	Power_Saving_Suspend_Mode();
	Check_Power_is_Saving();
}
// 从暂停模式中唤醒
void LT738_Wkup_Suspend(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
	LT738_SDRAM_initail(MCLK);
}


// LT738进入休眠模式
void LT738_SleepMode(void)
{
	Power_Saving_Sleep_Mode();
	Check_Power_is_Saving();
}
// 从休眠模式中唤醒
void LT738_Wkup_Sleep(void)
{
	Power_Normal_Mode();
	Check_Power_is_Normal();
}

//-------------------------------------------------------------------------------------------------------------------------------
void MPU_Memory_Write_Window_8bit//通过设置工作窗口的方法描点画图
(
 unsigned short x            // x坐标
,unsigned short y            // y坐标
,unsigned short w            // 宽度
,unsigned short h            // 高度
,const unsigned char *data   // 数据(8位)
)
{
	unsigned short i,j,temp;
	Graphic_Mode();
    Active_Window_XY(x,y);
	Active_Window_WH(w,h);
	Goto_Pixel_XY(x,y);
	LCD_CmdWrite(0x04);
	for(j=0;j< h;j++)
	{
		for(i=0;i< w;i++)
		{
			 //if((i%32)==0)	Check_Mem_WR_FIFO_not_Full();
			 temp = *data;
			 data++;
			 temp+=(*data<<8);
			 LCD_DataWrite_Pixel(temp);
			 data++;
		}
	}
	//Check_Mem_WR_FIFO_Empty();
	Active_Window_XY(0,0);
	Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);
}

void MPU_Memory_Write_Pixel_8bit//通过改变坐标的方法描点画图
(
 unsigned short x            // x坐标
,unsigned short y            // y坐标
,unsigned short w            // 宽度
,unsigned short h            // 高度
,const unsigned char *data   // 数据(8位)
)
{
	unsigned short i,j,temp;
	Graphic_Mode();
	for(j=0;j< h;j++)
	{
		for(i=0;i< w;i++)
		{
			 Goto_Pixel_XY(x+i,y+j);//设定坐标
			 LCD_CmdWrite(0x04);//写SDRAM寄存器
			 temp = *data;
			 data++;
			 temp+=(*data<<8);
			 LCD_DataWrite_Pixel(temp);//写16位数据
			 data++;
		}
	}
	//Check_Mem_WR_FIFO_Empty();
}

#if 0
u8 BUFF[LCD_XSIZE_TFT*2] = {0};

//MCU读取SPI flash描点画图
//x,y:坐标  w,h:宽度和高度  addr:flash地址
void Memory_Write_Flash(u16 x,u16 y,u16 w,u16 h,u32 addr)
{
	u16 i,j,color;
	Graphic_Mode();
	for(j = 0 ; j < h ; j++)
	{
		W25X_Read_Data(BUFF,addr,w*2);//MCU读取图片一行的数据
		Goto_Pixel_XY(x,y+j);
		LCD_CmdWrite(0x04);
		for(i = 0 ; i < w ; i++)//写入一行数据
		{
			color = BUFF[i*2+1];
			color = ((color<<8)&0xff00)+BUFF[i*2];
			//if((i%32)==0)	Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite_Pixel(color);
		}
		addr+=w*2;//指向下一行
	}
	//Check_Mem_WR_FIFO_Empty();
}

#endif 


//-------------------------------------------------------------------------------------------------------------------------------



