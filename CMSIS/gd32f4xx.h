/*!
    \file    gd32f4xx.h
    \brief   general definitions for GD32F4xx
    
    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_H
#define GD32F4XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* define GD32F4xx */
#if !defined (GD32F450)  && !defined (GD32F405) && !defined (GD32F407)
  /* #define GD32F450 */
  /* #define GD32F405 */
  /* #define GD32F407 */
#endif /* define GD32F4xx */
   
#if !defined (GD32F450)  && !defined (GD32F405) && !defined (GD32F407)
 #error "Please select the target GD32F4xx device in gd32f4xx.h file"
#endif /* undefine GD32F4xx tip */

/* define value of high speed crystal oscillator (HXTAL) in Hz */
#if !defined  (HXTAL_VALUE)
#define HXTAL_VALUE    ((uint32_t)8000000)
#endif /* high speed crystal oscillator value */

/* define startup timeout value of high speed crystal oscillator (HXTAL) */
#if !defined  (HXTAL_STARTUP_TIMEOUT)
#define HXTAL_STARTUP_TIMEOUT   ((uint16_t)0xFFFF)
#endif /* high speed crystal oscillator startup timeout */

/* define value of internal 16MHz RC oscillator (IRC16M) in Hz */
#if !defined  (IRC16M_VALUE) 
#define IRC16M_VALUE  ((uint32_t)16000000)
#endif /* internal 16MHz RC oscillator value */

/* define startup timeout value of internal 16MHz RC oscillator (IRC16M) */
#if !defined  (IRC16M_STARTUP_TIMEOUT)
#define IRC16M_STARTUP_TIMEOUT   ((uint16_t)0x0500)
#endif /* internal 16MHz RC oscillator startup timeout */

/* define value of internal 32KHz RC oscillator(IRC32K) in Hz */
#if !defined  (IRC32K_VALUE) 
#define IRC32K_VALUE  ((uint32_t)32000)
#endif /* internal 32KHz RC oscillator value */

/* define value of low speed crystal oscillator (LXTAL)in Hz */
#if !defined  (LXTAL_VALUE) 
#define LXTAL_VALUE  ((uint32_t)32768)
#endif /* low speed crystal oscillator value */

/* I2S external clock in selection */
//#define I2S_EXTERNAL_CLOCK_IN          (uint32_t)12288000U

/* GD32F4xx firmware library version number V1.0 */
#define __GD32F4xx_STDPERIPH_VERSION_MAIN   (0x03) /*!< [31:24] main version     */
#define __GD32F4xx_STDPERIPH_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version     */
#define __GD32F4xx_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version     */
#define __GD32F4xx_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __GD32F4xx_STDPERIPH_VERSION        ((__GD32F4xx_STDPERIPH_VERSION_MAIN << 24)\
                                            |(__GD32F4xx_STDPERIPH_VERSION_SUB1 << 16)\
                                            |(__GD32F4xx_STDPERIPH_VERSION_SUB2 << 8)\
                                            |(__GD32F4xx_STDPERIPH_VERSION_RC))

