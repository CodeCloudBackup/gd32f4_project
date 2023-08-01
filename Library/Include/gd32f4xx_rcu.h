/**
  ******************************************************************************
  * @file    stm32f4xx_RCU.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   This file contains all the functions prototypes for the RCU firmware library.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_RCU_H
#define __STM32F4xx_RCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup RCU
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
  uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
  uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
}RCU_ClocksTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup RCU_Exported_Constants
  * @{
  */
  
/** @defgroup RCU_HSE_configuration 
  * @{
  */
#define RCU_HSE_OFF                      ((uint8_t)0x00)
#define RCU_HSE_ON                       ((uint8_t)0x01)
#define RCU_HSE_Bypass                   ((uint8_t)0x05)
#define IS_RCU_HSE(HSE) (((HSE) == RCU_HSE_OFF) || ((HSE) == RCU_HSE_ON) || \
                         ((HSE) == RCU_HSE_Bypass))
/**
  * @}
  */ 

/** @defgroup RCU_LSE_Dual_Mode_Selection
  * @{
  */
#define RCU_LSE_LOWPOWER_MODE           ((uint8_t)0x00)
#define RCU_LSE_HIGHDRIVE_MODE          ((uint8_t)0x01)
#define IS_RCU_LSE_MODE(MODE)           (((MODE) == RCU_LSE_LOWPOWER_MODE) || \
                                         ((MODE) == RCU_LSE_HIGHDRIVE_MODE))
/**
  * @}
  */

/** @defgroup RCU_PLL_Clock_Source 
  * @{
  */
#define RCU_PLLSource_HSI                ((uint32_t)0x00000000)
#define RCU_PLLSource_HSE                ((uint32_t)0x00400000)
#define IS_RCU_PLL_SOURCE(SOURCE) (((SOURCE) == RCU_PLLSource_HSI) || \
                                   ((SOURCE) == RCU_PLLSource_HSE))
#define IS_RCU_PLLM_VALUE(VALUE) ((VALUE) <= 63)
#define IS_RCU_PLLN_VALUE(VALUE) ((192 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCU_PLLP_VALUE(VALUE) (((VALUE) == 2) || ((VALUE) == 4) || ((VALUE) == 6) || ((VALUE) == 8))
#define IS_RCU_PLLQ_VALUE(VALUE) ((4 <= (VALUE)) && ((VALUE) <= 15))
 
#define IS_RCU_PLLI2SN_VALUE(VALUE) ((192 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCU_PLLI2SR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))  
#define IS_RCU_PLLI2SM_VALUE(VALUE) ((VALUE) <= 63)  

#define IS_RCU_PLLI2SQ_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 15))
#define IS_RCU_PLLSAIN_VALUE(VALUE) ((192 <= (VALUE)) && ((VALUE) <= 432))
#define IS_RCU_PLLSAIQ_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 15))
#define IS_RCU_PLLSAIR_VALUE(VALUE) ((2 <= (VALUE)) && ((VALUE) <= 7))  

#define IS_RCU_PLLSAI_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))
#define IS_RCU_PLLI2S_DIVQ_VALUE(VALUE) ((1 <= (VALUE)) && ((VALUE) <= 32))

#define RCU_PLLSAIDivR_Div2                ((uint32_t)0x00000000)
#define RCU_PLLSAIDivR_Div4                ((uint32_t)0x00010000)
#define RCU_PLLSAIDivR_Div8                ((uint32_t)0x00020000)
#define RCU_PLLSAIDivR_Div16               ((uint32_t)0x00030000)
#define IS_RCU_PLLSAI_DIVR_VALUE(VALUE) (((VALUE) == RCU_PLLSAIDivR_Div2) ||\
                                        ((VALUE) == RCU_PLLSAIDivR_Div4)  ||\
                                        ((VALUE) == RCU_PLLSAIDivR_Div8)  ||\
                                        ((VALUE) == RCU_PLLSAIDivR_Div16))
 
/**
  * @}
  */ 
  
/** @defgroup RCU_System_Clock_Source 
  * @{
  */
#define RCU_SYSCLKSource_HSI             ((uint32_t)0x00000000)
#define RCU_SYSCLKSource_HSE             ((uint32_t)0x00000001)
#define RCU_SYSCLKSource_PLLCLK          ((uint32_t)0x00000002)
#define IS_RCU_SYSCLK_SOURCE(SOURCE) (((SOURCE) == RCU_SYSCLKSource_HSI) || \
                                      ((SOURCE) == RCU_SYSCLKSource_HSE) || \
                                      ((SOURCE) == RCU_SYSCLKSource_PLLCLK))
/**
  * @}
  */ 
  
/** @defgroup RCU_AHB_Clock_Source
  * @{
  */
