#ifndef _LT738_Lib_H
#define _LT738_Lib_H
#include "LT738.h"
#include "if_port.h"


//外部晶振
#define XI_4M            0
#define XI_8M            0
#define XI_10M    	     1
#define XI_12M           0

//选择显示分辨率
#define TFT480272 	0		// 选择分辨率480×272
#define TFT800480 	1		// 选择分辨率800×480
#define TFT1024600 	0		// 选择分辨率1024×600
#define TFT1024768 	0		// 选择分辨率1024×768

#if TFT480272       //480*272分辨率的参数配置
#define LCD_XSIZE_TFT    480
#define LCD_YSIZE_TFT    272

#define LCD_VBPD		 12
#define LCD_VFPD	 	 8
#define LCD_VSPW		 4  //13
#define LCD_HBPD		 43
#define LCD_HFPD		 8
#define LCD_HSPW	   4  //48
#endif

#if TFT800480    //800*480分辨率的参数配置

#define LCD_XSIZE_TFT    800
#define LCD_YSIZE_TFT    480

#define LCD_VBPD		 32
#define LCD_VFPD	 	 3
#define LCD_VSPW		 10  
#define LCD_HBPD		 88
#define LCD_HFPD		 40
#define LCD_HSPW	   10  
#endif

#if TFT1024600      //1024*600分辨率的参数配置
#define LCD_XSIZE_TFT    1024
#define LCD_YSIZE_TFT    600

#define LCD_VBPD		 32
#define LCD_VFPD	 	 3
#define LCD_VSPW		 10  
#define LCD_HBPD		 88
#define LCD_HFPD		 40
#define LCD_HSPW	   10  
#endif

#if TFT1024768     //1024*768分辨率的参数配置
#define LCD_XSIZE_TFT    1024
#define LCD_YSIZE_TFT    768

#define LCD_VBPD		 23
#define LCD_VFPD	 	 15
#define LCD_VSPW		 10  
#define LCD_HBPD		 160
#define LCD_HFPD		 160
#define LCD_HSPW	   10  
#endif



#define color256_black   0x00
#define color256_white   0xff
#define color256_red     0xe0
#define color256_green   0x1c
#define color256_blue    0x03
#define color256_yellow  color256_red|color256_green
#define color256_cyan    color256_green|color256_blue
#define color256_purple  color256_red|color256_blue

#if 1
 
#define color65k_black   0x0000
#define color65k_white   0xffff
#define color65k_red     0xf800
#define color65k_green   0x07e0
#define color65k_blue    0x001f
#define color65k_yellow  color65k_red|color65k_green
#define color65k_cyan    color65k_green|color65k_blue
#define color65k_purple  color65k_red|color65k_blue

#define color65k_grayscale1    2113
#define color65k_grayscale2    2113*2
#define color65k_grayscale3    2113*3
#define color65k_grayscale4    2113*4
#define color65k_grayscale5    2113*5
#define color65k_grayscale6    2113*6
#define color65k_grayscale7    2113*7
#define color65k_grayscale8    2113*8
#define color65k_grayscale9    2113*9
#define color65k_grayscale10   2113*10
#define color65k_grayscale11   2113*11
#define color65k_grayscale12   2113*12
#define color65k_grayscale13   2113*13
#define color65k_grayscale14   2113*14
#define color65k_grayscale15   2113*15
#define color65k_grayscale16   2113*16
#define color65k_grayscale17   2113*17
#define color65k_grayscale18   2113*18
#define color65k_grayscale19   2113*19
#define color65k_grayscale20   2113*20
#define color65k_grayscale21   2113*21
#define color65k_grayscale22   2113*22
#define color65k_grayscale23   2113*23
#define color65k_grayscale24   2113*24
#define color65k_grayscale25   2113*25
#define color65k_grayscale26   2113*26
#define color65k_grayscale27   2113*27
#define color65k_grayscale28   2113*28
#define color65k_grayscale29   2113*29
#define color65k_grayscale30   2113*30

 
#define color16M_black   0x00000000
#define color16M_white   0x00ffffff
#define color16M_red     0x00ff0000
#define color16M_green   0x0000ff00
#define color16M_blue    0x000000ff
#define color16M_yellow  color16M_red|color16M_green
#define color16M_cyan    color16M_green|color16M_blue
#define color16M_purple  color16M_red|color16M_blue

#endif

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0x8410  //0xF7DE   
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


void LT738_Init(void);