/* configuration of the cortex-M4 processor and core peripherals */
#define __CM4_REV                 0x0001   /*!< core revision r0p1                                       */
#define __MPU_PRESENT             1        /*!< GD32F4xx provide MPU                                     */
#define __NVIC_PRIO_BITS          4        /*!< GD32F4xx uses 4 bits for the priority levels             */
#define __Vendor_SysTickConfig    0        /*!< set to 1 if different sysTick config is used             */
#define __FPU_PRESENT             1        /*!< FPU present                                              */
/* define interrupt number */
typedef enum IRQn
{
    /* cortex-M4 processor exceptions numbers */
    NonMaskableInt_IRQn          = -14,    /*!< 2 non maskable interrupt                                 */
    MemoryManagement_IRQn        = -12,    /*!< 4 cortex-M4 memory management interrupt                  */
    BusFault_IRQn                = -11,    /*!< 5 cortex-M4 bus fault interrupt                          */
    UsageFault_IRQn              = -10,    /*!< 6 cortex-M4 usage fault interrupt                        */
    SVCall_IRQn                  = -5,     /*!< 11 cortex-M4 SV call interrupt                           */
    DebugMonitor_IRQn            = -4,     /*!< 12 cortex-M4 debug monitor interrupt                     */
    PendSV_IRQn                  = -2,     /*!< 14 cortex-M4 pend SV interrupt                           */
    SysTick_IRQn                 = -1,     /*!< 15 cortex-M4 system tick interrupt                       */
    /* interruput numbers */
    WWDGT_IRQn                   = 0,      /*!< window watchdog timer interrupt                          */
    LVD_IRQn                     = 1,      /*!< LVD through EXTI line detect interrupt                   */
    TAMPER_STAMP_IRQn            = 2,      /*!< tamper and timestamp through EXTI line detect            */
    RTC_WKUP_IRQn                = 3,      /*!< RTC wakeup through EXTI line interrupt                   */
    FMC_IRQn                     = 4,      /*!< FMC interrupt                                            */
    RCU_CTC_IRQn                 = 5,      /*!< RCU and CTC interrupt                                    */
    EXTI0_IRQn                   = 6,      /*!< EXTI line 0 interrupts                                   */
    EXTI1_IRQn                   = 7,      /*!< EXTI line 1 interrupts                                   */
    EXTI2_IRQn                   = 8,      /*!< EXTI line 2 interrupts                                   */
    EXTI3_IRQn                   = 9,      /*!< EXTI line 3 interrupts                                   */
    EXTI4_IRQn                   = 10,     /*!< EXTI line 4 interrupts                                   */
    DMA0_Channel0_IRQn           = 11,     /*!< DMA0 channel0 Interrupt                                  */
    DMA0_Channel1_IRQn           = 12,     /*!< DMA0 channel1 Interrupt                                  */
    DMA0_Channel2_IRQn           = 13,     /*!< DMA0 channel2 interrupt                                  */
    DMA0_Channel3_IRQn           = 14,     /*!< DMA0 channel3 interrupt                                  */
    DMA0_Channel4_IRQn           = 15,     /*!< DMA0 channel4 interrupt                                  */
    DMA0_Channel5_IRQn           = 16,     /*!< DMA0 channel5 interrupt                                  */
    DMA0_Channel6_IRQn           = 17,     /*!< DMA0 channel6 interrupt                                  */
    ADC_IRQn                     = 18,     /*!< ADC interrupt                                            */
    CAN0_TX_IRQn                 = 19,     /*!< CAN0 TX interrupt                                        */
    CAN0_RX0_IRQn                = 20,     /*!< CAN0 RX0 interrupt                                       */
    CAN0_RX1_IRQn                = 21,     /*!< CAN0 RX1 interrupt                                       */
    CAN0_EWMC_IRQn               = 22,     /*!< CAN0 EWMC interrupt                                      */
    EXTI5_9_IRQn                 = 23,     /*!< EXTI[9:5] interrupts                                     */
    TIMER0_BRK_TIMER8_IRQn       = 24,     /*!< TIMER0 break and TIMER8 interrupts                       */
    TIMER0_UP_TIMER9_IRQn        = 25,     /*!< TIMER0 update and TIMER9 interrupts                      */
    TIMER0_TRG_CMT_TIMER10_IRQn  = 26,     /*!< TIMER0 trigger and commutation  and TIMER10 interrupts   */
    TIMER0_Channel_IRQn          = 27,     /*!< TIMER0 channel capture compare interrupt                 */
    TIMER1_IRQn                  = 28,     /*!< TIMER1 interrupt                                         */
    TIMER2_IRQn                  = 29,     /*!< TIMER2 interrupt                                         */
    TIMER3_IRQn                  = 30,     /*!< TIMER3 interrupts                                        */
    I2C0_EV_IRQn                 = 31,     /*!< I2C0 event interrupt                                     */
    I2C0_ER_IRQn                 = 32,     /*!< I2C0 error interrupt                                     */
    I2C1_EV_IRQn                 = 33,     /*!< I2C1 event interrupt                                     */
    I2C1_ER_IRQn                 = 34,     /*!< I2C1 error interrupt                                     */
    SPI0_IRQn                    = 35,     /*!< SPI0 interrupt                                           */
    SPI1_IRQn                    = 36,     /*!< SPI1 interrupt                                           */
    USART0_IRQn                  = 37,     /*!< USART0 interrupt                                         */
    USART1_IRQn                  = 38,     /*!< USART1 interrupt                                         */
    USART2_IRQn                  = 39,     /*!< USART2 interrupt                                         */
    EXTI10_15_IRQn               = 40,     /*!< EXTI[15:10] interrupts                                   */
    RTC_Alarm_IRQn               = 41,     /*!< RTC alarm interrupt                                      */
    USBFS_WKUP_IRQn              = 42,     /*!< USBFS wakeup interrupt                                   */
    TIMER7_BRK_TIMER11_IRQn      = 43,     /*!< TIMER7 break and TIMER11 interrupts                      */
    TIMER7_UP_TIMER12_IRQn       = 44,     /*!< TIMER7 update and TIMER12 interrupts                     */
    TIMER7_TRG_CMT_TIMER13_IRQn  = 45,     /*!< TIMER7 trigger and commutation and TIMER13 interrupts    */
    TIMER7_Channel_IRQn          = 46,     /*!< TIMER7 channel capture compare interrupt                 */
    DMA0_Channel7_IRQn           = 47,     /*!< DMA0 channel7 interrupt                                  */
    
#if defined (GD32F450)
    EXMC_IRQn                    = 48,     /*!< EXMC interrupt                                           */
    SDIO_IRQn                    = 49,     /*!< SDIO interrupt                                           */
    TIMER4_IRQn                  = 50,     /*!< TIMER4 interrupt                                         */
    SPI2_IRQn                    = 51,     /*!< SPI2 interrupt                                           */
    UART3_IRQn                   = 52,     /*!< UART3 interrupt                                          */
    UART4_IRQn                   = 53,     /*!< UART4 interrupt                                          */
    TIMER5_DAC_IRQn              = 54,     /*!< TIMER5 and DAC0 DAC1 underrun error interrupts           */
    TIMER6_IRQn                  = 55,     /*!< TIMER6 interrupt                                         */
    DMA1_Channel0_IRQn           = 56,     /*!< DMA1 channel0 interrupt                                  */
    DMA1_Channel1_IRQn           = 57,     /*!< DMA1 channel1 interrupt                                  */
    DMA1_Channel2_IRQn           = 58,     /*!< DMA1 channel2 interrupt                                  */
    DMA1_Channel3_IRQn           = 59,     /*!< DMA1 channel3 interrupt                                  */
    DMA1_Channel4_IRQn           = 60,     /*!< DMA1 channel4 interrupt                                  */
    ENET_IRQn                    = 61,     /*!< ENET interrupt                                           */
    ENET_WKUP_IRQn               = 62,     /*!< ENET wakeup through EXTI line interrupt                  */
    CAN1_TX_IRQn                 = 63,     /*!< CAN1 TX interrupt                                        */
    CAN1_RX0_IRQn                = 64,     /*!< CAN1 RX0 interrupt                                       */
    CAN1_RX1_IRQn                = 65,     /*!< CAN1 RX1 interrupt                                       */
    CAN1_EWMC_IRQn               = 66,     /*!< CAN1 EWMC interrupt                                      */
    USBFS_IRQn                   = 67,     /*!< USBFS interrupt                                          */
    DMA1_Channel5_IRQn           = 68,     /*!< DMA1 channel5 interrupt                                  */
    DMA1_Channel6_IRQn           = 69,     /*!< DMA1 channel6 interrupt                                  */
    DMA1_Channel7_IRQn           = 70,     /*!< DMA1 channel7 interrupt                                  */
    USART5_IRQn                  = 71,     /*!< USART5 interrupt                                         */
    I2C2_EV_IRQn                 = 72,     /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 73,     /*!< I2C2 error interrupt                                     */
    USBHS_EP1_Out_IRQn           = 74,     /*!< USBHS endpoint 1 out interrupt                           */
    USBHS_EP1_In_IRQn            = 75,     /*!< USBHS endpoint 1 in interrupt                            */
    USBHS_WKUP_IRQn              = 76,     /*!< USBHS wakeup through EXTI line interrupt                 */
    USBHS_IRQn                   = 77,     /*!< USBHS interrupt                                          */
    DCI_IRQn                     = 78,     /*!< DCI interrupt                                            */
    TRNG_IRQn                    = 80,     /*!< TRNG interrupt                                           */
    FPU_IRQn                     = 81,     /*!< FPU interrupt                                            */
    UART6_IRQn                   = 82,     /*!< UART6 interrupt                                          */
    UART7_IRQn                   = 83,     /*!< UART7 interrupt                                          */
    SPI3_IRQn                    = 84,     /*!< SPI3 interrupt                                           */
    SPI4_IRQn                    = 85,     /*!< SPI4 interrupt                                           */
    SPI5_IRQn                    = 86,     /*!< SPI5 interrupt                                           */
    TLI_IRQn                     = 88,     /*!< TLI interrupt                                            */
    TLI_ER_IRQn                  = 89,     /*!< TLI error interrupt                                      */
    IPA_IRQn                     = 90,     /*!< IPA interrupt                                            */
#endif /* GD32F450 */

#if defined (GD32F405)
    SDIO_IRQn                    = 49,     /*!< SDIO interrupt                                           */
    TIMER4_IRQn                  = 50,     /*!< TIMER4 interrupt                                         */
    SPI2_IRQn                    = 51,     /*!< SPI2 interrupt                                           */
    UART3_IRQn                   = 52,     /*!< UART3 interrupt                                          */
    UART4_IRQn                   = 53,     /*!< UART4 interrupt                                          */
    TIMER5_DAC_IRQn              = 54,     /*!< TIMER5 and DAC0 DAC1 underrun error interrupts           */
    TIMER6_IRQn                  = 55,     /*!< TIMER6 interrupt                                         */
    DMA1_Channel0_IRQn           = 56,     /*!< DMA1 channel0 interrupt                                  */
    DMA1_Channel1_IRQn           = 57,     /*!< DMA1 channel1 interrupt                                  */
    DMA1_Channel2_IRQn           = 58,     /*!< DMA1 channel2 interrupt                                  */
    DMA1_Channel3_IRQn           = 59,     /*!< DMA1 channel3 interrupt                                  */
    DMA1_Channel4_IRQn           = 60,     /*!< DMA1 channel4 interrupt                                  */
    CAN1_TX_IRQn                 = 63,     /*!< CAN1 TX interrupt                                        */
    CAN1_RX0_IRQn                = 64,     /*!< CAN1 RX0 interrupt                                       */
    CAN1_RX1_IRQn                = 65,     /*!< CAN1 RX1 interrupt                                       */
    CAN1_EWMC_IRQn               = 66,     /*!< CAN1 EWMC interrupt                                      */
    USBFS_IRQn                   = 67,     /*!< USBFS interrupt                                          */
    DMA1_Channel5_IRQn           = 68,     /*!< DMA1 channel5 interrupt                                  */
    DMA1_Channel6_IRQn           = 69,     /*!< DMA1 channel6 interrupt                                  */
    DMA1_Channel7_IRQn           = 70,     /*!< DMA1 channel7 interrupt                                  */
    USART5_IRQn                  = 71,     /*!< USART5 interrupt                                         */
    I2C2_EV_IRQn                 = 72,     /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 73,     /*!< I2C2 error interrupt                                     */
    USBHS_EP1_Out_IRQn           = 74,     /*!< USBHS endpoint 1 Out interrupt                           */
    USBHS_EP1_In_IRQn            = 75,     /*!< USBHS endpoint 1 in interrupt                            */
    USBHS_WKUP_IRQn              = 76,     /*!< USBHS wakeup through EXTI line interrupt                 */
    USBHS_IRQn                   = 77,     /*!< USBHS interrupt                                          */
    DCI_IRQn                     = 78,     /*!< DCI interrupt                                            */
    TRNG_IRQn                    = 80,     /*!< TRNG interrupt                                           */
    FPU_IRQn                     = 81,     /*!< FPU interrupt                                            */
#endif /* GD32F405 */

#if defined (GD32F407)
    EXMC_IRQn                    = 48,     /*!< EXMC interrupt                                           */
    SDIO_IRQn                    = 49,     /*!< SDIO interrupt                                           */
    TIMER4_IRQn                  = 50,     /*!< TIMER4 interrupt                                         */
    SPI2_IRQn                    = 51,     /*!< SPI2 interrupt                                           */
    UART3_IRQn                   = 52,     /*!< UART3 interrupt                                          */
    UART4_IRQn                   = 53,     /*!< UART4 interrupt                                          */
    TIMER5_DAC_IRQn              = 54,     /*!< TIMER5 and DAC0 DAC1 underrun error interrupts           */
    TIMER6_IRQn                  = 55,     /*!< TIMER6 interrupt                                         */
    DMA1_Channel0_IRQn           = 56,     /*!< DMA1 channel0 interrupt                                  */
    DMA1_Channel1_IRQn           = 57,     /*!< DMA1 channel1 interrupt                                  */
    DMA1_Channel2_IRQn           = 58,     /*!< DMA1 channel2 interrupt                                  */
    DMA1_Channel3_IRQn           = 59,     /*!< DMA1 channel3 interrupt                                  */
    DMA1_Channel4_IRQn           = 60,     /*!< DMA1 channel4 interrupt                                  */
    ENET_IRQn                    = 61,     /*!< ENET interrupt                                           */
    ENET_WKUP_IRQn               = 62,     /*!< ENET wakeup through EXTI line interrupt                  */
    CAN1_TX_IRQn                 = 63,     /*!< CAN1 TX interrupt                                        */
    CAN1_RX0_IRQn                = 64,     /*!< CAN1 RX0 interrupt                                       */
    CAN1_RX1_IRQn                = 65,     /*!< CAN1 RX1 interrupt                                       */
    CAN1_EWMC_IRQn               = 66,     /*!< CAN1 EWMC interrupt                                      */
    USBFS_IRQn                   = 67,     /*!< USBFS interrupt                                          */
    DMA1_Channel5_IRQn           = 68,     /*!< DMA1 channel5 interrupt                                  */
    DMA1_Channel6_IRQn           = 69,     /*!< DMA1 channel6 interrupt                                  */
    DMA1_Channel7_IRQn           = 70,     /*!< DMA1 channel7 interrupt                                  */
    USART5_IRQn                  = 71,     /*!< USART5 interrupt                                         */
    I2C2_EV_IRQn                 = 72,     /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 73,     /*!< I2C2 error interrupt                                     */
    USBHS_EP1_Out_IRQn           = 74,     /*!< USBHS endpoint 1 out interrupt                           */
    USBHS_EP1_In_IRQn            = 75,     /*!< USBHS endpoint 1 in interrupt                            */
    USBHS_WKUP_IRQn              = 76,     /*!< USBHS wakeup through EXTI line interrupt                 */
    USBHS_IRQn                   = 77,     /*!< USBHS interrupt                                          */
    DCI_IRQn                     = 78,     /*!< DCI interrupt                                            */
    TRNG_IRQn                    = 80,     /*!< TRNG interrupt                                           */
    FPU_IRQn                     = 81,     /*!< FPU interrupt                                            */
#endif /* GD32F407 */

} IRQn_Type;

/* includes */
#include "core_cm4.h"
#include "system_gd32f4xx.h"
#include <stdint.h>


/** @addtogroup Exported_types
  * @{
  */  
/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

#define assert_param(expr) ((void)0)

/* enum definitions */
typedef enum {DISABLE = 0, ENABLE = !DISABLE}EventStatus, ControlStatus;
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus;
typedef enum {FALSE = 0, TRUE = !FALSE} BOOL;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
/** 
  * @brief Reset and Clock Control
  */

typedef struct
{
  __IO uint32_t CTL;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  __IO uint32_t PLL;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  __IO uint32_t CFG0;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
  __IO uint32_t INT;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  __IO uint32_t AHB1RST;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  __IO uint32_t AHB2RST;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  __IO uint32_t AHB3RST;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint32_t      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  __IO uint32_t APB1RST;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  __IO uint32_t APB2RST;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  __IO uint32_t AHB1EN;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  __IO uint32_t AHB2EN;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  __IO uint32_t AHB3EN;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  __IO uint32_t APB1EN;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  __IO uint32_t APB2EN;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  __IO uint32_t AHB1SPEN;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  __IO uint32_t AHB2SPEN;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  __IO uint32_t AHB3SPEN;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint32_t      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  __IO uint32_t APB1SPEN;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  __IO uint32_t APB2SPEN;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  __IO uint32_t BDCTL;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  __IO uint32_t RSTSCK;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  __IO uint32_t PLLSSCTL;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  __IO uint32_t PLLI2S;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  __IO uint32_t PLLSAI;    /*!< RCC PLLSAI configuration register,                           Address offset: 0x88 */
  __IO uint32_t CFG1;       /*!< RCC Dedicated Clocks configuration register,                 Address offset: 0x8C */

} RCU_TypeDef;