#define RCU_SYSCLK_Div1                  ((uint32_t)0x00000000)
#define RCU_SYSCLK_Div2                  ((uint32_t)0x00000080)
#define RCU_SYSCLK_Div4                  ((uint32_t)0x00000090)
#define RCU_SYSCLK_Div8                  ((uint32_t)0x000000A0)
#define RCU_SYSCLK_Div16                 ((uint32_t)0x000000B0)
#define RCU_SYSCLK_Div64                 ((uint32_t)0x000000C0)
#define RCU_SYSCLK_Div128                ((uint32_t)0x000000D0)
#define RCU_SYSCLK_Div256                ((uint32_t)0x000000E0)
#define RCU_SYSCLK_Div512                ((uint32_t)0x000000F0)
#define IS_RCU_HCLK(HCLK) (((HCLK) == RCU_SYSCLK_Div1) || ((HCLK) == RCU_SYSCLK_Div2) || \
                           ((HCLK) == RCU_SYSCLK_Div4) || ((HCLK) == RCU_SYSCLK_Div8) || \
                           ((HCLK) == RCU_SYSCLK_Div16) || ((HCLK) == RCU_SYSCLK_Div64) || \
                           ((HCLK) == RCU_SYSCLK_Div128) || ((HCLK) == RCU_SYSCLK_Div256) || \
                           ((HCLK) == RCU_SYSCLK_Div512))
/**
  * @}
  */ 
  
/** @defgroup RCU_APB1_APB2_Clock_Source
  * @{
  */
#define RCU_HCLK_Div1                    ((uint32_t)0x00000000)
#define RCU_HCLK_Div2                    ((uint32_t)0x00001000)
#define RCU_HCLK_Div4                    ((uint32_t)0x00001400)
#define RCU_HCLK_Div8                    ((uint32_t)0x00001800)
#define RCU_HCLK_Div16                   ((uint32_t)0x00001C00)
#define IS_RCU_PCLK(PCLK) (((PCLK) == RCU_HCLK_Div1) || ((PCLK) == RCU_HCLK_Div2) || \
                           ((PCLK) == RCU_HCLK_Div4) || ((PCLK) == RCU_HCLK_Div8) || \
                           ((PCLK) == RCU_HCLK_Div16))
/**
  * @}
  */ 
  
/** @defgroup RCU_Interrupt_Source 
  * @{
  */
#define RCU_IT_LSIRDY                    ((uint8_t)0x01)
#define RCU_IT_LSERDY                    ((uint8_t)0x02)
#define RCU_IT_HSIRDY                    ((uint8_t)0x04)
#define RCU_IT_HSERDY                    ((uint8_t)0x08)
#define RCU_IT_PLLRDY                    ((uint8_t)0x10)
#define RCU_IT_PLLI2SRDY                 ((uint8_t)0x20) 
#define RCU_IT_PLLSAIRDY                 ((uint8_t)0x40)
#define RCU_IT_CSS                       ((uint8_t)0x80)

#define IS_RCU_IT(IT) ((((IT) & (uint8_t)0x80) == 0x00) && ((IT) != 0x00))
#define IS_RCU_GET_IT(IT) (((IT) == RCU_IT_LSIRDY) || ((IT) == RCU_IT_LSERDY) || \
                           ((IT) == RCU_IT_HSIRDY) || ((IT) == RCU_IT_HSERDY) || \
                           ((IT) == RCU_IT_PLLRDY) || ((IT) == RCU_IT_CSS) || \
                           ((IT) == RCU_IT_PLLSAIRDY) || ((IT) == RCU_IT_PLLI2SRDY))
#define IS_RCU_CLEAR_IT(IT)((IT) != 0x00)

/**
  * @}
  */ 
  
/** @defgroup RCU_LSE_Configuration 
  * @{
  */
#define RCU_LSE_OFF                      ((uint8_t)0x00)
#define RCU_LSE_ON                       ((uint8_t)0x01)
#define RCU_LSE_Bypass                   ((uint8_t)0x04)
#define IS_RCU_LSE(LSE) (((LSE) == RCU_LSE_OFF) || ((LSE) == RCU_LSE_ON) || \
                         ((LSE) == RCU_LSE_Bypass))
/**
  * @}
  */ 
  
/** @defgroup RCU_RTC_Clock_Source
  * @{
  */
