/*!
    \file    gd32f4xx_gpio.h
    \brief   definitions for the GPIO
    
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

#ifndef GD32F4XX_GPIO_H
#define GD32F4XX_GPIO_H

#include "gd32f4xx.h"

/** @addtogroup GPIO
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC) || \
                                    ((PERIPH) == GPIOD) || \
                                    ((PERIPH) == GPIOE) || \
                                    ((PERIPH) == GPIOF) || \
                                    ((PERIPH) == GPIOG) || \
                                    ((PERIPH) == GPIOH) || \
                                    ((PERIPH) == GPIOI) 

/** 
  * @brief  GPIO Configuration Mode enumeration 
  */   
typedef enum
{ 
  GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
  GPIO_Mode_OUT  = 0x01, /*!< GPIO Output Mode */
  GPIO_Mode_AF   = 0x02, /*!< GPIO Alternate function Mode */
  GPIO_Mode_AN   = 0x03  /*!< GPIO Analog Mode */
}GPIOMode_TypeDef;
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_IN)  || ((MODE) == GPIO_Mode_OUT) || \
                            ((MODE) == GPIO_Mode_AF)|| ((MODE) == GPIO_Mode_AN))

/** 
  * @brief  GPIO Output type enumeration 
  */  
typedef enum
{ 
  GPIO_OType_PP = 0x00,
  GPIO_OType_OD = 0x01
}GPIOOType_TypeDef;
#define IS_GPIO_OTYPE(OTYPE) (((OTYPE) == GPIO_OType_PP) || ((OTYPE) == GPIO_OType_OD))
/** 
  * @brief  GPIO Configuration PullUp PullDown enumeration 
  */ 
typedef enum
{ 
  GPIO_PuPd_NOPULL = 0x00,
  GPIO_PuPd_UP     = 0x01,
  GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
#define IS_GPIO_PUPD(PUPD) (((PUPD) == GPIO_PuPd_NOPULL) || ((PUPD) == GPIO_PuPd_UP) || \
                            ((PUPD) == GPIO_PuPd_DOWN))


/** 
  * @brief  GPIO Output Maximum frequency enumeration 
  */  
typedef enum
{ 
  GPIO_Low_Speed     = 0x00, /*!< Low speed    */
  GPIO_Medium_Speed  = 0x01, /*!< Medium speed */
  GPIO_Fast_Speed    = 0x02, /*!< Fast speed   */
  GPIO_High_Speed    = 0x03  /*!< High speed   */
}GPIOSpeed_TypeDef;

/* Add legacy definition */
#define  GPIO_Speed_2MHz    GPIO_Low_Speed    
#define  GPIO_Speed_25MHz   GPIO_Medium_Speed 
#define  GPIO_Speed_50MHz   GPIO_Fast_Speed 
#define  GPIO_Speed_100MHz  GPIO_High_Speed  
  
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Low_Speed) || ((SPEED) == GPIO_Medium_Speed) || \
                              ((SPEED) == GPIO_Fast_Speed)||  ((SPEED) == GPIO_High_Speed)) 

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */ 
typedef enum
{ 
  Bit_RESET = 0,
  Bit_SET
}BitAction;
#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))

/** 
  * @brief   GPIO Init structure definition  
  */ 
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPIO_pins_define */

  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPIOMode_TypeDef */

  GPIOSpeed_TypeDef GPIO_Speed;   /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPIOSpeed_TypeDef */

  GPIOOType_TypeDef GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPIOOType_TypeDef */

  GPIOPuPd_TypeDef GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPIOPuPd_TypeDef */
}GPIO_InitTypeDef;

/** @defgroup GPIO_pins_define 
  * @{
  */ 
#define GPIO_Pin_0                 ((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_PIN_MASK              ((uint32_t)0x0000FFFF) /* PIN mask for assert test */
#define IS_GPIO_PIN(PIN)           (((PIN) & GPIO_PIN_MASK ) != (uint32_t)0x00)
#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))
/**
  * @}
  */ 
/** @defgroup GPIO_Pin_sources 
  * @{
  */ 
#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)

#define IS_GPIO_PIN_SOURCE(PINSOURCE) (((PINSOURCE) == GPIO_PinSource0) || \
                                       ((PINSOURCE) == GPIO_PinSource1) || \
                                       ((PINSOURCE) == GPIO_PinSource2) || \
                                       ((PINSOURCE) == GPIO_PinSource3) || \
                                       ((PINSOURCE) == GPIO_PinSource4) || \
                                       ((PINSOURCE) == GPIO_PinSource5) || \
                                       ((PINSOURCE) == GPIO_PinSource6) || \
                                       ((PINSOURCE) == GPIO_PinSource7) || \
                                       ((PINSOURCE) == GPIO_PinSource8) || \
                                       ((PINSOURCE) == GPIO_PinSource9) || \
                                       ((PINSOURCE) == GPIO_PinSource10) || \
                                       ((PINSOURCE) == GPIO_PinSource11) || \
                                       ((PINSOURCE) == GPIO_PinSource12) || \
                                       ((PINSOURCE) == GPIO_PinSource13) || \
                                       ((PINSOURCE) == GPIO_PinSource14) || \
                                       ((PINSOURCE) == GPIO_PinSource15))