/* 写数据到内存 */
void MPU8_8bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);
void MPU8_16bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);
void MPU8_24bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data); 
void MPU16_16bpp_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data);
void MPU16_24bpp_Mode1_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data); 
void MPU16_24bpp_Mode2_Memory_Write(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned short *data);

/* 硬件画线段 */
void LT738_DrawLine(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long LineColor);
void LT738_DrawLine_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long LineColor,unsigned short Width);

/* 硬件画圆 */
void LT738_DrawCircle(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long CircleColor);
void LT738_DrawCircle_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long ForegroundColor);
void LT738_DrawCircle_Width(unsigned short XCenter,unsigned short YCenter,unsigned short R,unsigned long CircleColor,unsigned long ForegroundColor,unsigned short Width);

/* 硬件画椭圆 */
void LT738_DrawEllipse(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long EllipseColor);
void LT738_DrawEllipse_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawEllipse_Width(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long EllipseColor,unsigned long ForegroundColor,unsigned short Width);   

/* 硬件画矩形 */
void LT738_DrawSquare(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long SquareColor);
void LT738_DrawSquare_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long ForegroundColor);
void LT738_DrawSquare_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned long SquareColor,unsigned long ForegroundColor,unsigned short Width);

/* 硬件画圆角矩形 */
void LT738_DrawCircleSquare(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long CircleSquareColor);
void LT738_DrawCircleSquare_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawCircleSquare_Width(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X_R,unsigned short Y_R,unsigned long CircleSquareColor,unsigned long ForegroundColor,unsigned short Width);

/* 硬件画三角形 */
void LT738_DrawTriangle(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor);
void LT738_DrawTriangle_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long ForegroundColor);
void LT738_DrawTriangle_Frame(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor,unsigned long ForegroundColor);

/* 硬件画四边形 */
void LT738_DrawQuadrilateral(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned long ForegroundColor);
void LT738_DrawQuadrilateral_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned long ForegroundColor);
void LT738_DrawTriangle_Frame(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned long TriangleColor ,unsigned long ForegroundColor);

/* 硬件画五边形 */
void LT738_DrawPentagon(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned long ForegroundColor);
void LT738_DrawPentagon_Fill(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned long ForegroundColor);

/* 硬件画曲线 */
void LT738_DrawLeftUpCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawLeftDownCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawRightUpCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_DrawRightDownCurve(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor);
void LT738_SelectDrawCurve(unsigned short XCenter ,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor,unsigned short  Dir);

/* 硬件画1/4椭圆 */
void LT738_DrawLeftUpCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawLeftDownCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawRightUpCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_DrawRightDownCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long ForegroundColor);
void LT738_SelectDrawCurve_Fill(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned long CurveColor,unsigned short  Dir);

/* 硬件画圆柱 */
unsigned char LT738_DrawCylinder(unsigned short XCenter,unsigned short YCenter,unsigned short X_R,unsigned short Y_R,unsigned short H,unsigned long CylinderColor,unsigned long ForegroundColor);

/* 硬件画四棱柱 */
void LT738_DrawQuadrangular(unsigned short X1,unsigned short Y1,unsigned short X2,unsigned short Y2,unsigned short X3,unsigned short Y3,unsigned short X4,unsigned short Y4,unsigned short X5,unsigned short Y5,unsigned short X6,unsigned short Y6,unsigned long QuadrangularColor,unsigned long ForegroundColor);

/* 表格 */
void LT738_MakeTable(unsigned short X1,unsigned short Y1,unsigned short W,unsigned short H,unsigned short Line,unsigned short Row,unsigned long  TableColor,unsigned long  ItemColor,unsigned long  ForegroundColor,unsigned short width1,unsigned short width2,unsigned char  mode);

/* 使用内建字库 */
void LT738_Select_Internal_Font_Init(unsigned char Size,unsigned char XxN,unsigned char YxN,unsigned char ChromaKey,unsigned char Alignment);
void LT738_Print_Internal_Font_String(unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor ,char *c);

/* 使用外建字库 */
void LT738_Select_Outside_Font_Init(unsigned char SCS,unsigned char Clk,unsigned long FlashAddr,unsigned long MemoryAddr,unsigned long DisplayAddr,unsigned long Num,unsigned char Size,unsigned char XxN,unsigned char YxN,unsigned char ChromaKey,unsigned char Alignment);
void LT738_Print_Outside_Font_String(unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor,unsigned char *c);
void LT738_Print_OutsideFont_Giant(unsigned char SCS,unsigned char Clk,unsigned long FlashAddr,unsigned long MemoryAddr,unsigned long ShowAddr,unsigned short width,unsigned char Size,unsigned char ChromaKey,unsigned short x,unsigned short y,unsigned long FontColor,unsigned long BackGroundColor,unsigned short w,unsigned short s,unsigned char *c);