#define RCU_RTCCLKSource_LSE             ((uint32_t)0x00000100)
#define RCU_RTCCLKSource_LSI             ((uint32_t)0x00000200)
#define RCU_RTCCLKSource_HSE_Div2        ((uint32_t)0x00020300)
#define RCU_RTCCLKSource_HSE_Div3        ((uint32_t)0x00030300)
#define RCU_RTCCLKSource_HSE_Div4        ((uint32_t)0x00040300)
#define RCU_RTCCLKSource_HSE_Div5        ((uint32_t)0x00050300)
#define RCU_RTCCLKSource_HSE_Div6        ((uint32_t)0x00060300)
#define RCU_RTCCLKSource_HSE_Div7        ((uint32_t)0x00070300)
#define RCU_RTCCLKSource_HSE_Div8        ((uint32_t)0x00080300)
#define RCU_RTCCLKSource_HSE_Div9        ((uint32_t)0x00090300)
#define RCU_RTCCLKSource_HSE_Div10       ((uint32_t)0x000A0300)
#define RCU_RTCCLKSource_HSE_Div11       ((uint32_t)0x000B0300)
#define RCU_RTCCLKSource_HSE_Div12       ((uint32_t)0x000C0300)
#define RCU_RTCCLKSource_HSE_Div13       ((uint32_t)0x000D0300)
#define RCU_RTCCLKSource_HSE_Div14       ((uint32_t)0x000E0300)
#define RCU_RTCCLKSource_HSE_Div15       ((uint32_t)0x000F0300)
#define RCU_RTCCLKSource_HSE_Div16       ((uint32_t)0x00100300)
#define RCU_RTCCLKSource_HSE_Div17       ((uint32_t)0x00110300)
#define RCU_RTCCLKSource_HSE_Div18       ((uint32_t)0x00120300)
#define RCU_RTCCLKSource_HSE_Div19       ((uint32_t)0x00130300)
#define RCU_RTCCLKSource_HSE_Div20       ((uint32_t)0x00140300)
#define RCU_RTCCLKSource_HSE_Div21       ((uint32_t)0x00150300)
#define RCU_RTCCLKSource_HSE_Div22       ((uint32_t)0x00160300)
#define RCU_RTCCLKSource_HSE_Div23       ((uint32_t)0x00170300)
#define RCU_RTCCLKSource_HSE_Div24       ((uint32_t)0x00180300)
#define RCU_RTCCLKSource_HSE_Div25       ((uint32_t)0x00190300)
#define RCU_RTCCLKSource_HSE_Div26       ((uint32_t)0x001A0300)
#define RCU_RTCCLKSource_HSE_Div27       ((uint32_t)0x001B0300)
#define RCU_RTCCLKSource_HSE_Div28       ((uint32_t)0x001C0300)
#define RCU_RTCCLKSource_HSE_Div29       ((uint32_t)0x001D0300)
#define RCU_RTCCLKSource_HSE_Div30       ((uint32_t)0x001E0300)
#define RCU_RTCCLKSource_HSE_Div31       ((uint32_t)0x001F0300)
#define IS_RCU_RTCCLK_SOURCE(SOURCE) (((SOURCE) == RCU_RTCCLKSource_LSE) || \
                                      ((SOURCE) == RCU_RTCCLKSource_LSI) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div2) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div3) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div4) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div5) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div6) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div7) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div8) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div9) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div10) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div11) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div12) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div13) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div14) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div15) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div16) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div17) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div18) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div19) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div20) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div21) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div22) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div23) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div24) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div25) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div26) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div27) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div28) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div29) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div30) || \
                                      ((SOURCE) == RCU_RTCCLKSource_HSE_Div31))
/**
  * @}
  */ 
  
/** @defgroup RCU_I2S_Clock_Source
  * @{
  */
#define RCU_I2S2CLKSource_PLLI2S             ((uint8_t)0x00)
#define RCU_I2S2CLKSource_Ext                ((uint8_t)0x01)

#define IS_RCU_I2SCLK_SOURCE(SOURCE) (((SOURCE) == RCU_I2S2CLKSource_PLLI2S) || ((SOURCE) == RCU_I2S2CLKSource_Ext))                                
/**
  * @}
  */ 

/** @defgroup RCU_SAI_BlockA_Clock_Source
  * @{
  */
#define RCU_SAIACLKSource_PLLSAI             ((uint32_t)0x00000000)
#define RCU_SAIACLKSource_PLLI2S             ((uint32_t)0x00100000)
#define RCU_SAIACLKSource_Ext                ((uint32_t)0x00200000)

#define IS_RCU_SAIACLK_SOURCE(SOURCE) (((SOURCE) == RCU_SAIACLKSource_PLLI2S) ||\
                                       ((SOURCE) == RCU_SAIACLKSource_PLLSAI) ||\
                                       ((SOURCE) == RCU_SAIACLKSource_Ext))
/**
  * @}
  */ 

/** @defgroup RCU_SAI_BlockB_Clock_Source
  * @{
  */
