#include "usart.h"
#include "LT738_Demo.h"


#if 1
void Display_picture(
 unsigned short x			// x坐标
,unsigned short y			// y坐标
,unsigned short w			// 宽度 注意：必须为4的倍数
,unsigned short h			// 高度
,const unsigned char *bmp	// 图片数据
)	
{
	  u16  i,j,color;
	  u8 a,b;
		int p;
	
	Graphic_Mode();
  Active_Window_XY(x,y);
	Active_Window_WH(w,h);
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i< h;i++)
	{
		for(j=0;j< w;j++)
		{  

      a=(bmp[p]);
			b=(bmp[p+1]);
			color=((a<<8)|b);			
			if((j%32)==0)Check_Mem_WR_FIFO_not_Full();			
			LCD_DataWrite_Pixel(color);
			p=p+2;
	
			

		}
	}
	Check_Mem_WR_FIFO_Empty();
	Active_Window_XY(0,0);
	Active_Window_WH(LCD_XSIZE_TFT,LCD_YSIZE_TFT);
}

#endif



//灰阶
void GRAY_32(void)
{
    int i;
//LT738_DrawSquare_Fill(0,0,LCD_XSIZE_TFT,LCD_YSIZE_TFT,White);
for(i=0;i<32;i++)
LT738_DrawSquare_Fill(0,(LCD_YSIZE_TFT/24)*i,(LCD_XSIZE_TFT-1),(i+1)*(LCD_YSIZE_TFT/24),(i<<11)|(i<<6)|(i<<0));

}


//棋盘格
void WB_block(void)
{ int i,n,v,h;
  int x=LCD_XSIZE_TFT/8,y=LCD_YSIZE_TFT/8;
  h=LCD_XSIZE_TFT/x;
  v=LCD_YSIZE_TFT/y;
  for(n=0;n<v;n++)
  { if(n%2==0)
      { for(i=0;i<h;i++)
        { if(i%2==0)
          { LT738_DrawSquare_Fill(i*x,n*y,(i+1)*x-1,(n+1)*y,White); }
        else { LT738_DrawSquare_Fill(i*x,n*y,(i+1)*x-1,(n+1)*y,Black);
        }
      }
   }
  else
    { for(i=0;i<h;i++)
     { if(i%2==0)
       {
    	 LT738_DrawSquare_Fill(i*x,n*y,(i+1)*x-1,(n+1)*y,Black);
       }
       else
       {
    	  LT738_DrawSquare_Fill(i*x,n*y,(i+1)*x-1,(n+1)*y,White);
       }
     }
    }
  }
}








