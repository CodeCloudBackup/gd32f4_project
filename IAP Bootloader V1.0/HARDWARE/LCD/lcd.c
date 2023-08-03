#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "if_port.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6核心板
//4.3寸/5.0寸/7.0寸/ TFT液晶驱动
//版本：V1.0
//////////////////////////////////////////////////////////////////////////////////

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
  
//管理LCD参数
//默认为竖屏
_lcd_dev lcddev;
	 
//开始写内存
void LCD_WriteRAM_Prepare(void)
{
  	LCD_CmdWrite(0x04);  
}	 

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
	while(i--);
}

//读取某个点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x,u16 y)
{
	u16  a1;
	LCD_CmdWrite(0x5F);
	LCD_DataWrite(x);  
	LCD_CmdWrite(0x60);	   
	LCD_DataWrite(x>>8);
	LCD_CmdWrite(0x61);
	LCD_DataWrite(y);
	LCD_CmdWrite(0x62);	   
	LCD_DataWrite(y>>8);
	LCD_CmdWrite(0x04);
	
	LCD_DataRead();
	a1 = LCD_DataRead();
	return a1;

}			 

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	
	LCD_CmdWrite(0x5F);
	LCD_DataWrite(Xpos);  
  LCD_CmdWrite(0x60);	   
	LCD_DataWrite(Xpos>>8);
  LCD_CmdWrite(0x61);
	LCD_DataWrite(Ypos);
  LCD_CmdWrite(0x62);	   
	LCD_DataWrite(Ypos>>8);	
}


//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_CmdWrite(0x5F);
	LCD_DataWrite(x);  
	LCD_CmdWrite(0x60);	   
	LCD_DataWrite(x>>8);
	LCD_CmdWrite(0x61);
	LCD_DataWrite(y);
	LCD_CmdWrite(0x62);	   
	LCD_DataWrite(y>>8);
	LCD_CmdWrite(0x04);
	
	LCD_DataWrite_Pixel(POINT_COLOR);
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_CmdWrite(0x5F);
	LCD_DataWrite(x);  
	LCD_CmdWrite(0x60);	   
	LCD_DataWrite(x>>8);
	LCD_CmdWrite(0x61);
	LCD_DataWrite(y);
	LCD_CmdWrite(0x62);	   
	LCD_DataWrite(y>>8);
	LCD_CmdWrite(0x04);
	
	LCD_DataWrite_Pixel(color);
}


//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).
//sx,sy:窗口起始坐标(左上角)
//width,height:窗口宽度和高度,必须大于0!!
//窗体大小:width*height.
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	Active_Window_XY(sx,sy);							//设定工作窗口的坐标
	Active_Window_WH(width,height);	//设定工作窗口的宽度（必须为4的倍数）和高度
	LCD_SetCursor(sx, sy); 
}

//显示模块初始化
void LCD_Init(void)
{ 	 
	Select_Main_Window_16bpp();						//选择显示颜色深度
	Main_Image_Start_Address(layer1_start_addr);	//设定主窗口起始地址（SDRAM中显示到屏幕的内容的起始地址）
	Main_Image_Width(LCD_XSIZE_TFT);				//设定主窗口的宽度
	Main_Window_Start_XY(0,0);						//设定主窗口显示的起始坐标
	Canvas_Image_Start_address(layer1_start_addr);	//设定写入到SDRAM的起始地址
	Canvas_image_width(LCD_XSIZE_TFT);				//设定写入SDRAM的宽度（必须为4的倍数）
  Active_Window_XY(0,0);							//设定工作窗口的坐标
	Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);	//设定工作窗口的宽度（必须为4的倍数）和高度			 
 	delay_ms(50);
#if TFT800480
     lcddev.width = 800;
     lcddev.height = 480;
#endif
#if TFT1024600
     lcddev.width = 1024;
     lcddev.height = 600;
#endif
	LCD_Clear(WHITE);
}  
//清屏函数
//color:要清屏的填充色
void LCD_Clear(u16 color)
{
	  LT738_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,color);	
}

//在指定区域内填充指定颜色
//区域大小:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	  LT738_DrawSquare_Fill(sx, sy, ex, ey, color);

}

//在指定区域内填充指定颜色块
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 height, width;
    u16 i, j;
    width = ex - sx + 1;            //得到填充的宽度
    height = ey - sy + 1;           //高度

    for (i = 0; i < height; i++)
    {
        LCD_SetCursor(sx, sy + i);  //设置光标位置
        LCD_CmdWrite(0x04);         //开始写入内存
        for (j = 0; j < width; j++)
        {
					LCD_DataWrite_Pixel(color[i * width + j]);//写入数据
        }
    }
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
		LT738_DrawLine(x1, y1, x2, y2, POINT_COLOR);
}

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LT738_DrawSquare(x1, y1, x2, y2, POINT_COLOR);

}
//在指定位置画一个指定大小的圆
//(x0,y0):中心点
//r    :半径
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	LT738_DrawCircle(x0, y0, r, POINT_COLOR); 
}


//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  		
    char temp[1]={0};
		temp[0]=num;
	  LT738_Select_Internal_Font_Init(size, 1, 1, mode, 1); 
    LT738_Print_Internal_Font_String(x, y, POINT_COLOR, BACK_COLOR,temp );		  	   	 	  
}   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
		u8 t, temp;
    u8 enshow = 0;
   char string[10]={0};
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

       if (enshow == 0 && t < (len - 1))
        {
           if (temp == 0)
           {
               LT738_Select_Internal_Font_Init(size, 1, 1, 0, 1); 
                LT738_Print_Internal_Font_String(x+(size / 2)*t, y, POINT_COLOR, BACK_COLOR," " );
              continue;
           }
           else enshow = 1;
       }
				string[t]=temp+0x30;		
       LT738_Select_Internal_Font_Init(size, 1, 1, 0, 1); 
			 LT738_Print_Internal_Font_String(x+ (size / 2)*t , y, POINT_COLOR, BACK_COLOR,string+t);
    }
}

//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode)
{
		u8 t, temp;
    u8 enshow = 0;
    char string[10]={0};
    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                if (mode & 0X80)
								{	LT738_Select_Internal_Font_Init(size, 1, 1, mode & 0X01, 1); 
                  LT738_Print_Internal_Font_String(x+ (size / 2)*t, y, POINT_COLOR, BACK_COLOR,"0" );}
                else 
										{	LT738_Select_Internal_Font_Init(size, 1, 1, mode & 0X01, 1); 
                  LT738_Print_Internal_Font_String(x+ (size / 2)*t, y, POINT_COLOR, BACK_COLOR," " );}
                continue;
            }
            else enshow = 1;
        }
         string[t]=temp+0x30;			
       LT738_Select_Internal_Font_Init(size, 1, 1, mode & 0X01, 1); 
			 LT738_Print_Internal_Font_String(x+ (size / 2)*t , y, POINT_COLOR, BACK_COLOR,string+t);
    }
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
		LT738_Select_Internal_Font_Init(size, 1, 1, 0, 1); 
    LT738_Print_Internal_Font_String(x, y, POINT_COLOR, BACK_COLOR,p ); 
}






