#define RCU_SAIBCLKSource_PLLSAI             ((uint32_t)0x00000000)
#define RCU_SAIBCLKSource_PLLI2S             ((uint32_t)0x00400000)
#define RCU_SAIBCLKSource_Ext                ((uint32_t)0x00800000)

#define IS_RCU_SAIBCLK_SOURCE(SOURCE) (((SOURCE) == RCU_SAIBCLKSource_PLLI2S) ||\
                                       ((SOURCE) == RCU_SAIBCLKSource_PLLSAI) ||\
                                       ((SOURCE) == RCU_SAIBCLKSource_Ext))
/**
  * @}
  */ 

/** @defgroup RCU_TIM_PRescaler_Selection
  * @{
  */
#define RCU_TIMPrescDesactivated             ((uint8_t)0x00)
#define RCU_TIMPrescActivated                ((uint8_t)0x01)

#define IS_RCU_TIMCLK_PRESCALER(VALUE) (((VALUE) == RCU_TIMPrescDesactivated) || ((VALUE) == RCU_TIMPrescActivated))
/**
  * @}
  */
  
/** @defgroup RCU_AHB1_Peripherals 
  * @{
  */ 
#define RCU_AHB1Periph_GPIOA             ((uint32_t)0x00000001)
#define RCU_AHB1Periph_GPIOB             ((uint32_t)0x00000002)
#define RCU_AHB1Periph_GPIOC             ((uint32_t)0x00000004)
#define RCU_AHB1Periph_GPIOD             ((uint32_t)0x00000008)
#define RCU_AHB1Periph_GPIOE             ((uint32_t)0x00000010)
#define RCU_AHB1Periph_GPIOF             ((uint32_t)0x00000020)
#define RCU_AHB1Periph_GPIOG             ((uint32_t)0x00000040)
#define RCU_AHB1Periph_GPIOH             ((uint32_t)0x00000080)
#define RCU_AHB1Periph_GPIOI             ((uint32_t)0x00000100) 
#define RCU_AHB1Periph_CRC               ((uint32_t)0x00001000)
#define RCU_AHB1Periph_FLITF             ((uint32_t)0x00008000)
#define RCU_AHB1Periph_SRAM0             ((uint32_t)0x00010000)
#define RCU_AHB1Periph_SRAM1             ((uint32_t)0x00020000)
#define RCU_AHB1Periph_BKPSRAM           ((uint32_t)0x00040000)
#define RCU_AHB1Periph_SRAM2             ((uint32_t)0x00080000)
#define RCU_AHB1Periph_CCMDATARAMEN      ((uint32_t)0x00100000)
#define RCU_AHB1Periph_DMA0              ((uint32_t)0x00200000)
#define RCU_AHB1Periph_DMA1              ((uint32_t)0x00400000)
#define RCU_AHB1Periph_DMA2D             ((uint32_t)0x00800000)
#define RCU_AHB1Periph_ETH_MAC           ((uint32_t)0x02000000)
#define RCU_AHB1Periph_ETH_MAC_Tx        ((uint32_t)0x04000000)
#define RCU_AHB1Periph_ETH_MAC_Rx        ((uint32_t)0x08000000)
#define RCU_AHB1Periph_ETH_MAC_PTP       ((uint32_t)0x10000000)
#define RCU_AHB1Periph_OTG_HS            ((uint32_t)0x20000000)
#define RCU_AHB1Periph_OTG_HS_ULPI       ((uint32_t)0x40000000)

#define IS_RCU_AHB1_CLOCK_PERIPH(PERIPH) ((((PERIPH) & 0x810BE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCU_AHB1_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xDD1FE800) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCU_AHB1_LPMODE_PERIPH(PERIPH) ((((PERIPH) & 0x81106800) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 
  
/** @defgroup RCU_AHB2_Peripherals 
  * @{
  */  