/**
  * @}
  */ 

/** @defgroup GPIO_Alternat_function_selection_define 
  * @{
  */ 
/** 
  * @brief   AF 0 selection  
  */ 
#define GPIO_AF_RTC_50Hz      ((uint8_t)0x00)  /* RTC_50Hz Alternate Function mapping */
#define GPIO_AF_MCO           ((uint8_t)0x00)  /* MCO (MCO1 and MCO2) Alternate Function mapping */
#define GPIO_AF_TAMPER        ((uint8_t)0x00)  /* TAMPER (TAMPER_1 and TAMPER_2) Alternate Function mapping */
#define GPIO_AF_SWJ           ((uint8_t)0x00)  /* SWJ (SWD and JTAG) Alternate Function mapping */
#define GPIO_AF_TRACE         ((uint8_t)0x00)  /* TRACE Alternate Function mapping */

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF_TIM1          ((uint8_t)0x01)  /* TIM1 Alternate Function mapping */
#define GPIO_AF_TIM2          ((uint8_t)0x01)  /* TIM2 Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF_TIM3          ((uint8_t)0x02)  /* TIM3 Alternate Function mapping */
#define GPIO_AF_TIM4          ((uint8_t)0x02)  /* TIM4 Alternate Function mapping */
#define GPIO_AF_TIM5          ((uint8_t)0x02)  /* TIM5 Alternate Function mapping */

/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF_TIM8          ((uint8_t)0x03)  /* TIM8 Alternate Function mapping */
#define GPIO_AF_TIM9          ((uint8_t)0x03)  /* TIM9 Alternate Function mapping */
#define GPIO_AF_TIM10         ((uint8_t)0x03)  /* TIM10 Alternate Function mapping */
#define GPIO_AF_TIM11         ((uint8_t)0x03)  /* TIM11 Alternate Function mapping */

/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF_I2C0          ((uint8_t)0x04)  /* I2C1 Alternate Function mapping */
#define GPIO_AF_I2C1          ((uint8_t)0x04)  /* I2C2 Alternate Function mapping */
#define GPIO_AF_I2C2          ((uint8_t)0x04)  /* I2C3 Alternate Function mapping */

/** 
  * @brief   AF 5 selection  
  */ 
#define GPIO_AF_SPI1          ((uint8_t)0x05)  /* SPI1/I2S1 Alternate Function mapping */
#define GPIO_AF_SPI2          ((uint8_t)0x05)  /* SPI2/I2S2 Alternate Function mapping */
#define GPIO_AF5_SPI3         ((uint8_t)0x05)  /* SPI3/I2S3 Alternate Function mapping (Only for STM32F411xE Devices) */
#define GPIO_AF_SPI4          ((uint8_t)0x05)  /* SPI4/I2S4 Alternate Function mapping */
#define GPIO_AF_SPI5          ((uint8_t)0x05)  /* SPI5 Alternate Function mapping      */
#define GPIO_AF_SPI6          ((uint8_t)0x05)  /* SPI6 Alternate Function mapping      */

/** 
  * @brief   AF 6 selection  
  */ 
#define GPIO_AF_SPI3          ((uint8_t)0x06)  /* SPI3/I2S3 Alternate Function mapping */
#define GPIO_AF6_SPI2         ((uint8_t)0x06)  /* SPI2 Alternate Function mapping (Only for STM32F411xE Devices) */
#define GPIO_AF6_SPI4         ((uint8_t)0x06)  /* SPI4 Alternate Function mapping (Only for STM32F411xE Devices) */
#define GPIO_AF6_SPI5         ((uint8_t)0x06)  /* SPI5 Alternate Function mapping (Only for STM32F411xE Devices) */
#define GPIO_AF_SAI1          ((uint8_t)0x06)  /* SAI1 Alternate Function mapping      */

/** 
  * @brief   AF 7 selection  
  */ 
#define GPIO_AF_USART1         ((uint8_t)0x07)  /* USART1 Alternate Function mapping  */
#define GPIO_AF_USART2         ((uint8_t)0x07)  /* USART2 Alternate Function mapping  */
#define GPIO_AF_USART3         ((uint8_t)0x07)  /* USART3 Alternate Function mapping  */
#define GPIO_AF7_SPI3          ((uint8_t)0x07)  /* SPI3/I2S3ext Alternate Function mapping */