/** 
  * @brief General Purpose I/O
  */

typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint16_t BSRRL;    /*!< GPIO port bit set/reset low register,  Address offset: 0x18      */
  __IO uint16_t BSRRH;    /*!< GPIO port bit set/reset high register, Address offset: 0x1A      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

/** 
  * @brief DCMI
  */

typedef struct
{
  __IO uint32_t CR;       /*!< DCMI control register 1,                       Address offset: 0x00 */
  __IO uint32_t SR;       /*!< DCMI status register,                          Address offset: 0x04 */
  __IO uint32_t RISR;     /*!< DCMI raw interrupt status register,            Address offset: 0x08 */
  __IO uint32_t IER;      /*!< DCMI interrupt enable register,                Address offset: 0x0C */
  __IO uint32_t MISR;     /*!< DCMI masked interrupt status register,         Address offset: 0x10 */
  __IO uint32_t ICR;      /*!< DCMI interrupt clear register,                 Address offset: 0x14 */
  __IO uint32_t ESCR;     /*!< DCMI embedded synchronization code register,   Address offset: 0x18 */
  __IO uint32_t ESUR;     /*!< DCMI embedded synchronization unmask register, Address offset: 0x1C */
  __IO uint32_t CWSTRTR;  /*!< DCMI crop window start,                        Address offset: 0x20 */
  __IO uint32_t CWSIZER;  /*!< DCMI crop window size,                         Address offset: 0x24 */
  __IO uint32_t DR;       /*!< DCMI data register,                            Address offset: 0x28 */
} DCMI_TypeDef;

/** 
  * @brief DMA Controller
  */

typedef struct
{
  __IO uint32_t CR;     /*!< DMA stream x configuration register      */
  __IO uint32_t NDTR;   /*!< DMA stream x number of data register     */
  __IO uint32_t PAR;    /*!< DMA stream x peripheral address register */
  __IO uint32_t M0AR;   /*!< DMA stream x memory 0 address register   */
  __IO uint32_t M1AR;   /*!< DMA stream x memory 1 address register   */
  __IO uint32_t FCR;    /*!< DMA stream x FIFO control register       */
} DMA_Stream_TypeDef;

typedef struct
{
  __IO uint32_t LISR;   /*!< DMA low interrupt status register,      Address offset: 0x00 */
  __IO uint32_t HISR;   /*!< DMA high interrupt status register,     Address offset: 0x04 */
  __IO uint32_t LIFCR;  /*!< DMA low interrupt flag clear register,  Address offset: 0x08 */
  __IO uint32_t HIFCR;  /*!< DMA high interrupt flag clear register, Address offset: 0x0C */
} DMA_TypeDef;

/* bit operations */
#define REG32(addr)                  (*(vu32 *)(u32)(addr))
#define REG16(addr)                  (*(vu16 *)(u32)(addr))
#define REG8(addr)                   (*(vu8 *)(u32)(addr))
#define BIT(x)                       ((u32)((u32)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (u32)(end)))) 
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

/* main flash and SRAM memory map */
#define FLASH_BASE            ((u32)0x08000000U)        /*!< main FLASH base address          */
#define TCMSRAM_BASE          ((u32)0x10000000U)        /*!< TCMSRAM(64KB) base address       */
#define OPTION_BASE           ((u32)0x1FFEC000U)        /*!< Option bytes base address        */
#define SRAM_BASE             ((u32)0x20000000U)        /*!< SRAM0 base address               */

/* peripheral memory map */
#define APB1_BUS_BASE         ((uint32_t)0x40000000U)        /*!< apb1 base address                */
#define APB2_BUS_BASE         ((uint32_t)0x40010000U)        /*!< apb2 base address                */
#define AHB1_BUS_BASE         ((uint32_t)0x40020000U)        /*!< ahb1 base address                */
#define AHB2_BUS_BASE         ((uint32_t)0x50000000U)        /*!< ahb2 base address                */

/* EXMC memory map */
#define EXMC_BASE             ((uint32_t)0xA0000000U)        /*!< EXMC register base address       */

/* advanced peripheral bus 1 memory map */
#define TIMER_BASE            (APB1_BUS_BASE + 0x00000000U)  /*!< TIMER base address               */
#define RTC_BASE              (APB1_BUS_BASE + 0x00002800U)  /*!< RTC base address                 */
#define WWDGT_BASE            (APB1_BUS_BASE + 0x00002C00U)  /*!< WWDGT base address               */
#define FWDGT_BASE            (APB1_BUS_BASE + 0x00003000U)  /*!< FWDGT base address               */
#define I2S_ADD_BASE          (APB1_BUS_BASE + 0x00003400U)  /*!< I2S1_add base address            */
#define SPI_BASE              (APB1_BUS_BASE + 0x00003800U)  /*!< SPI base address                 */
#define USART_BASE            (APB1_BUS_BASE + 0x00004400U)  /*!< USART base address               */
#define I2C_BASE              (APB1_BUS_BASE + 0x00005400U)  /*!< I2C base address                 */
#define CAN_BASE              (APB1_BUS_BASE + 0x00006400U)  /*!< CAN base address                 */
#define CTC_BASE              (APB1_BUS_BASE + 0x00006C00U)  /*!< CTC base address                 */
#define PMU_BASE              (APB1_BUS_BASE + 0x00007000U)  /*!< PMU base address                 */
#define DAC_BASE              (APB1_BUS_BASE + 0x00007400U)  /*!< DAC base address                 */
#define IREF_BASE             (APB1_BUS_BASE + 0x0000C400U)  /*!< IREF base address                */

/* advanced peripheral bus 2 memory map */
#define TLI_BASE              (APB2_BUS_BASE + 0x00006800U)  /*!< TLI base address                 */
#define SYSCFG_BASE           (APB2_BUS_BASE + 0x00003800U)  /*!< SYSCFG base address              */
#define EXTI_BASE             (APB2_BUS_BASE + 0x00003C00U)  /*!< EXTI base address                */
#define SDIO_BASE             (APB2_BUS_BASE + 0x00002C00U)  /*!< SDIO base address                */
#define ADC_BASE              (APB2_BUS_BASE + 0x00002000U)  /*!< ADC base address                 */
/* advanced high performance bus 1 memory map */
#define GPIO_BASE             (AHB1_BUS_BASE + 0x00000000U)  /*!< GPIO base address                */
/*!< AHB1 peripherals */
#define GPIOA_BASE            (AHB1_BUS_BASE + 0x00000000U)
#define GPIOB_BASE            (AHB1_BUS_BASE + 0x00000400U)
#define GPIOC_BASE            (AHB1_BUS_BASE + 0x00000800U)
#define GPIOD_BASE            (AHB1_BUS_BASE + 0x00000C00U)
#define GPIOE_BASE            (AHB1_BUS_BASE + 0x00001000U)
#define GPIOF_BASE            (AHB1_BUS_BASE + 0x00001400U)
#define GPIOG_BASE            (AHB1_BUS_BASE + 0x00001800U)
#define GPIOH_BASE            (AHB1_BUS_BASE + 0x00001C00U)
#define GPIOI_BASE            (AHB1_BUS_BASE + 0x00002000U)
#define GPIOJ_BASE            (AHB1_BUS_BASE + 0x00002400U)
#define GPIOK_BASE            (AHB1_BUS_BASE + 0x00002800U)
#define CRC_BASE              (AHB1_BUS_BASE + 0x00003000U)  /*!< CRC base address                 */
#define RCU_BASE              (AHB1_BUS_BASE + 0x00003800U)  /*!< RCU base address                 */
#define FMC_BASE              (AHB1_BUS_BASE + 0x00003C00U)  /*!< FMC base address                 */
#define BKPSRAM_BASE          (AHB1_BUS_BASE + 0x00004000U)  /*!< BKPSRAM base address             */
#define DMA0_BASE             (AHB1_BUS_BASE + 0x00006000U)  /*!< DMA base address                 */
#define DMA0_Stream0_BASE     (DMA0_BASE   +   0x00000010U)
#define DMA0_Stream1_BASE     (DMA0_BASE   +   0x00000028U)
#define DMA0_Stream2_BASE     (DMA0_BASE   +   0x00000040U)
#define DMA0_Stream3_BASE     (DMA0_BASE   +   0x00000058U)
#define DMA0_Stream4_BASE     (DMA0_BASE   +   0x00000070U)
#define DMA0_Stream5_BASE     (DMA0_BASE   +   0x00000088U)
#define DMA0_Stream6_BASE     (DMA0_BASE   +   0x000000A0U)
#define DMA0_Stream7_BASE     (DMA0_BASE   +   0x000000B8U)
#define DMA1_BASE             (AHB1_BUS_BASE + 0x00006400U)	 /*!< DMA base address */
#define DMA1_Stream0_BASE     (DMA1_BASE   +   0x00000010U)
#define DMA1_Stream1_BASE     (DMA1_BASE   +   0x00000028U)
#define DMA1_Stream2_BASE     (DMA1_BASE   +   0x00000040U)
#define DMA1_Stream3_BASE     (DMA1_BASE   +   0x00000058U)
#define DMA1_Stream4_BASE     (DMA1_BASE   +   0x00000070U)
#define DMA1_Stream5_BASE     (DMA1_BASE   +   0x00000088U)
#define DMA1_Stream6_BASE     (DMA1_BASE   +   0x000000A0U)
#define DMA1_Stream7_BASE     (DMA1_BASE   +   0x000000B8U)
#define ENET_BASE             (AHB1_BUS_BASE + 0x00008000U)  /*!< ENET base address                */
#define IPA_BASE              (AHB1_BUS_BASE + 0x0000B000U)  /*!< IPA base address                 */
#define USBHS_BASE            (AHB1_BUS_BASE + 0x00020000U)  /*!< USBHS base address               */
/* advanced high performance bus 2 memory map */
#define USBFS_BASE            (AHB2_BUS_BASE + 0x00000000U)  /*!< USBFS base address               */
#define DCMI_BASE             (AHB2_BUS_BASE + 0x00050000U)  /*!< DCI base address */
#define TRNG_BASE             (AHB2_BUS_BASE + 0x00060800U)  /*!< TRNG base address                */
/* option byte and debug memory map */
#define OB_BASE               ((uint32_t)0x1FFEC000U)        /*!< OB base address                  */
#define DBG_BASE              ((uint32_t)0xE0042000U)        /*!< DBG base address                 */
/**
  * @}
  */
  