#define RCU_AHB2Periph_DCMI              ((uint32_t)0x00000001)
#define RCU_AHB2Periph_CRYP              ((uint32_t)0x00000010)
#define RCU_AHB2Periph_HASH              ((uint32_t)0x00000020)
#define RCU_AHB2Periph_RNG               ((uint32_t)0x00000040)
#define RCU_AHB2Periph_OTG_FS            ((uint32_t)0x00000080)
#define IS_RCU_AHB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFF0E) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCU_AHB3_Peripherals 
  * @{
  */ 
#if defined (gd32F40_41xxx)
#define RCU_AHB3Periph_FSMC                ((uint32_t)0x00000001)
#endif /* STM32F40_41xxx */

#if defined (gd32F427) || defined (gd32F429_439xx) 
#define RCU_AHB3Periph_FMC                ((uint32_t)0x00000001)
#endif /* STM32F427_437xx ||  STM32F429_439xx */

#define IS_RCU_AHB3_PERIPH(PERIPH) ((((PERIPH) & 0xFFFFFFFE) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCU_APB1_Peripherals 
  * @{
  */ 
#define RCU_APB1Periph_TIM1              ((uint32_t)0x00000001)
#define RCU_APB1Periph_TIM2              ((uint32_t)0x00000002)
#define RCU_APB1Periph_TIM3              ((uint32_t)0x00000004)
#define RCU_APB1Periph_TIM4              ((uint32_t)0x00000008)
#define RCU_APB1Periph_TIM5              ((uint32_t)0x00000010)
#define RCU_APB1Periph_TIM6              ((uint32_t)0x00000020)
#define RCU_APB1Periph_TIM11             ((uint32_t)0x00000040)
#define RCU_APB1Periph_TIM12             ((uint32_t)0x00000080)
#define RCU_APB1Periph_TIM13             ((uint32_t)0x00000100)
#define RCU_APB1Periph_WWDG              ((uint32_t)0x00000800)
#define RCU_APB1Periph_SPI1              ((uint32_t)0x00004000)
#define RCU_APB1Periph_SPI2              ((uint32_t)0x00008000)
#define RCU_APB1Periph_USART1            ((uint32_t)0x00020000)
#define RCU_APB1Periph_USART2            ((uint32_t)0x00040000)
#define RCU_APB1Periph_UART3             ((uint32_t)0x00080000)
#define RCU_APB1Periph_UART4             ((uint32_t)0x00100000)
#define RCU_APB1Periph_I2C0              ((uint32_t)0x00200000)
#define RCU_APB1Periph_I2C1              ((uint32_t)0x00400000)
#define RCU_APB1Periph_I2C2              ((uint32_t)0x00800000)
#define RCU_APB1Periph_CAN0              ((uint32_t)0x02000000)
#define RCU_APB1Periph_CAN1              ((uint32_t)0x04000000)
#define RCU_APB1Periph_PWR               ((uint32_t)0x10000000)
#define RCU_APB1Periph_DAC               ((uint32_t)0x20000000)
#define RCU_APB1Periph_UART6             ((uint32_t)0x40000000)
#define RCU_APB1Periph_UART7             ((uint32_t)0x80000000)
#define IS_RCU_APB1_PERIPH(PERIPH) ((((PERIPH) & 0x09013600) == 0x00) && ((PERIPH) != 0x00))
/**
  * @}
  */ 
  
/** @defgroup RCU_APB2_Peripherals 
  * @{
  */ 
#define RCU_APB2Periph_TIM0              ((uint32_t)0x00000001)
#define RCU_APB2Periph_TIM7              ((uint32_t)0x00000002)
#define RCU_APB2Periph_USART0            ((uint32_t)0x00000010)
#define RCU_APB2Periph_USART5            ((uint32_t)0x00000020)
#define RCU_APB2Periph_ADC               ((uint32_t)0x00000100)
#define RCU_APB2Periph_ADC0              ((uint32_t)0x00000100)
#define RCU_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCU_APB2Periph_ADC2              ((uint32_t)0x00000400)
#define RCU_APB2Periph_SDIO              ((uint32_t)0x00000800)
#define RCU_APB2Periph_SPI0              ((uint32_t)0x00001000)
#define RCU_APB2Periph_SPI3              ((uint32_t)0x00002000)
#define RCU_APB2Periph_SYSCFG            ((uint32_t)0x00004000)
#define RCU_APB2Periph_TIM8              ((uint32_t)0x00010000)
#define RCU_APB2Periph_TIM9             ((uint32_t)0x00020000)
#define RCU_APB2Periph_TIM10             ((uint32_t)0x00040000)
#define RCU_APB2Periph_SPI4              ((uint32_t)0x00100000)
#define RCU_APB2Periph_SPI5              ((uint32_t)0x00200000)
#define RCU_APB2Periph_SAI1              ((uint32_t)0x00400000)
#define RCU_APB2Periph_LTDC              ((uint32_t)0x04000000)

#define IS_RCU_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFB8880CC) == 0x00) && ((PERIPH) != 0x00))
#define IS_RCU_APB2_RESET_PERIPH(PERIPH) ((((PERIPH) & 0xFB8886CC) == 0x00) && ((PERIPH) != 0x00))

/**
  * @}
  */ 
  
/** @defgroup RCU_MCO1_Clock_Source_Prescaler
  * @{
  */
#define RCU_MCO1Source_HSI               ((uint32_t)0x00000000)
#define RCU_MCO1Source_LSE               ((uint32_t)0x00200000)
#define RCU_MCO1Source_HSE               ((uint32_t)0x00400000)
#define RCU_MCO1Source_PLLCLK            ((uint32_t)0x00600000)
#define RCU_MCO1Div_1                    ((uint32_t)0x00000000)
#define RCU_MCO1Div_2                    ((uint32_t)0x04000000)
#define RCU_MCO1Div_3                    ((uint32_t)0x05000000)
#define RCU_MCO1Div_4                    ((uint32_t)0x06000000)
#define RCU_MCO1Div_5                    ((uint32_t)0x07000000)
#define IS_RCU_MCO1SOURCE(SOURCE) (((SOURCE) == RCU_MCO1Source_HSI) || ((SOURCE) == RCU_MCO1Source_LSE) || \
                                   ((SOURCE) == RCU_MCO1Source_HSE) || ((SOURCE) == RCU_MCO1Source_PLLCLK))
                                   
#define IS_RCU_MCO1DIV(DIV) (((DIV) == RCU_MCO1Div_1) || ((DIV) == RCU_MCO1Div_2) || \
                             ((DIV) == RCU_MCO1Div_3) || ((DIV) == RCU_MCO1Div_4) || \
                             ((DIV) == RCU_MCO1Div_5)) 