/** 
  * @brief   AF 7 selection Legacy 
  */ 
#define GPIO_AF_I2S3ext   GPIO_AF7_SPI3

/** 
  * @brief   AF 8 selection  
  */ 
#define GPIO_AF_UART4         ((uint8_t)0x08)  /* UART4 Alternate Function mapping  */
#define GPIO_AF_UART5         ((uint8_t)0x08)  /* UART5 Alternate Function mapping  */
#define GPIO_AF_USART6        ((uint8_t)0x08)  /* USART6 Alternate Function mapping */
#define GPIO_AF_UART7         ((uint8_t)0x08)  /* UART7 Alternate Function mapping  */
#define GPIO_AF_UART8         ((uint8_t)0x08)  /* UART8 Alternate Function mapping  */

/** 
  * @brief   AF 9 selection 
  */ 
#define GPIO_AF_CAN1          ((uint8_t)0x09)  /* CAN1 Alternate Function mapping  */
#define GPIO_AF_CAN2          ((uint8_t)0x09)  /* CAN2 Alternate Function mapping  */
#define GPIO_AF_TIM12         ((uint8_t)0x09)  /* TIM12 Alternate Function mapping */
#define GPIO_AF_TIM13         ((uint8_t)0x09)  /* TIM13 Alternate Function mapping */
#define GPIO_AF_TIM14         ((uint8_t)0x09)  /* TIM14 Alternate Function mapping */

#define GPIO_AF9_I2C2         ((uint8_t)0x09)  /* I2C2 Alternate Function mapping (Only for STM32F401xx/STM32F411xE Devices) */
#define GPIO_AF9_I2C3         ((uint8_t)0x09)  /* I2C3 Alternate Function mapping (Only for STM32F401xx/STM32F411xE Devices) */

/** 
  * @brief   AF 10 selection  
  */ 
#define GPIO_AF_OTG_FS         ((uint8_t)0xA)  /* OTG_FS Alternate Function mapping */
#define GPIO_AF_OTG_HS         ((uint8_t)0xA)  /* OTG_HS Alternate Function mapping */

/** 
  * @brief   AF 11 selection  
  */ 
#define GPIO_AF_ETH             ((uint8_t)0x0B)  /* ETHERNET Alternate Function mapping */

/** 
  * @brief   AF 12 selection  
  */ 
#if defined (GD32F407)
#define GPIO_AF_FSMC             ((uint8_t)0xC)  /* FSMC Alternate Function mapping                     */
#endif /* STM32F40_41xxx */

#if defined (STM32F427_437xx) || defined (STM32F429_439xx)
#define GPIO_AF_FMC              ((uint8_t)0xC)  /* FMC Alternate Function mapping                      */
#endif /* STM32F427_437xx ||  STM32F429_439xx */

#define GPIO_AF_OTG_HS_FS        ((uint8_t)0xC)  /* OTG HS configured in FS, Alternate Function mapping */
#define GPIO_AF_SDIO             ((uint8_t)0xC)  /* SDIO Alternate Function mapping                     */

/** 
  * @brief   AF 13 selection  
  */ 
#define GPIO_AF_DCMI          ((uint8_t)0x0D)  /* DCMI Alternate Function mapping */

/** 
  * @brief   AF 14 selection  
  */

#define GPIO_AF_LTDC          ((uint8_t)0x0E)  /* LCD-TFT Alternate Function mapping */

/** 
  * @brief   AF 15 selection  
  */ 
#define GPIO_AF_EVENTOUT      ((uint8_t)0x0F)  /* EVENTOUT Alternate Function mapping */