/** @addtogroup Peripheral_declaration
  * @{
  */ 
#define RCU 									((RCU_TypeDef *) RCU_BASE)
#define DCMI                	((DCMI_TypeDef *) DCMI_BASE)
#define GPIOA             	  ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               	((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               	((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD              		((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE                 ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF             	  ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               	((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               	((GPIO_TypeDef *) GPIOH_BASE)
#define GPIOI              		((GPIO_TypeDef *) GPIOI_BASE)
#define DMA0                	((DMA_TypeDef *) DMA0_BASE)
#define DMA0_Stream0        ((DMA_Stream_TypeDef *) DMA0_Stream0_BASE)
#define DMA0_Stream1        ((DMA_Stream_TypeDef *) DMA0_Stream1_BASE)
#define DMA0_Stream2        ((DMA_Stream_TypeDef *) DMA0_Stream2_BASE)
#define DMA0_Stream3        ((DMA_Stream_TypeDef *) DMA0_Stream3_BASE)
#define DMA0_Stream4        ((DMA_Stream_TypeDef *) DMA0_Stream4_BASE)
#define DMA0_Stream5        ((DMA_Stream_TypeDef *) DMA0_Stream5_BASE)
#define DMA0_Stream6        ((DMA_Stream_TypeDef *) DMA0_Stream6_BASE)
#define DMA0_Stream7        ((DMA_Stream_TypeDef *) DMA0_Stream7_BASE)
#define DMA1                	((DMA_TypeDef *) DMA1_BASE)
#define DMA1_Stream0        ((DMA_Stream_TypeDef *) DMA1_Stream0_BASE)
#define DMA1_Stream1        ((DMA_Stream_TypeDef *) DMA1_Stream1_BASE)
#define DMA1_Stream2        ((DMA_Stream_TypeDef *) DMA1_Stream2_BASE)
#define DMA1_Stream3        ((DMA_Stream_TypeDef *) DMA1_Stream3_BASE)
#define DMA1_Stream4        ((DMA_Stream_TypeDef *) DMA1_Stream4_BASE)
#define DMA1_Stream5        ((DMA_Stream_TypeDef *) DMA1_Stream5_BASE)
#define DMA1_Stream6        ((DMA_Stream_TypeDef *) DMA1_Stream6_BASE)
#define DMA1_Stream7        ((DMA_Stream_TypeDef *) DMA1_Stream7_BASE)


/******************************************************************************/
/*                                                                            */
/*                                    DCMI                                    */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DCMI_CR register  ******************/
#define DCMI_CR_CAPTURE                      ((uint32_t)0x00000001)
#define DCMI_CR_CM                           ((uint32_t)0x00000002)
#define DCMI_CR_CROP                         ((uint32_t)0x00000004)
#define DCMI_CR_JPEG                         ((uint32_t)0x00000008)
#define DCMI_CR_ESS                          ((uint32_t)0x00000010)
#define DCMI_CR_PCKPOL                       ((uint32_t)0x00000020)
#define DCMI_CR_HSPOL                        ((uint32_t)0x00000040)
#define DCMI_CR_VSPOL                        ((uint32_t)0x00000080)
#define DCMI_CR_FCRC_0                       ((uint32_t)0x00000100)
#define DCMI_CR_FCRC_1                       ((uint32_t)0x00000200)
#define DCMI_CR_EDM_0                        ((uint32_t)0x00000400)
#define DCMI_CR_EDM_1                        ((uint32_t)0x00000800)
#define DCMI_CR_CRE                          ((uint32_t)0x00001000)
#define DCMI_CR_ENABLE                       ((uint32_t)0x00004000)

/********************  Bits definition for DCMI_SR register  ******************/
#define DCMI_SR_HSYNC                        ((uint32_t)0x00000001)
#define DCMI_SR_VSYNC                        ((uint32_t)0x00000002)
#define DCMI_SR_FNE                          ((uint32_t)0x00000004)

/********************  Bits definition for DCMI_RISR register  ****************/
#define DCMI_RISR_FRAME_RIS                  ((uint32_t)0x00000001)
#define DCMI_RISR_OVF_RIS                    ((uint32_t)0x00000002)
#define DCMI_RISR_ERR_RIS                    ((uint32_t)0x00000004)
#define DCMI_RISR_VSYNC_RIS                  ((uint32_t)0x00000008)
#define DCMI_RISR_LINE_RIS                   ((uint32_t)0x00000010)

/********************  Bits definition for DCMI_IER register  *****************/
#define DCMI_IER_FRAME_IE                    ((uint32_t)0x00000001)
#define DCMI_IER_OVF_IE                      ((uint32_t)0x00000002)
#define DCMI_IER_ERR_IE                      ((uint32_t)0x00000004)
#define DCMI_IER_VSYNC_IE                    ((uint32_t)0x00000008)
#define DCMI_IER_LINE_IE                     ((uint32_t)0x00000010)

/********************  Bits definition for DCMI_MISR register  ****************/
#define DCMI_MISR_FRAME_MIS                  ((uint32_t)0x00000001)
#define DCMI_MISR_OVF_MIS                    ((uint32_t)0x00000002)
#define DCMI_MISR_ERR_MIS                    ((uint32_t)0x00000004)
#define DCMI_MISR_VSYNC_MIS                  ((uint32_t)0x00000008)
#define DCMI_MISR_LINE_MIS                   ((uint32_t)0x00000010)

/********************  Bits definition for DCMI_ICR register  *****************/
#define DCMI_ICR_FRAME_ISC                   ((uint32_t)0x00000001)
#define DCMI_ICR_OVF_ISC                     ((uint32_t)0x00000002)
#define DCMI_ICR_ERR_ISC                     ((uint32_t)0x00000004)
#define DCMI_ICR_VSYNC_ISC                   ((uint32_t)0x00000008)
#define DCMI_ICR_LINE_ISC                    ((uint32_t)0x00000010)

/******************************************************************************/
/*                                                                            */
/*                             DMA Controller                                 */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for DMA_SxCR register  *****************/ 
#define DMA_SxCR_CHSEL                       ((uint32_t)0x0E000000)
#define DMA_SxCR_CHSEL_0                     ((uint32_t)0x02000000)
#define DMA_SxCR_CHSEL_1                     ((uint32_t)0x04000000)
#define DMA_SxCR_CHSEL_2                     ((uint32_t)0x08000000) 
#define DMA_SxCR_MBURST                      ((uint32_t)0x01800000)
#define DMA_SxCR_MBURST_0                    ((uint32_t)0x00800000)
#define DMA_SxCR_MBURST_1                    ((uint32_t)0x01000000)
#define DMA_SxCR_PBURST                      ((uint32_t)0x00600000)
#define DMA_SxCR_PBURST_0                    ((uint32_t)0x00200000)
#define DMA_SxCR_PBURST_1                    ((uint32_t)0x00400000)
#define DMA_SxCR_ACK                         ((uint32_t)0x00100000)
#define DMA_SxCR_CT                          ((uint32_t)0x00080000)  
#define DMA_SxCR_DBM                         ((uint32_t)0x00040000)
#define DMA_SxCR_PL                          ((uint32_t)0x00030000)
#define DMA_SxCR_PL_0                        ((uint32_t)0x00010000)
#define DMA_SxCR_PL_1                        ((uint32_t)0x00020000)
#define DMA_SxCR_PINCOS                      ((uint32_t)0x00008000)
#define DMA_SxCR_MSIZE                       ((uint32_t)0x00006000)
#define DMA_SxCR_MSIZE_0                     ((uint32_t)0x00002000)
#define DMA_SxCR_MSIZE_1                     ((uint32_t)0x00004000)
#define DMA_SxCR_PSIZE                       ((uint32_t)0x00001800)
#define DMA_SxCR_PSIZE_0                     ((uint32_t)0x00000800)
#define DMA_SxCR_PSIZE_1                     ((uint32_t)0x00001000)
#define DMA_SxCR_MINC                        ((uint32_t)0x00000400)
#define DMA_SxCR_PINC                        ((uint32_t)0x00000200)
#define DMA_SxCR_CIRC                        ((uint32_t)0x00000100)
#define DMA_SxCR_DIR                         ((uint32_t)0x000000C0)
#define DMA_SxCR_DIR_0                       ((uint32_t)0x00000040)
#define DMA_SxCR_DIR_1                       ((uint32_t)0x00000080)
#define DMA_SxCR_PFCTRL                      ((uint32_t)0x00000020)
#define DMA_SxCR_TCIE                        ((uint32_t)0x00000010)
#define DMA_SxCR_HTIE                        ((uint32_t)0x00000008)
#define DMA_SxCR_TEIE                        ((uint32_t)0x00000004)
#define DMA_SxCR_DMEIE                       ((uint32_t)0x00000002)
#define DMA_SxCR_EN                          ((uint32_t)0x00000001)

/********************  Bits definition for DMA_SxCNDTR register  **************/
#define DMA_SxNDT                            ((uint32_t)0x0000FFFF)
#define DMA_SxNDT_0                          ((uint32_t)0x00000001)
#define DMA_SxNDT_1                          ((uint32_t)0x00000002)
#define DMA_SxNDT_2                          ((uint32_t)0x00000004)
#define DMA_SxNDT_3                          ((uint32_t)0x00000008)
#define DMA_SxNDT_4                          ((uint32_t)0x00000010)
#define DMA_SxNDT_5                          ((uint32_t)0x00000020)
#define DMA_SxNDT_6                          ((uint32_t)0x00000040)
#define DMA_SxNDT_7                          ((uint32_t)0x00000080)
#define DMA_SxNDT_8                          ((uint32_t)0x00000100)
#define DMA_SxNDT_9                          ((uint32_t)0x00000200)
#define DMA_SxNDT_10                         ((uint32_t)0x00000400)
#define DMA_SxNDT_11                         ((uint32_t)0x00000800)
#define DMA_SxNDT_12                         ((uint32_t)0x00001000)
#define DMA_SxNDT_13                         ((uint32_t)0x00002000)
#define DMA_SxNDT_14                         ((uint32_t)0x00004000)
#define DMA_SxNDT_15                         ((uint32_t)0x00008000)