/**
  * @}
  */ 
  
/** @defgroup RCU_MCO2_Clock_Source_Prescaler
  * @{
  */
#define RCU_MCO2Source_SYSCLK            ((uint32_t)0x00000000)
#define RCU_MCO2Source_PLLI2SCLK         ((uint32_t)0x40000000)
#define RCU_MCO2Source_HSE               ((uint32_t)0x80000000)
#define RCU_MCO2Source_PLLCLK            ((uint32_t)0xC0000000)
#define RCU_MCO2Div_1                    ((uint32_t)0x00000000)
#define RCU_MCO2Div_2                    ((uint32_t)0x20000000)
#define RCU_MCO2Div_3                    ((uint32_t)0x28000000)
#define RCU_MCO2Div_4                    ((uint32_t)0x30000000)
#define RCU_MCO2Div_5                    ((uint32_t)0x38000000)
#define IS_RCU_MCO2SOURCE(SOURCE) (((SOURCE) == RCU_MCO2Source_SYSCLK) || ((SOURCE) == RCU_MCO2Source_PLLI2SCLK)|| \
                                   ((SOURCE) == RCU_MCO2Source_HSE) || ((SOURCE) == RCU_MCO2Source_PLLCLK))
                                   
#define IS_RCU_MCO2DIV(DIV) (((DIV) == RCU_MCO2Div_1) || ((DIV) == RCU_MCO2Div_2) || \
                             ((DIV) == RCU_MCO2Div_3) || ((DIV) == RCU_MCO2Div_4) || \
                             ((DIV) == RCU_MCO2Div_5))                             
/**
  * @}
  */ 
  
/** @defgroup RCU_Flag 
  * @{
  */
#define RCU_FLAG_HSIRDY                  ((uint8_t)0x21)  1
#define RCU_FLAG_HSERDY                  ((uint8_t)0x31)
#define RCU_FLAG_PLLRDY                  ((uint8_t)0x39)
#define RCU_FLAG_PLLI2SRDY               ((uint8_t)0x3B)
#define RCU_FLAG_PLLSAIRDY               ((uint8_t)0x3D)
#define RCU_FLAG_LSERDY                  ((uint8_t)0x41)
#define RCU_FLAG_LSIRDY                  ((uint8_t)0x61)
#define RCU_FLAG_BORRST                  ((uint8_t)0x79)
#define RCU_FLAG_PINRST                  ((uint8_t)0x7A)
#define RCU_FLAG_PORRST                  ((uint8_t)0x7B)
#define RCU_FLAG_SFTRST                  ((uint8_t)0x7C)
#define RCU_FLAG_IWDGRST                 ((uint8_t)0x7D)
#define RCU_FLAG_WWDGRST                 ((uint8_t)0x7E)
#define RCU_FLAG_LPWRRST                 ((uint8_t)0x7F)

#define IS_RCU_FLAG(FLAG) (((FLAG) == RCU_FLAG_HSIRDY)   || ((FLAG) == RCU_FLAG_HSERDY) || \
                           ((FLAG) == RCU_FLAG_PLLRDY)   || ((FLAG) == RCU_FLAG_LSERDY) || \
                           ((FLAG) == RCU_FLAG_LSIRDY)   || ((FLAG) == RCU_FLAG_BORRST) || \
                           ((FLAG) == RCU_FLAG_PINRST)   || ((FLAG) == RCU_FLAG_PORRST) || \
                           ((FLAG) == RCU_FLAG_SFTRST)   || ((FLAG) == RCU_FLAG_IWDGRST)|| \
                           ((FLAG) == RCU_FLAG_WWDGRST)  || ((FLAG) == RCU_FLAG_LPWRRST)|| \
                           ((FLAG) == RCU_FLAG_PLLI2SRDY)|| ((FLAG) == RCU_FLAG_PLLSAIRDY))