#if defined (GD32F407)
#define IS_GPIO_AF(AF)   (((AF) == GPIO_AF_RTC_50Hz)  || ((AF) == GPIO_AF_TIM14)     || \
                          ((AF) == GPIO_AF_MCO)       || ((AF) == GPIO_AF_TAMPER)    || \
                          ((AF) == GPIO_AF_SWJ)       || ((AF) == GPIO_AF_TRACE)     || \
                          ((AF) == GPIO_AF_TIM1)      || ((AF) == GPIO_AF_TIM2)      || \
                          ((AF) == GPIO_AF_TIM3)      || ((AF) == GPIO_AF_TIM4)      || \
                          ((AF) == GPIO_AF_TIM5)      || ((AF) == GPIO_AF_TIM8)      || \
                          ((AF) == GPIO_AF_I2C1)      || ((AF) == GPIO_AF_I2C2)      || \
                          ((AF) == GPIO_AF_I2C3)      || ((AF) == GPIO_AF_SPI1)      || \
                          ((AF) == GPIO_AF_SPI2)      || ((AF) == GPIO_AF_TIM13)     || \
                          ((AF) == GPIO_AF_SPI3)      || ((AF) == GPIO_AF_TIM14)     || \
                          ((AF) == GPIO_AF_USART1)    || ((AF) == GPIO_AF_USART2)    || \
                          ((AF) == GPIO_AF_USART3)    || ((AF) == GPIO_AF_UART4)     || \
                          ((AF) == GPIO_AF_UART5)     || ((AF) == GPIO_AF_USART6)    || \
                          ((AF) == GPIO_AF_CAN1)      || ((AF) == GPIO_AF_CAN2)      || \
                          ((AF) == GPIO_AF_OTG_FS)    || ((AF) == GPIO_AF_OTG_HS)    || \
                          ((AF) == GPIO_AF_ETH)       || ((AF) == GPIO_AF_OTG_HS_FS) || \
                          ((AF) == GPIO_AF_SDIO)      || ((AF) == GPIO_AF_DCMI)      || \
                          ((AF) == GPIO_AF_EVENTOUT)  || ((AF) == GPIO_AF_FSMC))
#endif /* STM32F401xx */													
/* registers definitions */
#define GPIO_CTL(gpiox)            REG32((gpiox) + 0x00U)    /*!< GPIO port control register */
#define GPIO_OMODE(gpiox)          REG32((gpiox) + 0x04U)    /*!< GPIO port output mode register */
#define GPIO_OSPD(gpiox)           REG32((gpiox) + 0x08U)    /*!< GPIO port output speed register */
#define GPIO_PUD(gpiox)            REG32((gpiox) + 0x0CU)    /*!< GPIO port pull-up/pull-down register */
#define GPIO_ISTAT(gpiox)          REG32((gpiox) + 0x10U)    /*!< GPIO port input status register */
#define GPIO_OCTL(gpiox)           REG32((gpiox) + 0x14U)    /*!< GPIO port output control register */
#define GPIO_BOP(gpiox)            REG32((gpiox) + 0x18U)    /*!< GPIO port bit operation register */
#define GPIO_LOCK(gpiox)           REG32((gpiox) + 0x1CU)    /*!< GPIO port configuration lock register */
#define GPIO_AFSEL0(gpiox)         REG32((gpiox) + 0x20U)    /*!< GPIO alternate function selected register 0 */
#define GPIO_AFSEL1(gpiox)         REG32((gpiox) + 0x24U)    /*!< GPIO alternate function selected register 1 */
#define GPIO_BC(gpiox)             REG32((gpiox) + 0x28U)    /*!< GPIO bit clear register */
#define GPIO_TG(gpiox)             REG32((gpiox) + 0x2CU)    /*!< GPIO port bit toggle register */

/* bits definitions */
/* GPIO_CTL */
#define GPIO_CTL_CTL0              BITS(0,1)                 /*!< pin 0 configuration bits */ 
#define GPIO_CTL_CTL1              BITS(2,3)                 /*!< pin 1 configuration bits */
#define GPIO_CTL_CTL2              BITS(4,5)                 /*!< pin 2 configuration bits */
#define GPIO_CTL_CTL3              BITS(6,7)                 /*!< pin 3 configuration bits */
#define GPIO_CTL_CTL4              BITS(8,9)                 /*!< pin 4 configuration bits */
#define GPIO_CTL_CTL5              BITS(10,11)               /*!< pin 5 configuration bits */
#define GPIO_CTL_CTL6              BITS(12,13)               /*!< pin 6 configuration bits */
#define GPIO_CTL_CTL7              BITS(14,15)               /*!< pin 7 configuration bits */
#define GPIO_CTL_CTL8              BITS(16,17)               /*!< pin 8 configuration bits */
#define GPIO_CTL_CTL9              BITS(18,19)               /*!< pin 9 configuration bits */
#define GPIO_CTL_CTL10             BITS(20,21)               /*!< pin 10 configuration bits */
#define GPIO_CTL_CTL11             BITS(22,23)               /*!< pin 11 configuration bits */
#define GPIO_CTL_CTL12             BITS(24,25)               /*!< pin 12 configuration bits */
#define GPIO_CTL_CTL13             BITS(26,27)               /*!< pin 13 configuration bits */
#define GPIO_CTL_CTL14             BITS(28,29)               /*!< pin 14 configuration bits */
#define GPIO_CTL_CTL15             BITS(30,31)               /*!< pin 15 configuration bits */