/********************  Bits definition for DMA_SxFCR register  ****************/ 
#define DMA_SxFCR_FEIE                       ((uint32_t)0x00000080)
#define DMA_SxFCR_FS                         ((uint32_t)0x00000038)
#define DMA_SxFCR_FS_0                       ((uint32_t)0x00000008)
#define DMA_SxFCR_FS_1                       ((uint32_t)0x00000010)
#define DMA_SxFCR_FS_2                       ((uint32_t)0x00000020)
#define DMA_SxFCR_DMDIS                      ((uint32_t)0x00000004)
#define DMA_SxFCR_FTH                        ((uint32_t)0x00000003)
#define DMA_SxFCR_FTH_0                      ((uint32_t)0x00000001)
#define DMA_SxFCR_FTH_1                      ((uint32_t)0x00000002)

/********************  Bits definition for DMA_LISR register  *****************/ 
#define DMA_LISR_TCIF3                       ((uint32_t)0x08000000)
#define DMA_LISR_HTIF3                       ((uint32_t)0x04000000)
#define DMA_LISR_TEIF3                       ((uint32_t)0x02000000)
#define DMA_LISR_DMEIF3                      ((uint32_t)0x01000000)
#define DMA_LISR_FEIF3                       ((uint32_t)0x00400000)
#define DMA_LISR_TCIF2                       ((uint32_t)0x00200000)
#define DMA_LISR_HTIF2                       ((uint32_t)0x00100000)
#define DMA_LISR_TEIF2                       ((uint32_t)0x00080000)
#define DMA_LISR_DMEIF2                      ((uint32_t)0x00040000)
#define DMA_LISR_FEIF2                       ((uint32_t)0x00010000)
#define DMA_LISR_TCIF1                       ((uint32_t)0x00000800)
#define DMA_LISR_HTIF1                       ((uint32_t)0x00000400)
#define DMA_LISR_TEIF1                       ((uint32_t)0x00000200)
#define DMA_LISR_DMEIF1                      ((uint32_t)0x00000100)
#define DMA_LISR_FEIF1                       ((uint32_t)0x00000040)
#define DMA_LISR_TCIF0                       ((uint32_t)0x00000020)
#define DMA_LISR_HTIF0                       ((uint32_t)0x00000010)
#define DMA_LISR_TEIF0                       ((uint32_t)0x00000008)
#define DMA_LISR_DMEIF0                      ((uint32_t)0x00000004)
#define DMA_LISR_FEIF0                       ((uint32_t)0x00000001)

/********************  Bits definition for DMA_HISR register  *****************/ 
#define DMA_HISR_TCIF7                       ((uint32_t)0x08000000)
#define DMA_HISR_HTIF7                       ((uint32_t)0x04000000)
#define DMA_HISR_TEIF7                       ((uint32_t)0x02000000)
#define DMA_HISR_DMEIF7                      ((uint32_t)0x01000000)
#define DMA_HISR_FEIF7                       ((uint32_t)0x00400000)
#define DMA_HISR_TCIF6                       ((uint32_t)0x00200000)
#define DMA_HISR_HTIF6                       ((uint32_t)0x00100000)
#define DMA_HISR_TEIF6                       ((uint32_t)0x00080000)
#define DMA_HISR_DMEIF6                      ((uint32_t)0x00040000)
#define DMA_HISR_FEIF6                       ((uint32_t)0x00010000)
#define DMA_HISR_TCIF5                       ((uint32_t)0x00000800)
#define DMA_HISR_HTIF5                       ((uint32_t)0x00000400)
#define DMA_HISR_TEIF5                       ((uint32_t)0x00000200)
#define DMA_HISR_DMEIF5                      ((uint32_t)0x00000100)
#define DMA_HISR_FEIF5                       ((uint32_t)0x00000040)
#define DMA_HISR_TCIF4                       ((uint32_t)0x00000020)
#define DMA_HISR_HTIF4                       ((uint32_t)0x00000010)
#define DMA_HISR_TEIF4                       ((uint32_t)0x00000008)
#define DMA_HISR_DMEIF4                      ((uint32_t)0x00000004)
#define DMA_HISR_FEIF4                       ((uint32_t)0x00000001)

/********************  Bits definition for DMA_LIFCR register  ****************/ 
#define DMA_LIFCR_CTCIF3                     ((uint32_t)0x08000000)
#define DMA_LIFCR_CHTIF3                     ((uint32_t)0x04000000)
#define DMA_LIFCR_CTEIF3                     ((uint32_t)0x02000000)
#define DMA_LIFCR_CDMEIF3                    ((uint32_t)0x01000000)
#define DMA_LIFCR_CFEIF3                     ((uint32_t)0x00400000)
#define DMA_LIFCR_CTCIF2                     ((uint32_t)0x00200000)
#define DMA_LIFCR_CHTIF2                     ((uint32_t)0x00100000)
#define DMA_LIFCR_CTEIF2                     ((uint32_t)0x00080000)
#define DMA_LIFCR_CDMEIF2                    ((uint32_t)0x00040000)
#define DMA_LIFCR_CFEIF2                     ((uint32_t)0x00010000)
#define DMA_LIFCR_CTCIF1                     ((uint32_t)0x00000800)
#define DMA_LIFCR_CHTIF1                     ((uint32_t)0x00000400)
#define DMA_LIFCR_CTEIF1                     ((uint32_t)0x00000200)
#define DMA_LIFCR_CDMEIF1                    ((uint32_t)0x00000100)
#define DMA_LIFCR_CFEIF1                     ((uint32_t)0x00000040)
#define DMA_LIFCR_CTCIF0                     ((uint32_t)0x00000020)
#define DMA_LIFCR_CHTIF0                     ((uint32_t)0x00000010)
#define DMA_LIFCR_CTEIF0                     ((uint32_t)0x00000008)
#define DMA_LIFCR_CDMEIF0                    ((uint32_t)0x00000004)
#define DMA_LIFCR_CFEIF0                     ((uint32_t)0x00000001)

/********************  Bits definition for DMA_HIFCR  register  ****************/ 
#define DMA_HIFCR_CTCIF7                     ((uint32_t)0x08000000)
#define DMA_HIFCR_CHTIF7                     ((uint32_t)0x04000000)
#define DMA_HIFCR_CTEIF7                     ((uint32_t)0x02000000)
#define DMA_HIFCR_CDMEIF7                    ((uint32_t)0x01000000)
#define DMA_HIFCR_CFEIF7                     ((uint32_t)0x00400000)
#define DMA_HIFCR_CTCIF6                     ((uint32_t)0x00200000)
#define DMA_HIFCR_CHTIF6                     ((uint32_t)0x00100000)
#define DMA_HIFCR_CTEIF6                     ((uint32_t)0x00080000)
#define DMA_HIFCR_CDMEIF6                    ((uint32_t)0x00040000)
#define DMA_HIFCR_CFEIF6                     ((uint32_t)0x00010000)
#define DMA_HIFCR_CTCIF5                     ((uint32_t)0x00000800)
#define DMA_HIFCR_CHTIF5                     ((uint32_t)0x00000400)
#define DMA_HIFCR_CTEIF5                     ((uint32_t)0x00000200)
#define DMA_HIFCR_CDMEIF5                    ((uint32_t)0x00000100)
#define DMA_HIFCR_CFEIF5                     ((uint32_t)0x00000040)
#define DMA_HIFCR_CTCIF4                     ((uint32_t)0x00000020)
#define DMA_HIFCR_CHTIF4                     ((uint32_t)0x00000010)
#define DMA_HIFCR_CTEIF4                     ((uint32_t)0x00000008)
#define DMA_HIFCR_CDMEIF4                    ((uint32_t)0x00000004)
#define DMA_HIFCR_CFEIF4                     ((uint32_t)0x00000001)
/******************************************************************************/
/*                                                                            */
/*                         AHB Master DMA2D Controller (DMA2D)                */
/*                                                                            */
/******************************************************************************/

/********************  Bit definition for DMA2D_CR register  ******************/

#define DMA2D_CR_START                     ((uint32_t)0x00000001)               /*!< Start transfer */
#define DMA2D_CR_SUSP                      ((uint32_t)0x00000002)               /*!< Suspend transfer */
#define DMA2D_CR_ABORT                     ((uint32_t)0x00000004)               /*!< Abort transfer */
#define DMA2D_CR_TEIE                      ((uint32_t)0x00000100)               /*!< Transfer Error Interrupt Enable */
#define DMA2D_CR_TCIE                      ((uint32_t)0x00000200)               /*!< Transfer Complete Interrupt Enable */
#define DMA2D_CR_TWIE                      ((uint32_t)0x00000400)               /*!< Transfer Watermark Interrupt Enable */
#define DMA2D_CR_CAEIE                     ((uint32_t)0x00000800)               /*!< CLUT Access Error Interrupt Enable */
#define DMA2D_CR_CTCIE                     ((uint32_t)0x00001000)               /*!< CLUT Transfer Complete Interrupt Enable */
#define DMA2D_CR_CEIE                      ((uint32_t)0x00002000)               /*!< Configuration Error Interrupt Enable */
#define DMA2D_CR_MODE                      ((uint32_t)0x00030000)               /*!< DMA2D Mode */

/********************  Bit definition for DMA2D_ISR register  *****************/

#define DMA2D_ISR_TEIF                     ((uint32_t)0x00000001)               /*!< Transfer Error Interrupt Flag */
#define DMA2D_ISR_TCIF                     ((uint32_t)0x00000002)               /*!< Transfer Complete Interrupt Flag */
#define DMA2D_ISR_TWIF                     ((uint32_t)0x00000004)               /*!< Transfer Watermark Interrupt Flag */
#define DMA2D_ISR_CAEIF                    ((uint32_t)0x00000008)               /*!< CLUT Access Error Interrupt Flag */
#define DMA2D_ISR_CTCIF                    ((uint32_t)0x00000010)               /*!< CLUT Transfer Complete Interrupt Flag */
#define DMA2D_ISR_CEIF                     ((uint32_t)0x00000020)               /*!< Configuration Error Interrupt Flag */