#define IS_RCU_CALIBRATION_VALUE(VALUE) ((VALUE) <= 0x1F)
/**
  * @}
  */ 
/* bits definitions */
/* RCU_CTL */
#define RCU_CTL_IRC16MEN                BIT(0)                    /*!< internal high speed oscillator enable */
#define RCU_CTL_IRC16MSTB               BIT(1)                    /*!< IRC16M high speed internal oscillator stabilization flag */
#define RCU_CTL_IRC16MADJ               BITS(3,7)                 /*!< high speed internal oscillator clock trim adjust value */
#define RCU_CTL_IRC16MCALIB             BITS(8,15)                /*!< high speed internal oscillator calibration value register */
#define RCU_CTL_HXTALEN                 BIT(16)                   /*!< external high speed oscillator enable */
#define RCU_CTL_HXTALSTB                BIT(17)                   /*!< external crystal oscillator clock stabilization flag */
#define RCU_CTL_HXTALBPS                BIT(18)                   /*!< external crystal oscillator clock bypass mode enable */
#define RCU_CTL_CKMEN                   BIT(19)                   /*!< HXTAL clock monitor enable */
#define RCU_CTL_PLLEN                   BIT(24)                   /*!< PLL enable */
#define RCU_CTL_PLLSTB                  BIT(25)                   /*!< PLL Clock Stabilization Flag */
#define RCU_CTL_PLLI2SEN                BIT(26)                   /*!< PLLI2S enable */
#define RCU_CTL_PLLI2SSTB               BIT(27)                   /*!< PLLI2S Clock Stabilization Flag */
#define RCU_CTL_PLLSAIEN                BIT(28)                   /*!< PLLSAI enable */
#define RCU_CTL_PLLSAISTB               BIT(29)                   /*!< PLLSAI Clock Stabilization Flag */

/* RCU_PLL */
#define RCU_PLL_PLLPSC                  BITS(0,5)                 /*!< The PLL VCO source clock prescaler */
#define RCU_PLL_PLLN                    BITS(6,14)                /*!< The PLL VCO clock multi factor */
#define RCU_PLL_PLLP                    BITS(16,17)               /*!< The PLLP output frequency division factor from PLL VCO clock */
#define RCU_PLL_PLLSEL                  BIT(22)                   /*!< PLL Clock Source Selection */
#define RCU_PLL_PLLQ                    BITS(24,27)               /*!< The PLL Q output frequency division factor from PLL VCO clock */

/* RCU_CFG0 */
#define RCU_CFG0_SCS                    BITS(0,1)                 /*!< system clock switch */
#define RCU_CFG0_SCSS                   BITS(2,3)                 /*!< system clock switch status */
#define RCU_CFG0_AHBPSC                 BITS(4,7)                 /*!< AHB prescaler selection */
#define RCU_CFG0_APB1PSC                BITS(10,12)               /*!< APB1 prescaler selection */
#define RCU_CFG0_APB2PSC                BITS(13,15)               /*!< APB2 prescaler selection */
#define RCU_CFG0_RTCDIV                 BITS(16,20)               /*!< RTC clock divider factor */
#define RCU_CFG0_CKOUT0SEL              BITS(21,22)               /*!< CKOUT0 Clock Source Selection */
#define RCU_CFG0_I2SSEL                 BIT(23)                   /*!< I2S Clock Source Selection */
#define RCU_CFG0_CKOUT0DIV              BITS(24,26)               /*!< The CK_OUT0 divider which the CK_OUT0 frequency can be reduced */
#define RCU_CFG0_CKOUT1DIV              BITS(27,29)               /*!< The CK_OUT1 divider which the CK_OUT1 frequency can be reduced */
#define RCU_CFG0_CKOUT1SEL              BITS(30,31)               /*!< CKOUT1 Clock Source Selection */
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 

/* Function used to set the RCU clock configuration to the default reset state */
void RCU_DeInit(void);

/* Internal/external clocks, PLL, CSS and MCO configuration functions *********/
void        RCU_HSEConfig(uint8_t RCU_HSE);
ErrStatus 	RCU_WaitForHSEStartUp(void);
void        RCU_AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
void        RCU_HSICmd(FunctionalState NewState);
void        RCU_LSEConfig(uint8_t RCU_LSE);
void        RCU_LSICmd(FunctionalState NewState);
void        RCU_PLLConfig(uint32_t RCU_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ);
void        RCU_PLLCmd(FunctionalState NewState);