/* GPIO_OMODE */
#define GPIO_OMODE_OM0             BIT(0)                    /*!< pin 0 output mode bit */
#define GPIO_OMODE_OM1             BIT(1)                    /*!< pin 1 output mode bit */
#define GPIO_OMODE_OM2             BIT(2)                    /*!< pin 2 output mode bit */
#define GPIO_OMODE_OM3             BIT(3)                    /*!< pin 3 output mode bit */
#define GPIO_OMODE_OM4             BIT(4)                    /*!< pin 4 output mode bit */
#define GPIO_OMODE_OM5             BIT(5)                    /*!< pin 5 output mode bit */
#define GPIO_OMODE_OM6             BIT(6)                    /*!< pin 6 output mode bit */
#define GPIO_OMODE_OM7             BIT(7)                    /*!< pin 7 output mode bit */
#define GPIO_OMODE_OM8             BIT(8)                    /*!< pin 8 output mode bit */
#define GPIO_OMODE_OM9             BIT(9)                    /*!< pin 9 output mode bit */
#define GPIO_OMODE_OM10            BIT(10)                   /*!< pin 10 output mode bit */
#define GPIO_OMODE_OM11            BIT(11)                   /*!< pin 11 output mode bit */
#define GPIO_OMODE_OM12            BIT(12)                   /*!< pin 12 output mode bit */
#define GPIO_OMODE_OM13            BIT(13)                   /*!< pin 13 output mode bit */
#define GPIO_OMODE_OM14            BIT(14)                   /*!< pin 14 output mode bit */
#define GPIO_OMODE_OM15            BIT(15)                   /*!< pin 15 output mode bit */

/* GPIO_OSPD */
#define GPIO_OSPD_OSPD0            BITS(0,1)                 /*!< pin 0 output max speed bits */
#define GPIO_OSPD_OSPD1            BITS(2,3)                 /*!< pin 1 output max speed bits */
#define GPIO_OSPD_OSPD2            BITS(4,5)                 /*!< pin 2 output max speed bits */
#define GPIO_OSPD_OSPD3            BITS(6,7)                 /*!< pin 3 output max speed bits */
#define GPIO_OSPD_OSPD4            BITS(8,9)                 /*!< pin 4 output max speed bits */
#define GPIO_OSPD_OSPD5            BITS(10,11)               /*!< pin 5 output max speed bits */
#define GPIO_OSPD_OSPD6            BITS(12,13)               /*!< pin 6 output max speed bits */
#define GPIO_OSPD_OSPD7            BITS(14,15)               /*!< pin 7 output max speed bits */
#define GPIO_OSPD_OSPD8            BITS(16,17)               /*!< pin 8 output max speed bits */
#define GPIO_OSPD_OSPD9            BITS(18,19)               /*!< pin 9 output max speed bits */
#define GPIO_OSPD_OSPD10           BITS(20,21)               /*!< pin 10 output max speed bits */
#define GPIO_OSPD_OSPD11           BITS(22,23)               /*!< pin 11 output max speed bits */
#define GPIO_OSPD_OSPD12           BITS(24,25)               /*!< pin 12 output max speed bits */
#define GPIO_OSPD_OSPD13           BITS(26,27)               /*!< pin 13 output max speed bits */
#define GPIO_OSPD_OSPD14           BITS(28,29)               /*!< pin 14 output max speed bits */
#define GPIO_OSPD_OSPD15           BITS(30,31)               /*!< pin 15 output max speed bits */

/* GPIO_PUD */
#define GPIO_PUD_PUD0              BITS(0,1)                 /*!< pin 0 pull-up or pull-down bits */
#define GPIO_PUD_PUD1              BITS(2,3)                 /*!< pin 1 pull-up or pull-down bits */
#define GPIO_PUD_PUD2              BITS(4,5)                 /*!< pin 2 pull-up or pull-down bits */
#define GPIO_PUD_PUD3              BITS(6,7)                 /*!< pin 3 pull-up or pull-down bits */
#define GPIO_PUD_PUD4              BITS(8,9)                 /*!< pin 4 pull-up or pull-down bits */
#define GPIO_PUD_PUD5              BITS(10,11)               /*!< pin 5 pull-up or pull-down bits */
#define GPIO_PUD_PUD6              BITS(12,13)               /*!< pin 6 pull-up or pull-down bits */
#define GPIO_PUD_PUD7              BITS(14,15)               /*!< pin 7 pull-up or pull-down bits */
#define GPIO_PUD_PUD8              BITS(16,17)               /*!< pin 8 pull-up or pull-down bits */
#define GPIO_PUD_PUD9              BITS(18,19)               /*!< pin 9 pull-up or pull-down bits */
#define GPIO_PUD_PUD10             BITS(20,21)               /*!< pin 10 pull-up or pull-down bits */
#define GPIO_PUD_PUD11             BITS(22,23)               /*!< pin 11 pull-up or pull-down bits */
#define GPIO_PUD_PUD12             BITS(24,25)               /*!< pin 12 pull-up or pull-down bits */
#define GPIO_PUD_PUD13             BITS(26,27)               /*!< pin 13 pull-up or pull-down bits */
#define GPIO_PUD_PUD14             BITS(28,29)               /*!< pin 14 pull-up or pull-down bits */
#define GPIO_PUD_PUD15             BITS(30,31)               /*!< pin 15 pull-up or pull-down bits */