/********************  Bit definition for DMA2D_IFSR register  ****************/

#define DMA2D_IFSR_CTEIF                   ((uint32_t)0x00000001)               /*!< Clears Transfer Error Interrupt Flag */
#define DMA2D_IFSR_CTCIF                   ((uint32_t)0x00000002)               /*!< Clears Transfer Complete Interrupt Flag */
#define DMA2D_IFSR_CTWIF                   ((uint32_t)0x00000004)               /*!< Clears Transfer Watermark Interrupt Flag */
#define DMA2D_IFSR_CCAEIF                  ((uint32_t)0x00000008)               /*!< Clears CLUT Access Error Interrupt Flag */
#define DMA2D_IFSR_CCTCIF                  ((uint32_t)0x00000010)               /*!< Clears CLUT Transfer Complete Interrupt Flag */
#define DMA2D_IFSR_CCEIF                   ((uint32_t)0x00000020)               /*!< Clears Configuration Error Interrupt Flag */

/********************  Bit definition for DMA2D_FGMAR register  ***************/

#define DMA2D_FGMAR_MA                     ((uint32_t)0xFFFFFFFF)               /*!< Memory Address */

/********************  Bit definition for DMA2D_FGOR register  ****************/

#define DMA2D_FGOR_LO                      ((uint32_t)0x00003FFF)               /*!< Line Offset */

/********************  Bit definition for DMA2D_BGMAR register  ***************/

#define DMA2D_BGMAR_MA                     ((uint32_t)0xFFFFFFFF)               /*!< Memory Address */

/********************  Bit definition for DMA2D_BGOR register  ****************/

#define DMA2D_BGOR_LO                      ((uint32_t)0x00003FFF)               /*!< Line Offset */

/********************  Bit definition for DMA2D_FGPFCCR register  *************/

#define DMA2D_FGPFCCR_CM                   ((uint32_t)0x0000000F)               /*!< Color mode */
#define DMA2D_FGPFCCR_CCM                  ((uint32_t)0x00000010)               /*!< CLUT Color mode */
#define DMA2D_FGPFCCR_START                ((uint32_t)0x00000020)               /*!< Start */
#define DMA2D_FGPFCCR_CS                   ((uint32_t)0x0000FF00)               /*!< CLUT size */
#define DMA2D_FGPFCCR_AM                   ((uint32_t)0x00030000)               /*!< Alpha mode */
#define DMA2D_FGPFCCR_ALPHA                ((uint32_t)0xFF000000)               /*!< Alpha value */

/********************  Bit definition for DMA2D_FGCOLR register  **************/

#define DMA2D_FGCOLR_BLUE                  ((uint32_t)0x000000FF)               /*!< Blue Value */
#define DMA2D_FGCOLR_GREEN                 ((uint32_t)0x0000FF00)               /*!< Green Value */
#define DMA2D_FGCOLR_RED                   ((uint32_t)0x00FF0000)               /*!< Red Value */   

/********************  Bit definition for DMA2D_BGPFCCR register  *************/

#define DMA2D_BGPFCCR_CM                   ((uint32_t)0x0000000F)               /*!< Color mode */
#define DMA2D_BGPFCCR_CCM                  ((uint32_t)0x00000010)               /*!< CLUT Color mode */
#define DMA2D_BGPFCCR_START                ((uint32_t)0x00000020)               /*!< Start */
#define DMA2D_BGPFCCR_CS                   ((uint32_t)0x0000FF00)               /*!< CLUT size */
#define DMA2D_BGPFCCR_AM                   ((uint32_t)0x00030000)               /*!< Alpha Mode */
#define DMA2D_BGPFCCR_ALPHA                ((uint32_t)0xFF000000)               /*!< Alpha value */

/********************  Bit definition for DMA2D_BGCOLR register  **************/

#define DMA2D_BGCOLR_BLUE                  ((uint32_t)0x000000FF)               /*!< Blue Value */
#define DMA2D_BGCOLR_GREEN                 ((uint32_t)0x0000FF00)               /*!< Green Value */
#define DMA2D_BGCOLR_RED                   ((uint32_t)0x00FF0000)               /*!< Red Value */

/********************  Bit definition for DMA2D_FGCMAR register  **************/

#define DMA2D_FGCMAR_MA                    ((uint32_t)0xFFFFFFFF)               /*!< Memory Address */

/********************  Bit definition for DMA2D_BGCMAR register  **************/

#define DMA2D_BGCMAR_MA                    ((uint32_t)0xFFFFFFFF)               /*!< Memory Address */

/********************  Bit definition for DMA2D_OPFCCR register  **************/

#define DMA2D_OPFCCR_CM                    ((uint32_t)0x00000007)               /*!< Color mode */

/********************  Bit definition for DMA2D_OCOLR register  ***************/

/*!<Mode_ARGB8888/RGB888 */

#define DMA2D_OCOLR_BLUE_1                 ((uint32_t)0x000000FF)               /*!< BLUE Value */
#define DMA2D_OCOLR_GREEN_1                ((uint32_t)0x0000FF00)               /*!< GREEN Value  */
#define DMA2D_OCOLR_RED_1                  ((uint32_t)0x00FF0000)               /*!< Red Value */
#define DMA2D_OCOLR_ALPHA_1                ((uint32_t)0xFF000000)               /*!< Alpha Channel Value */

/*!<Mode_RGB565 */
#define DMA2D_OCOLR_BLUE_2                 ((uint32_t)0x0000001F)               /*!< BLUE Value */
#define DMA2D_OCOLR_GREEN_2                ((uint32_t)0x000007E0)               /*!< GREEN Value  */
#define DMA2D_OCOLR_RED_2                  ((uint32_t)0x0000F800)               /*!< Red Value */

/*!<Mode_ARGB1555 */
#define DMA2D_OCOLR_BLUE_3                 ((uint32_t)0x0000001F)               /*!< BLUE Value */
#define DMA2D_OCOLR_GREEN_3                ((uint32_t)0x000003E0)               /*!< GREEN Value  */
#define DMA2D_OCOLR_RED_3                  ((uint32_t)0x00007C00)               /*!< Red Value */
#define DMA2D_OCOLR_ALPHA_3                ((uint32_t)0x00008000)               /*!< Alpha Channel Value */

/*!<Mode_ARGB4444 */
#define DMA2D_OCOLR_BLUE_4                 ((uint32_t)0x0000000F)               /*!< BLUE Value */
#define DMA2D_OCOLR_GREEN_4                ((uint32_t)0x000000F0)               /*!< GREEN Value  */
#define DMA2D_OCOLR_RED_4                  ((uint32_t)0x00000F00)               /*!< Red Value */
#define DMA2D_OCOLR_ALPHA_4                ((uint32_t)0x0000F000)               /*!< Alpha Channel Value */

/********************  Bit definition for DMA2D_OMAR register  ****************/

#define DMA2D_OMAR_MA                      ((uint32_t)0xFFFFFFFF)               /*!< Memory Address */

/********************  Bit definition for DMA2D_OOR register  *****************/

#define DMA2D_OOR_LO                       ((uint32_t)0x00003FFF)               /*!< Line Offset */

/********************  Bit definition for DMA2D_NLR register  *****************/

#define DMA2D_NLR_NL                       ((uint32_t)0x0000FFFF)               /*!< Number of Lines */
#define DMA2D_NLR_PL                       ((uint32_t)0x3FFF0000)               /*!< Pixel per Lines */

/********************  Bit definition for DMA2D_LWR register  *****************/

#define DMA2D_LWR_LW                       ((uint32_t)0x0000FFFF)               /*!< Line Watermark */

/********************  Bit definition for DMA2D_AMTCR register  ***************/

#define DMA2D_AMTCR_EN                     ((uint32_t)0x00000001)               /*!< Enable */
#define DMA2D_AMTCR_DT                     ((uint32_t)0x0000FF00)               /*!< Dead Time */

/******************************************************************************/
/*                                                                            */
/*                            General Purpose I/O                             */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODER0                    ((uint32_t)0x00000003)
#define GPIO_MODER_MODER0_0                  ((uint32_t)0x00000001)
#define GPIO_MODER_MODER0_1                  ((uint32_t)0x00000002)

#define GPIO_MODER_MODER1                    ((uint32_t)0x0000000C)
#define GPIO_MODER_MODER1_0                  ((uint32_t)0x00000004)
#define GPIO_MODER_MODER1_1                  ((uint32_t)0x00000008)

#define GPIO_MODER_MODER2                    ((uint32_t)0x00000030)
#define GPIO_MODER_MODER2_0                  ((uint32_t)0x00000010)
#define GPIO_MODER_MODER2_1                  ((uint32_t)0x00000020)

#define GPIO_MODER_MODER3                    ((uint32_t)0x000000C0)
#define GPIO_MODER_MODER3_0                  ((uint32_t)0x00000040)
#define GPIO_MODER_MODER3_1                  ((uint32_t)0x00000080)

#define GPIO_MODER_MODER4                    ((uint32_t)0x00000300)
#define GPIO_MODER_MODER4_0                  ((uint32_t)0x00000100)
#define GPIO_MODER_MODER4_1                  ((uint32_t)0x00000200)

#define GPIO_MODER_MODER5                    ((uint32_t)0x00000C00)
#define GPIO_MODER_MODER5_0                  ((uint32_t)0x00000400)
#define GPIO_MODER_MODER5_1                  ((uint32_t)0x00000800)

#define GPIO_MODER_MODER6                    ((uint32_t)0x00003000)
#define GPIO_MODER_MODER6_0                  ((uint32_t)0x00001000)
#define GPIO_MODER_MODER6_1                  ((uint32_t)0x00002000)

#define GPIO_MODER_MODER7                    ((uint32_t)0x0000C000)
#define GPIO_MODER_MODER7_0                  ((uint32_t)0x00004000)
#define GPIO_MODER_MODER7_1                  ((uint32_t)0x00008000)

#define GPIO_MODER_MODER8                    ((uint32_t)0x00030000)
#define GPIO_MODER_MODER8_0                  ((uint32_t)0x00010000)
#define GPIO_MODER_MODER8_1                  ((uint32_t)0x00020000)