/* 文字光标 */
void LT738_Text_cursor_Init(unsigned char On_Off_Blinking,unsigned short Blinking_Time,unsigned short X_W,unsigned short Y_W);
void LT738_Enable_Text_Cursor(void);
void LT738_Disable_Text_Cursor(void);

/*  图像光标 */
void LT738_Graphic_cursor_Init(unsigned char Cursor_N,unsigned char Color1,unsigned char Color2,unsigned short X_Pos,unsigned short Y_Pos,unsigned char *Cursor_Buf);
void LT738_Set_Graphic_cursor_Pos(unsigned char Cursor_N,unsigned short X_Pos,unsigned short Y_Pos);
void LT738_Enable_Graphic_Cursor(void);
void LT738_Disable_Graphic_Cursor(void);

/* 区块传输引擎（BitBLT） */
void BTE_Solid_Fill(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Memory_Copy(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long S1_Addr,unsigned short S1_W,unsigned short XS1,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Memory_Copy_Chroma_key(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned long Background_color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Pattern_Fill(unsigned char P_8x8_or_16x16,unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W, unsigned short XDes,unsigned short YDes,unsigned int ROP_Code ,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_Pattern_Fill_With_Chroma_key(unsigned char P_8x8_or_16x16,unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned long Background_color,unsigned short X_W,unsigned short Y_H);
void LT738_BTE_MCU_Write_MCU_16bit(unsigned long S1_Addr,unsigned short S1_W,unsigned short XS,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned int ROP_Code,unsigned short X_W,unsigned short Y_H ,const unsigned short *data);
void LT738_BTE_MCU_Write_Chroma_key_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned long Background_color,unsigned short X_W,unsigned short Y_H,const unsigned short *data);
void LT738_BTE_MCU_Write_ColorExpansion_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned long Foreground_color ,unsigned long Background_color ,const unsigned short *data);
void LT738_BTE_MCU_Write_ColorExpansion_Chroma_key_MCU_16bit(unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned long Foreground_color,const unsigned short *data);
void BTE_Alpha_Blending(unsigned long S0_Addr,unsigned short S0_W,unsigned short XS0,unsigned short YS0,unsigned long S1_Addr,unsigned short S1_W,unsigned short XS1,unsigned short YS1,unsigned long Des_Addr,unsigned short Des_W,unsigned short XDes,unsigned short YDes,unsigned short X_W,unsigned short Y_H,unsigned char alpha);

/*  PIP */
void LT738_PIP_Init(unsigned char On_Off,unsigned char Select_PIP,unsigned long PAddr,unsigned short XP,unsigned short YP,unsigned long ImageWidth,unsigned short X_Dis,unsigned short Y_Dis,unsigned short X_W,unsigned short Y_H);
void LT738_Set_DisWindowPos(unsigned char On_Off,unsigned char Select_PIP,unsigned short X_Dis,unsigned short Y_Dis);

/*  PWM */
void LT738_PWM0_Init(unsigned char on_off,unsigned char Clock_Divided,unsigned char Prescalar,unsigned short Count_Buffer,unsigned short Compare_Buffer);
void LT738_PWM1_Init(unsigned char on_off,unsigned char Clock_Divided,unsigned char Prescalar,unsigned short Count_Buffer,unsigned short Compare_Buffer);
void LT738_PWM0_Duty(unsigned short Compare_Buffer);
void LT738_PWM1_Duty(unsigned short Compare_Buffer);

/*  Standby Mode */
void LT738_Standby(void);
void LT738_Wkup_Standby(void);

/* Suspend Mode */
void LT738_Suspend(void);
void LT738_Wkup_Suspend(void);

/* Sleep Mode */
void LT738_SleepMode(void);
void LT738_Wkup_Sleep(void);

void MPU_Memory_Write_Window_8bit(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);//通过设置工作窗口的方法描点画图（宽度必须为4的倍数）
void MPU_Memory_Write_Pixel_8bit(unsigned short x,unsigned short y,unsigned short w,unsigned short h,const unsigned char *data);//通过改变坐标的方法描点画图
void Memory_Write_Flash(u16 x,u16 y,u16 w,u16 h,u32 addr);//MCU读取SPI flash描点画图


#endif