/* GPIO_ISTAT */
#define GPIO_ISTAT_ISTAT0          BIT(0)                    /*!< pin 0 input status */
#define GPIO_ISTAT_ISTAT1          BIT(1)                    /*!< pin 1 input status */
#define GPIO_ISTAT_ISTAT2          BIT(2)                    /*!< pin 2 input status */
#define GPIO_ISTAT_ISTAT3          BIT(3)                    /*!< pin 3 input status */
#define GPIO_ISTAT_ISTAT4          BIT(4)                    /*!< pin 4 input status */
#define GPIO_ISTAT_ISTAT5          BIT(5)                    /*!< pin 5 input status */
#define GPIO_ISTAT_ISTAT6          BIT(6)                    /*!< pin 6 input status */
#define GPIO_ISTAT_ISTAT7          BIT(7)                    /*!< pin 7 input status */
#define GPIO_ISTAT_ISTAT8          BIT(8)                    /*!< pin 8 input status */
#define GPIO_ISTAT_ISTAT9          BIT(9)                    /*!< pin 9 input status */
#define GPIO_ISTAT_ISTAT10         BIT(10)                   /*!< pin 10 input status */
#define GPIO_ISTAT_ISTAT11         BIT(11)                   /*!< pin 11 input status */
#define GPIO_ISTAT_ISTAT12         BIT(12)                   /*!< pin 12 input status */
#define GPIO_ISTAT_ISTAT13         BIT(13)                   /*!< pin 13 input status */
#define GPIO_ISTAT_ISTAT14         BIT(14)                   /*!< pin 14 input status */
#define GPIO_ISTAT_ISTAT15         BIT(15)                   /*!< pin 15 input status */

/* GPIO_OCTL */
#define GPIO_OCTL_OCTL0            BIT(0)                    /*!< pin 0 output bit */
#define GPIO_OCTL_OCTL1            BIT(1)                    /*!< pin 1 output bit */
#define GPIO_OCTL_OCTL2            BIT(2)                    /*!< pin 2 output bit */
#define GPIO_OCTL_OCTL3            BIT(3)                    /*!< pin 3 output bit */
#define GPIO_OCTL_OCTL4            BIT(4)                    /*!< pin 4 output bit */
#define GPIO_OCTL_OCTL5            BIT(5)                    /*!< pin 5 output bit */
#define GPIO_OCTL_OCTL6            BIT(6)                    /*!< pin 6 output bit */
#define GPIO_OCTL_OCTL7            BIT(7)                    /*!< pin 7 output bit */
#define GPIO_OCTL_OCTL8            BIT(8)                    /*!< pin 8 output bit */
#define GPIO_OCTL_OCTL9            BIT(9)                    /*!< pin 9 output bit */
#define GPIO_OCTL_OCTL10           BIT(10)                   /*!< pin 10 output bit */
#define GPIO_OCTL_OCTL11           BIT(11)                   /*!< pin 11 output bit */
#define GPIO_OCTL_OCTL12           BIT(12)                   /*!< pin 12 output bit */
#define GPIO_OCTL_OCTL13           BIT(13)                   /*!< pin 13 output bit */
#define GPIO_OCTL_OCTL14           BIT(14)                   /*!< pin 14 output bit */
#define GPIO_OCTL_OCTL15           BIT(15)                   /*!< pin 15 output bit */