#define GPIO_MODER_MODER9                    ((uint32_t)0x000C0000)
#define GPIO_MODER_MODER9_0                  ((uint32_t)0x00040000)
#define GPIO_MODER_MODER9_1                  ((uint32_t)0x00080000)

#define GPIO_MODER_MODER10                   ((uint32_t)0x00300000)
#define GPIO_MODER_MODER10_0                 ((uint32_t)0x00100000)
#define GPIO_MODER_MODER10_1                 ((uint32_t)0x00200000)

#define GPIO_MODER_MODER11                   ((uint32_t)0x00C00000)
#define GPIO_MODER_MODER11_0                 ((uint32_t)0x00400000)
#define GPIO_MODER_MODER11_1                 ((uint32_t)0x00800000)

#define GPIO_MODER_MODER12                   ((uint32_t)0x03000000)
#define GPIO_MODER_MODER12_0                 ((uint32_t)0x01000000)
#define GPIO_MODER_MODER12_1                 ((uint32_t)0x02000000)

#define GPIO_MODER_MODER13                   ((uint32_t)0x0C000000)
#define GPIO_MODER_MODER13_0                 ((uint32_t)0x04000000)
#define GPIO_MODER_MODER13_1                 ((uint32_t)0x08000000)

#define GPIO_MODER_MODER14                   ((uint32_t)0x30000000)
#define GPIO_MODER_MODER14_0                 ((uint32_t)0x10000000)
#define GPIO_MODER_MODER14_1                 ((uint32_t)0x20000000)

#define GPIO_MODER_MODER15                   ((uint32_t)0xC0000000)
#define GPIO_MODER_MODER15_0                 ((uint32_t)0x40000000)
#define GPIO_MODER_MODER15_1                 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_OTYPER register  ****************/
#define GPIO_OTYPER_OT_0                     ((uint32_t)0x00000001)
#define GPIO_OTYPER_OT_1                     ((uint32_t)0x00000002)
#define GPIO_OTYPER_OT_2                     ((uint32_t)0x00000004)
#define GPIO_OTYPER_OT_3                     ((uint32_t)0x00000008)
#define GPIO_OTYPER_OT_4                     ((uint32_t)0x00000010)
#define GPIO_OTYPER_OT_5                     ((uint32_t)0x00000020)
#define GPIO_OTYPER_OT_6                     ((uint32_t)0x00000040)
#define GPIO_OTYPER_OT_7                     ((uint32_t)0x00000080)
#define GPIO_OTYPER_OT_8                     ((uint32_t)0x00000100)
#define GPIO_OTYPER_OT_9                     ((uint32_t)0x00000200)
#define GPIO_OTYPER_OT_10                    ((uint32_t)0x00000400)
#define GPIO_OTYPER_OT_11                    ((uint32_t)0x00000800)
#define GPIO_OTYPER_OT_12                    ((uint32_t)0x00001000)
#define GPIO_OTYPER_OT_13                    ((uint32_t)0x00002000)
#define GPIO_OTYPER_OT_14                    ((uint32_t)0x00004000)
#define GPIO_OTYPER_OT_15                    ((uint32_t)0x00008000)

/******************  Bits definition for GPIO_OSPEEDR register  ***************/
#define GPIO_OSPEEDER_OSPEEDR0               ((uint32_t)0x00000003)
#define GPIO_OSPEEDER_OSPEEDR0_0             ((uint32_t)0x00000001)
#define GPIO_OSPEEDER_OSPEEDR0_1             ((uint32_t)0x00000002)

#define GPIO_OSPEEDER_OSPEEDR1               ((uint32_t)0x0000000C)
#define GPIO_OSPEEDER_OSPEEDR1_0             ((uint32_t)0x00000004)
#define GPIO_OSPEEDER_OSPEEDR1_1             ((uint32_t)0x00000008)

#define GPIO_OSPEEDER_OSPEEDR2               ((uint32_t)0x00000030)
#define GPIO_OSPEEDER_OSPEEDR2_0             ((uint32_t)0x00000010)
#define GPIO_OSPEEDER_OSPEEDR2_1             ((uint32_t)0x00000020)

#define GPIO_OSPEEDER_OSPEEDR3               ((uint32_t)0x000000C0)
#define GPIO_OSPEEDER_OSPEEDR3_0             ((uint32_t)0x00000040)
#define GPIO_OSPEEDER_OSPEEDR3_1             ((uint32_t)0x00000080)

#define GPIO_OSPEEDER_OSPEEDR4               ((uint32_t)0x00000300)
#define GPIO_OSPEEDER_OSPEEDR4_0             ((uint32_t)0x00000100)
#define GPIO_OSPEEDER_OSPEEDR4_1             ((uint32_t)0x00000200)

#define GPIO_OSPEEDER_OSPEEDR5               ((uint32_t)0x00000C00)
#define GPIO_OSPEEDER_OSPEEDR5_0             ((uint32_t)0x00000400)
#define GPIO_OSPEEDER_OSPEEDR5_1             ((uint32_t)0x00000800)

#define GPIO_OSPEEDER_OSPEEDR6               ((uint32_t)0x00003000)
#define GPIO_OSPEEDER_OSPEEDR6_0             ((uint32_t)0x00001000)
#define GPIO_OSPEEDER_OSPEEDR6_1             ((uint32_t)0x00002000)

#define GPIO_OSPEEDER_OSPEEDR7               ((uint32_t)0x0000C000)
#define GPIO_OSPEEDER_OSPEEDR7_0             ((uint32_t)0x00004000)
#define GPIO_OSPEEDER_OSPEEDR7_1             ((uint32_t)0x00008000)

#define GPIO_OSPEEDER_OSPEEDR8               ((uint32_t)0x00030000)
#define GPIO_OSPEEDER_OSPEEDR8_0             ((uint32_t)0x00010000)
#define GPIO_OSPEEDER_OSPEEDR8_1             ((uint32_t)0x00020000)

#define GPIO_OSPEEDER_OSPEEDR9               ((uint32_t)0x000C0000)
#define GPIO_OSPEEDER_OSPEEDR9_0             ((uint32_t)0x00040000)
#define GPIO_OSPEEDER_OSPEEDR9_1             ((uint32_t)0x00080000)

#define GPIO_OSPEEDER_OSPEEDR10              ((uint32_t)0x00300000)
#define GPIO_OSPEEDER_OSPEEDR10_0            ((uint32_t)0x00100000)
#define GPIO_OSPEEDER_OSPEEDR10_1            ((uint32_t)0x00200000)

#define GPIO_OSPEEDER_OSPEEDR11              ((uint32_t)0x00C00000)
#define GPIO_OSPEEDER_OSPEEDR11_0            ((uint32_t)0x00400000)
#define GPIO_OSPEEDER_OSPEEDR11_1            ((uint32_t)0x00800000)

#define GPIO_OSPEEDER_OSPEEDR12              ((uint32_t)0x03000000)
#define GPIO_OSPEEDER_OSPEEDR12_0            ((uint32_t)0x01000000)
#define GPIO_OSPEEDER_OSPEEDR12_1            ((uint32_t)0x02000000)

#define GPIO_OSPEEDER_OSPEEDR13              ((uint32_t)0x0C000000)
#define GPIO_OSPEEDER_OSPEEDR13_0            ((uint32_t)0x04000000)
#define GPIO_OSPEEDER_OSPEEDR13_1            ((uint32_t)0x08000000)

#define GPIO_OSPEEDER_OSPEEDR14              ((uint32_t)0x30000000)
#define GPIO_OSPEEDER_OSPEEDR14_0            ((uint32_t)0x10000000)
#define GPIO_OSPEEDER_OSPEEDR14_1            ((uint32_t)0x20000000)

#define GPIO_OSPEEDER_OSPEEDR15              ((uint32_t)0xC0000000)
#define GPIO_OSPEEDER_OSPEEDR15_0            ((uint32_t)0x40000000)
#define GPIO_OSPEEDER_OSPEEDR15_1            ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_PUPDR register  *****************/
#define GPIO_PUPDR_PUPDR0                    ((uint32_t)0x00000003)
#define GPIO_PUPDR_PUPDR0_0                  ((uint32_t)0x00000001)
#define GPIO_PUPDR_PUPDR0_1                  ((uint32_t)0x00000002)

#define GPIO_PUPDR_PUPDR1                    ((uint32_t)0x0000000C)
#define GPIO_PUPDR_PUPDR1_0                  ((uint32_t)0x00000004)
#define GPIO_PUPDR_PUPDR1_1                  ((uint32_t)0x00000008)

#define GPIO_PUPDR_PUPDR2                    ((uint32_t)0x00000030)
#define GPIO_PUPDR_PUPDR2_0                  ((uint32_t)0x00000010)
#define GPIO_PUPDR_PUPDR2_1                  ((uint32_t)0x00000020)

#define GPIO_PUPDR_PUPDR3                    ((uint32_t)0x000000C0)
#define GPIO_PUPDR_PUPDR3_0                  ((uint32_t)0x00000040)
#define GPIO_PUPDR_PUPDR3_1                  ((uint32_t)0x00000080)

#define GPIO_PUPDR_PUPDR4                    ((uint32_t)0x00000300)
#define GPIO_PUPDR_PUPDR4_0                  ((uint32_t)0x00000100)
#define GPIO_PUPDR_PUPDR4_1                  ((uint32_t)0x00000200)

#define GPIO_PUPDR_PUPDR5                    ((uint32_t)0x00000C00)
#define GPIO_PUPDR_PUPDR5_0                  ((uint32_t)0x00000400)
#define GPIO_PUPDR_PUPDR5_1                  ((uint32_t)0x00000800)

#define GPIO_PUPDR_PUPDR6                    ((uint32_t)0x00003000)
#define GPIO_PUPDR_PUPDR6_0                  ((uint32_t)0x00001000)
#define GPIO_PUPDR_PUPDR6_1                  ((uint32_t)0x00002000)

#define GPIO_PUPDR_PUPDR7                    ((uint32_t)0x0000C000)
#define GPIO_PUPDR_PUPDR7_0                  ((uint32_t)0x00004000)
#define GPIO_PUPDR_PUPDR7_1                  ((uint32_t)0x00008000)

#define GPIO_PUPDR_PUPDR8                    ((uint32_t)0x00030000)
#define GPIO_PUPDR_PUPDR8_0                  ((uint32_t)0x00010000)
#define GPIO_PUPDR_PUPDR8_1                  ((uint32_t)0x00020000)

