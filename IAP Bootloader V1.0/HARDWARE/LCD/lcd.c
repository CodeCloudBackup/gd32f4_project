#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"	 
#include "delay.h"	 
#include "if_port.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//WKS GD32F427ZGT6���İ�
//4.3��/5.0��/7.0��/ TFTҺ������
//�汾��V1.0
//////////////////////////////////////////////////////////////////////////////////

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
  
//����LCD����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	 
//��ʼд�ڴ�
void LCD_WriteRAM_Prepare(void)
{
  	LCD_CmdWrite(0x04);  
}	 

//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}

//��ȡĳ�������ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
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

//���ù��λ��
//Xpos:������
//Ypos:������
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


//����
//x,y:����
//POINT_COLOR:�˵����ɫ
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

//���ٻ���
//x,y:����
//color:��ɫ
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


//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height.
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height)
{
	Active_Window_XY(sx,sy);							//�趨�������ڵ�����
	Active_Window_WH(width,height);	//�趨�������ڵĿ�ȣ�����Ϊ4�ı������͸߶�
	LCD_SetCursor(sx, sy); 
}

//��ʾģ���ʼ��
void LCD_Init(void)
{ 	 
	Select_Main_Window_16bpp();						//ѡ����ʾ��ɫ���
	Main_Image_Start_Address(layer1_start_addr);	//�趨��������ʼ��ַ��SDRAM����ʾ����Ļ�����ݵ���ʼ��ַ��
	Main_Image_Width(LCD_XSIZE_TFT);				//�趨�����ڵĿ��
	Main_Window_Start_XY(0,0);						//�趨��������ʾ����ʼ����
	Canvas_Image_Start_address(layer1_start_addr);	//�趨д�뵽SDRAM����ʼ��ַ
	Canvas_image_width(LCD_XSIZE_TFT);				//�趨д��SDRAM�Ŀ�ȣ�����Ϊ4�ı�����
  Active_Window_XY(0,0);							//�趨�������ڵ�����
	Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);	//�趨�������ڵĿ�ȣ�����Ϊ4�ı������͸߶�			 
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
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
	  LT738_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,color);	
}

//��ָ�����������ָ����ɫ
//�����С:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	  LT738_DrawSquare_Fill(sx, sy, ex, ey, color);

}

//��ָ�����������ָ����ɫ��
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color)
{
    u16 height, width;
    u16 i, j;
    width = ex - sx + 1;            //�õ����Ŀ��
    height = ey - sy + 1;           //�߶�

    for (i = 0; i < height; i++)
    {
        LCD_SetCursor(sx, sy + i);  //���ù��λ��
        LCD_CmdWrite(0x04);         //��ʼд���ڴ�
        for (j = 0; j < width; j++)
        {
					LCD_DataWrite_Pixel(color[i * width + j]);//д������
        }
    }
}

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
		LT738_DrawLine(x1, y1, x2, y2, POINT_COLOR);
}

//������
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LT738_DrawSquare(x1, y1, x2, y2, POINT_COLOR);

}
//��ָ��λ�û�һ��ָ����С��Բ
//(x0,y0):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	LT738_DrawCircle(x0, y0, r, POINT_COLOR); 
}


//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  		
    char temp[1]={0};
		temp[0]=num;
	  LT738_Select_Internal_Font_Init(size, 1, 1, mode, 1); 
    LT738_Print_Internal_Font_String(x, y, POINT_COLOR, BACK_COLOR,temp );		  	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);
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

//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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

//��ʾ�ַ���
//x,y:�������
//width,height:�����С
//size:�����С
//*p:�ַ�����ʼ��ַ
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
		LT738_Select_Internal_Font_Init(size, 1, 1, 0, 1); 
    LT738_Print_Internal_Font_String(x, y, POINT_COLOR, BACK_COLOR,p ); 
}






