/* GPIO_BOP */
#define GPIO_BOP_BOP0              BIT(0)                    /*!< pin 0 set bit */
#define GPIO_BOP_BOP1              BIT(1)                    /*!< pin 1 set bit */
#define GPIO_BOP_BOP2              BIT(2)                    /*!< pin 2 set bit */
#define GPIO_BOP_BOP3              BIT(3)                    /*!< pin 3 set bit */
#define GPIO_BOP_BOP4              BIT(4)                    /*!< pin 4 set bit */
#define GPIO_BOP_BOP5              BIT(5)                    /*!< pin 5 set bit */
#define GPIO_BOP_BOP6              BIT(6)                    /*!< pin 6 set bit */
#define GPIO_BOP_BOP7              BIT(7)                    /*!< pin 7 set bit */
#define GPIO_BOP_BOP8              BIT(8)                    /*!< pin 8 set bit */
#define GPIO_BOP_BOP9              BIT(9)                    /*!< pin 9 set bit */
#define GPIO_BOP_BOP10             BIT(10)                   /*!< pin 10 set bit */
#define GPIO_BOP_BOP11             BIT(11)                   /*!< pin 11 set bit */
#define GPIO_BOP_BOP12             BIT(12)                   /*!< pin 12 set bit */
#define GPIO_BOP_BOP13             BIT(13)                   /*!< pin 13 set bit */
#define GPIO_BOP_BOP14             BIT(14)                   /*!< pin 14 set bit */
#define GPIO_BOP_BOP15             BIT(15)                   /*!< pin 15 set bit */
#define GPIO_BOP_CR0               BIT(16)                   /*!< pin 0 clear bit */
#define GPIO_BOP_CR1               BIT(17)                   /*!< pin 1 clear bit */
#define GPIO_BOP_CR2               BIT(18)                   /*!< pin 2 clear bit */
#define GPIO_BOP_CR3               BIT(19)                   /*!< pin 3 clear bit */
#define GPIO_BOP_CR4               BIT(20)                   /*!< pin 4 clear bit */
#define GPIO_BOP_CR5               BIT(21)                   /*!< pin 5 clear bit */
#define GPIO_BOP_CR6               BIT(22)                   /*!< pin 6 clear bit */
#define GPIO_BOP_CR7               BIT(23)                   /*!< pin 7 clear bit */
#define GPIO_BOP_CR8               BIT(24)                   /*!< pin 8 clear bit */
#define GPIO_BOP_CR9               BIT(25)                   /*!< pin 9 clear bit */
#define GPIO_BOP_CR10              BIT(26)                   /*!< pin 10 clear bit */
#define GPIO_BOP_CR11              BIT(27)                   /*!< pin 11 clear bit */
#define GPIO_BOP_CR12              BIT(28)                   /*!< pin 12 clear bit */
#define GPIO_BOP_CR13              BIT(29)                   /*!< pin 13 clear bit */
#define GPIO_BOP_CR14              BIT(30)                   /*!< pin 14 clear bit */
#define GPIO_BOP_CR15              BIT(31)                   /*!< pin 15 clear bit */

/* GPIO_LOCK */
#define GPIO_LOCK_LK0              BIT(0)                    /*!< pin 0 lock bit */
#define GPIO_LOCK_LK1              BIT(1)                    /*!< pin 1 lock bit */
#define GPIO_LOCK_LK2              BIT(2)                    /*!< pin 2 lock bit */
#define GPIO_LOCK_LK3              BIT(3)                    /*!< pin 3 lock bit */
#define GPIO_LOCK_LK4              BIT(4)                    /*!< pin 4 lock bit */
#define GPIO_LOCK_LK5              BIT(5)                    /*!< pin 5 lock bit */
#define GPIO_LOCK_LK6              BIT(6)                    /*!< pin 6 lock bit */
#define GPIO_LOCK_LK7              BIT(7)                    /*!< pin 7 lock bit */
#define GPIO_LOCK_LK8              BIT(8)                    /*!< pin 8 lock bit */
#define GPIO_LOCK_LK9              BIT(9)                    /*!< pin 9 lock bit */
#define GPIO_LOCK_LK10             BIT(10)                   /*!< pin 10 lock bit */
#define GPIO_LOCK_LK11             BIT(11)                   /*!< pin 11 lock bit */
#define GPIO_LOCK_LK12             BIT(12)                   /*!< pin 12 lock bit */
#define GPIO_LOCK_LK13             BIT(13)                   /*!< pin 13 lock bit */
#define GPIO_LOCK_LK14             BIT(14)                   /*!< pin 14 lock bit */
#define GPIO_LOCK_LK15             BIT(15)                   /*!< pin 15 lock bit */
#define GPIO_LOCK_LKK              BIT(16)                   /*!< pin sequence lock key */

/* GPIO_AFSEL0 */
#define GPIO_AFSEL0_SEL0           BITS(0,3)                 /*!< pin 0 alternate function selected */
#define GPIO_AFSEL0_SEL1           BITS(4,7)                 /*!< pin 1 alternate function selected */
#define GPIO_AFSEL0_SEL2           BITS(8,11)                /*!< pin 2 alternate function selected */
#define GPIO_AFSEL0_SEL3           BITS(12,15)               /*!< pin 3 alternate function selected */
#define GPIO_AFSEL0_SEL4           BITS(16,19)               /*!< pin 4 alternate function selected */
#define GPIO_AFSEL0_SEL5           BITS(20,23)               /*!< pin 5 alternate function selected */
#define GPIO_AFSEL0_SEL6           BITS(24,27)               /*!< pin 6 alternate function selected */
#define GPIO_AFSEL0_SEL7           BITS(28,31)               /*!< pin 7 alternate function selected */