#define GPIO_PUPDR_PUPDR9                    ((uint32_t)0x000C0000)
#define GPIO_PUPDR_PUPDR9_0                  ((uint32_t)0x00040000)
#define GPIO_PUPDR_PUPDR9_1                  ((uint32_t)0x00080000)

#define GPIO_PUPDR_PUPDR10                   ((uint32_t)0x00300000)
#define GPIO_PUPDR_PUPDR10_0                 ((uint32_t)0x00100000)
#define GPIO_PUPDR_PUPDR10_1                 ((uint32_t)0x00200000)

#define GPIO_PUPDR_PUPDR11                   ((uint32_t)0x00C00000)
#define GPIO_PUPDR_PUPDR11_0                 ((uint32_t)0x00400000)
#define GPIO_PUPDR_PUPDR11_1                 ((uint32_t)0x00800000)

#define GPIO_PUPDR_PUPDR12                   ((uint32_t)0x03000000)
#define GPIO_PUPDR_PUPDR12_0                 ((uint32_t)0x01000000)
#define GPIO_PUPDR_PUPDR12_1                 ((uint32_t)0x02000000)

#define GPIO_PUPDR_PUPDR13                   ((uint32_t)0x0C000000)
#define GPIO_PUPDR_PUPDR13_0                 ((uint32_t)0x04000000)
#define GPIO_PUPDR_PUPDR13_1                 ((uint32_t)0x08000000)

#define GPIO_PUPDR_PUPDR14                   ((uint32_t)0x30000000)
#define GPIO_PUPDR_PUPDR14_0                 ((uint32_t)0x10000000)
#define GPIO_PUPDR_PUPDR14_1                 ((uint32_t)0x20000000)

#define GPIO_PUPDR_PUPDR15                   ((uint32_t)0xC0000000)
#define GPIO_PUPDR_PUPDR15_0                 ((uint32_t)0x40000000)
#define GPIO_PUPDR_PUPDR15_1                 ((uint32_t)0x80000000)

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_IDR_0                       ((uint32_t)0x00000001)
#define GPIO_IDR_IDR_1                       ((uint32_t)0x00000002)
#define GPIO_IDR_IDR_2                       ((uint32_t)0x00000004)
#define GPIO_IDR_IDR_3                       ((uint32_t)0x00000008)
#define GPIO_IDR_IDR_4                       ((uint32_t)0x00000010)
#define GPIO_IDR_IDR_5                       ((uint32_t)0x00000020)
#define GPIO_IDR_IDR_6                       ((uint32_t)0x00000040)
#define GPIO_IDR_IDR_7                       ((uint32_t)0x00000080)
#define GPIO_IDR_IDR_8                       ((uint32_t)0x00000100)
#define GPIO_IDR_IDR_9                       ((uint32_t)0x00000200)
#define GPIO_IDR_IDR_10                      ((uint32_t)0x00000400)
#define GPIO_IDR_IDR_11                      ((uint32_t)0x00000800)
#define GPIO_IDR_IDR_12                      ((uint32_t)0x00001000)
#define GPIO_IDR_IDR_13                      ((uint32_t)0x00002000)
#define GPIO_IDR_IDR_14                      ((uint32_t)0x00004000)
#define GPIO_IDR_IDR_15                      ((uint32_t)0x00008000)
/* Old GPIO_IDR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_IDR_0                    GPIO_IDR_IDR_0
#define GPIO_OTYPER_IDR_1                    GPIO_IDR_IDR_1
#define GPIO_OTYPER_IDR_2                    GPIO_IDR_IDR_2
#define GPIO_OTYPER_IDR_3                    GPIO_IDR_IDR_3
#define GPIO_OTYPER_IDR_4                    GPIO_IDR_IDR_4
#define GPIO_OTYPER_IDR_5                    GPIO_IDR_IDR_5
#define GPIO_OTYPER_IDR_6                    GPIO_IDR_IDR_6
#define GPIO_OTYPER_IDR_7                    GPIO_IDR_IDR_7
#define GPIO_OTYPER_IDR_8                    GPIO_IDR_IDR_8
#define GPIO_OTYPER_IDR_9                    GPIO_IDR_IDR_9
#define GPIO_OTYPER_IDR_10                   GPIO_IDR_IDR_10
#define GPIO_OTYPER_IDR_11                   GPIO_IDR_IDR_11
#define GPIO_OTYPER_IDR_12                   GPIO_IDR_IDR_12
#define GPIO_OTYPER_IDR_13                   GPIO_IDR_IDR_13
#define GPIO_OTYPER_IDR_14                   GPIO_IDR_IDR_14
#define GPIO_OTYPER_IDR_15                   GPIO_IDR_IDR_15

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_ODR_0                       ((uint32_t)0x00000001)
#define GPIO_ODR_ODR_1                       ((uint32_t)0x00000002)
#define GPIO_ODR_ODR_2                       ((uint32_t)0x00000004)
#define GPIO_ODR_ODR_3                       ((uint32_t)0x00000008)
#define GPIO_ODR_ODR_4                       ((uint32_t)0x00000010)
#define GPIO_ODR_ODR_5                       ((uint32_t)0x00000020)
#define GPIO_ODR_ODR_6                       ((uint32_t)0x00000040)
#define GPIO_ODR_ODR_7                       ((uint32_t)0x00000080)
#define GPIO_ODR_ODR_8                       ((uint32_t)0x00000100)
#define GPIO_ODR_ODR_9                       ((uint32_t)0x00000200)
#define GPIO_ODR_ODR_10                      ((uint32_t)0x00000400)
#define GPIO_ODR_ODR_11                      ((uint32_t)0x00000800)
#define GPIO_ODR_ODR_12                      ((uint32_t)0x00001000)
#define GPIO_ODR_ODR_13                      ((uint32_t)0x00002000)
#define GPIO_ODR_ODR_14                      ((uint32_t)0x00004000)
#define GPIO_ODR_ODR_15                      ((uint32_t)0x00008000)
/* Old GPIO_ODR register bits definition, maintained for legacy purpose */
#define GPIO_OTYPER_ODR_0                    GPIO_ODR_ODR_0
#define GPIO_OTYPER_ODR_1                    GPIO_ODR_ODR_1
#define GPIO_OTYPER_ODR_2                    GPIO_ODR_ODR_2
#define GPIO_OTYPER_ODR_3                    GPIO_ODR_ODR_3
#define GPIO_OTYPER_ODR_4                    GPIO_ODR_ODR_4
#define GPIO_OTYPER_ODR_5                    GPIO_ODR_ODR_5
#define GPIO_OTYPER_ODR_6                    GPIO_ODR_ODR_6
#define GPIO_OTYPER_ODR_7                    GPIO_ODR_ODR_7
#define GPIO_OTYPER_ODR_8                    GPIO_ODR_ODR_8
#define GPIO_OTYPER_ODR_9                    GPIO_ODR_ODR_9
#define GPIO_OTYPER_ODR_10                   GPIO_ODR_ODR_10
#define GPIO_OTYPER_ODR_11                   GPIO_ODR_ODR_11
#define GPIO_OTYPER_ODR_12                   GPIO_ODR_ODR_12
#define GPIO_OTYPER_ODR_13                   GPIO_ODR_ODR_13
#define GPIO_OTYPER_ODR_14                   GPIO_ODR_ODR_14
#define GPIO_OTYPER_ODR_15                   GPIO_ODR_ODR_15

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_BS_0                       ((uint32_t)0x00000001)
#define GPIO_BSRR_BS_1                       ((uint32_t)0x00000002)
#define GPIO_BSRR_BS_2                       ((uint32_t)0x00000004)
#define GPIO_BSRR_BS_3                       ((uint32_t)0x00000008)
#define GPIO_BSRR_BS_4                       ((uint32_t)0x00000010)
#define GPIO_BSRR_BS_5                       ((uint32_t)0x00000020)
#define GPIO_BSRR_BS_6                       ((uint32_t)0x00000040)
#define GPIO_BSRR_BS_7                       ((uint32_t)0x00000080)
#define GPIO_BSRR_BS_8                       ((uint32_t)0x00000100)
#define GPIO_BSRR_BS_9                       ((uint32_t)0x00000200)
#define GPIO_BSRR_BS_10                      ((uint32_t)0x00000400)
#define GPIO_BSRR_BS_11                      ((uint32_t)0x00000800)
#define GPIO_BSRR_BS_12                      ((uint32_t)0x00001000)
#define GPIO_BSRR_BS_13                      ((uint32_t)0x00002000)
#define GPIO_BSRR_BS_14                      ((uint32_t)0x00004000)
#define GPIO_BSRR_BS_15                      ((uint32_t)0x00008000)
#define GPIO_BSRR_BR_0                       ((uint32_t)0x00010000)
#define GPIO_BSRR_BR_1                       ((uint32_t)0x00020000)
#define GPIO_BSRR_BR_2                       ((uint32_t)0x00040000)
#define GPIO_BSRR_BR_3                       ((uint32_t)0x00080000)
#define GPIO_BSRR_BR_4                       ((uint32_t)0x00100000)
#define GPIO_BSRR_BR_5                       ((uint32_t)0x00200000)
#define GPIO_BSRR_BR_6                       ((uint32_t)0x00400000)
#define GPIO_BSRR_BR_7                       ((uint32_t)0x00800000)
#define GPIO_BSRR_BR_8                       ((uint32_t)0x01000000)
#define GPIO_BSRR_BR_9                       ((uint32_t)0x02000000)
#define GPIO_BSRR_BR_10                      ((uint32_t)0x04000000)
#define GPIO_BSRR_BR_11                      ((uint32_t)0x08000000)
#define GPIO_BSRR_BR_12                      ((uint32_t)0x10000000)
#define GPIO_BSRR_BR_13                      ((uint32_t)0x20000000)
#define GPIO_BSRR_BR_14                      ((uint32_t)0x40000000)
#define GPIO_BSRR_BR_15                      ((uint32_t)0x80000000)

/* define marco USE_STDPERIPH_DRIVER */
#if !defined  USE_STDPERIPH_DRIVER
#define USE_STDPERIPH_DRIVER
#endif 
#ifdef USE_STDPERIPH_DRIVER
#include "gd32f4xx_libopt.h"
#endif /* USE_STDPERIPH_DRIVER */

#ifdef __cplusplus
}
#endif
#endif 