#if defined (STM32F40_41xxx) || defined (STM32F401xx)
void        RCU_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR);
#elif defined (STM32F411xE)
void        RCU_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR, uint32_t PLLI2SM);
#elif defined (STM32F427_437xx) || defined (STM32F429_439xx)
void        RCU_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SQ, uint32_t PLLI2SR);
#else
#endif /* STM32F40_41xxx || STM32F401xx */

void        RCU_PLLI2SCmd(FunctionalState NewState);
void        RCU_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR);
void        RCU_PLLSAICmd(FunctionalState NewState);
void        RCU_ClockSecuritySystemCmd(FunctionalState NewState);
void        RCU_MCO1Config(uint32_t RCU_MCO1Source, uint32_t RCU_MCO1Div);
void        RCU_MCO2Config(uint32_t RCU_MCO2Source, uint32_t RCU_MCO2Div);

/* System, AHB and APB busses clocks configuration functions ******************/
void        RCU_SYSCLKConfig(uint32_t RCU_SYSCLKSource);
uint8_t     RCU_GetSYSCLKSource(void);
void        RCU_HCLKConfig(uint32_t RCU_SYSCLK);
void        RCU_PCLK1Config(uint32_t RCU_HCLK);
void        RCU_PCLK2Config(uint32_t RCU_HCLK);
void        RCU_GetClocksFreq(RCU_ClocksTypeDef* RCU_Clocks);

/* Peripheral clocks configuration functions **********************************/
void        RCU_RTCCLKConfig(uint32_t RCU_RTCCLKSource);
void        RCU_RTCCLKCmd(FunctionalState NewState);
void        RCU_BackupResetCmd(FunctionalState NewState);
void        RCU_I2SCLKConfig(uint32_t RCU_I2SCLKSource); 
void        RCU_SAIPLLI2SClkDivConfig(uint32_t RCU_PLLI2SDivQ);
void        RCU_SAIPLLSAIClkDivConfig(uint32_t RCU_PLLSAIDivQ);
void        RCU_SAIBlockACLKConfig(uint32_t RCU_SAIBlockACLKSource);
void        RCU_SAIBlockBCLKConfig(uint32_t RCU_SAIBlockBCLKSource);
void        RCU_LTDCCLKDivConfig(uint32_t RCU_PLLSAIDivR);
void        RCU_TIMCLKPresConfig(uint32_t RCU_TIMCLKPrescaler);

void        RCU_AHB1PeriphClockCmd(uint32_t RCU_AHB1Periph, FunctionalState NewState);
void        RCU_AHB2PeriphClockCmd(uint32_t RCU_AHB2Periph, FunctionalState NewState);
void        RCU_AHB3PeriphClockCmd(uint32_t RCU_AHB3Periph, FunctionalState NewState);
void        RCU_APB1PeriphClockCmd(uint32_t RCU_APB1Periph, FunctionalState NewState);
void        RCU_APB2PeriphClockCmd(uint32_t RCU_APB2Periph, FunctionalState NewState);

void        RCU_AHB1PeriphResetCmd(uint32_t RCU_AHB1Periph, FunctionalState NewState);
void        RCU_AHB2PeriphResetCmd(uint32_t RCU_AHB2Periph, FunctionalState NewState);
void        RCU_AHB3PeriphResetCmd(uint32_t RCU_AHB3Periph, FunctionalState NewState);
void        RCU_APB1PeriphResetCmd(uint32_t RCU_APB1Periph, FunctionalState NewState);
void        RCU_APB2PeriphResetCmd(uint32_t RCU_APB2Periph, FunctionalState NewState);

void        RCU_AHB1PeriphClockLPModeCmd(uint32_t RCU_AHB1Periph, FunctionalState NewState);
void        RCU_AHB2PeriphClockLPModeCmd(uint32_t RCU_AHB2Periph, FunctionalState NewState);
void        RCU_AHB3PeriphClockLPModeCmd(uint32_t RCU_AHB3Periph, FunctionalState NewState);
void        RCU_APB1PeriphClockLPModeCmd(uint32_t RCU_APB1Periph, FunctionalState NewState);
void        RCU_APB2PeriphClockLPModeCmd(uint32_t RCU_APB2Periph, FunctionalState NewState);

void        RCU_LSEModeConfig(uint8_t Mode);

/* Interrupts and flags management functions **********************************/
void        RCU_ITConfig(uint8_t RCU_IT, FunctionalState NewState);
FlagStatus  RCU_GetFlagStatus(uint8_t RCU_FLAG);
void        RCU_ClearFlag(void);
FlagStatus    RCU_GetITStatus(uint8_t RCU_IT);
void        RCU_ClearITPendingBit(uint8_t RCU_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_RCU_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