/* GPIO_AFSEL1 */
#define GPIO_AFSEL1_SEL8           BITS(0,3)                 /*!< pin 8 alternate function selected */
#define GPIO_AFSEL1_SEL9           BITS(4,7)                 /*!< pin 9 alternate function selected */
#define GPIO_AFSEL1_SEL10          BITS(8,11)                /*!< pin 10 alternate function selected */
#define GPIO_AFSEL1_SEL11          BITS(12,15)               /*!< pin 11 alternate function selected */
#define GPIO_AFSEL1_SEL12          BITS(16,19)               /*!< pin 12 alternate function selected */
#define GPIO_AFSEL1_SEL13          BITS(20,23)               /*!< pin 13 alternate function selected */
#define GPIO_AFSEL1_SEL14          BITS(24,27)               /*!< pin 14 alternate function selected */
#define GPIO_AFSEL1_SEL15          BITS(28,31)               /*!< pin 15 alternate function selected */

/* GPIO_BC */
#define GPIO_BC_CR0                BIT(0)                    /*!< pin 0 clear bit */
#define GPIO_BC_CR1                BIT(1)                    /*!< pin 1 clear bit */
#define GPIO_BC_CR2                BIT(2)                    /*!< pin 2 clear bit */
#define GPIO_BC_CR3                BIT(3)                    /*!< pin 3 clear bit */
#define GPIO_BC_CR4                BIT(4)                    /*!< pin 4 clear bit */
#define GPIO_BC_CR5                BIT(5)                    /*!< pin 5 clear bit */
#define GPIO_BC_CR6                BIT(6)                    /*!< pin 6 clear bit */
#define GPIO_BC_CR7                BIT(7)                    /*!< pin 7 clear bit */
#define GPIO_BC_CR8                BIT(8)                    /*!< pin 8 clear bit */
#define GPIO_BC_CR9                BIT(9)                    /*!< pin 9 clear bit */
#define GPIO_BC_CR10               BIT(10)                   /*!< pin 10 clear bit */
#define GPIO_BC_CR11               BIT(11)                   /*!< pin 11 clear bit */
#define GPIO_BC_CR12               BIT(12)                   /*!< pin 12 clear bit */
#define GPIO_BC_CR13               BIT(13)                   /*!< pin 13 clear bit */
#define GPIO_BC_CR14               BIT(14)                   /*!< pin 14 clear bit */
#define GPIO_BC_CR15               BIT(15)                   /*!< pin 15 clear bit */

/* GPIO_TG */
#define GPIO_TG_TG0                BIT(0)                    /*!< pin 0 toggle bit */
#define GPIO_TG_TG1                BIT(1)                    /*!< pin 1 toggle bit */
#define GPIO_TG_TG2                BIT(2)                    /*!< pin 2 toggle bit */
#define GPIO_TG_TG3                BIT(3)                    /*!< pin 3 toggle bit */
#define GPIO_TG_TG4                BIT(4)                    /*!< pin 4 toggle bit */
#define GPIO_TG_TG5                BIT(5)                    /*!< pin 5 toggle bit */
#define GPIO_TG_TG6                BIT(6)                    /*!< pin 6 toggle bit */
#define GPIO_TG_TG7                BIT(7)                    /*!< pin 7 toggle bit */
#define GPIO_TG_TG8                BIT(8)                    /*!< pin 8 toggle bit */
#define GPIO_TG_TG9                BIT(9)                    /*!< pin 9 toggle bit */
#define GPIO_TG_TG10               BIT(10)                   /*!< pin 10 toggle bit */
#define GPIO_TG_TG11               BIT(11)                   /*!< pin 11 toggle bit */
#define GPIO_TG_TG12               BIT(12)                   /*!< pin 12 toggle bit */
#define GPIO_TG_TG13               BIT(13)                   /*!< pin 13 toggle bit */
#define GPIO_TG_TG14               BIT(14)                   /*!< pin 14 toggle bit */
#define GPIO_TG_TG15               BIT(15)                   /*!< pin 15 toggle bit */

/* function declarations */
/* reset GPIO port */
void GPIO_DeInit(GPIO_TypeDef* GPIOx);
/* Initialization and Configuration functions *********************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
/* GPIO Alternate functions configuration function ****************************/
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);


#endif /* GD32F4XX_GPIO_H */
