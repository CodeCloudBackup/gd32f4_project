/*!
    \file    gd32f4xx_rcu.h
    \brief   definitions for the RCU
    
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

#ifndef GD32F4XX_RCU_H
#define GD32F4XX_RCU_H

#include "gd32f4xx.h"


/* define startup timeout count */
#define OSC_STARTUP_TIMEOUT         ((uint32_t)0x000fffffU)
#define LXTAL_STARTUP_TIMEOUT       ((uint32_t)0x0fffffffU)
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint32_t SYSCLK_Frequency; /*!<  SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;   /*!<  HCLK clock frequency expressed in Hz   */
  uint32_t PCLK1_Frequency;  /*!<  PCLK1 clock frequency expressed in Hz  */
  uint32_t PCLK2_Frequency;  /*!<  PCLK2 clock frequency expressed in Hz  */
}RCU_ClocksTypeDef;

/* registers definitions */
#define RCU_ADDCTL                      REG32(RCU_BASE + 0xC0U)        /*!< Additional clock control register */
#define RCU_ADDINT                      REG32(RCU_BASE + 0xCCU)        /*!< Additional clock interrupt register */
#define RCU_ADDAPB1RST                  REG32(RCU_BASE + 0xE0U)        /*!< APB1 additional reset register */
#define RCU_ADDAPB1EN                   REG32(RCU_BASE + 0xE4U)        /*!< APB1 additional enable register */
#define RCU_ADDAPB1SPEN                 REG32(RCU_BASE + 0xE8U)        /*!< APB1 additional sleep mode enable register */
#define RCU_VKEY                        REG32(RCU_BASE + 0x100U)       /*!< voltage key register */
#define RCU_DSV                         REG32(RCU_BASE + 0x134U)       /*!< deep-sleep mode voltage register */

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

/* RCU_INT */
#define RCU_INT_IRC32KSTBIF             BIT(0)                    /*!< IRC32K stabilization interrupt flag */
#define RCU_INT_LXTALSTBIF              BIT(1)                    /*!< LXTAL stabilization interrupt flag */
#define RCU_INT_IRC16MSTBIF             BIT(2)                    /*!< IRC16M stabilization interrupt flag */
#define RCU_INT_HXTALSTBIF              BIT(3)                    /*!< HXTAL stabilization interrupt flag */
#define RCU_INT_PLLSTBIF                BIT(4)                    /*!< PLL stabilization interrupt flag */
#define RCU_INT_PLLI2SSTBIF             BIT(5)                    /*!< PLLI2S stabilization interrupt flag */
#define RCU_INT_PLLSAISTBIF             BIT(6)                    /*!< PLLSAI stabilization interrupt flag */
#define RCU_INT_CKMIF                   BIT(7)                    /*!< HXTAL clock stuck interrupt flag */
#define RCU_INT_IRC32KSTBIE             BIT(8)                    /*!< IRC32K stabilization interrupt enable */
#define RCU_INT_LXTALSTBIE              BIT(9)                    /*!< LXTAL stabilization interrupt enable */
#define RCU_INT_IRC16MSTBIE             BIT(10)                   /*!< IRC16M stabilization interrupt enable */
#define RCU_INT_HXTALSTBIE              BIT(11)                   /*!< HXTAL stabilization interrupt enable */
#define RCU_INT_PLLSTBIE                BIT(12)                   /*!< PLL stabilization interrupt enable */
#define RCU_INT_PLLI2SSTBIE             BIT(13)                   /*!< PLLI2S Stabilization Interrupt Enable */
#define RCU_INT_PLLSAISTBIE             BIT(14)                   /*!< PLLSAI Stabilization Interrupt Enable */
#define RCU_INT_IRC32KSTBIC             BIT(16)                   /*!< IRC32K Stabilization Interrupt Clear */
#define RCU_INT_LXTALSTBIC              BIT(17)                   /*!< LXTAL Stabilization Interrupt Clear */
#define RCU_INT_IRC16MSTBIC             BIT(18)                   /*!< IRC16M Stabilization Interrupt Clear */
#define RCU_INT_HXTALSTBIC              BIT(19)                   /*!< HXTAL Stabilization Interrupt Clear */
#define RCU_INT_PLLSTBIC                BIT(20)                   /*!< PLL stabilization Interrupt Clear */
#define RCU_INT_PLLI2SSTBIC             BIT(21)                   /*!< PLLI2S stabilization Interrupt Clear */
#define RCU_INT_PLLSAISTBIC             BIT(22)                   /*!< PLLSAI stabilization Interrupt Clear */
#define RCU_INT_CKMIC                   BIT(23)                   /*!< HXTAL Clock Stuck Interrupt Clear */


/* RCU_AHB1SPEN */
#define RCU_AHB1SPEN_PASPEN             BIT(0)                    /*!< GPIO port A clock enable when sleep mode */
#define RCU_AHB1SPEN_PBSPEN             BIT(1)                    /*!< GPIO port B clock enable when sleep mode */
#define RCU_AHB1SPEN_PCSPEN             BIT(2)                    /*!< GPIO port C clock enable when sleep mode */
#define RCU_AHB1SPEN_PDSPEN             BIT(3)                    /*!< GPIO port D clock enable when sleep mode */
#define RCU_AHB1SPEN_PESPEN             BIT(4)                    /*!< GPIO port E clock enable when sleep mode */
#define RCU_AHB1SPEN_PFSPEN             BIT(5)                    /*!< GPIO port F clock enable when sleep mode */
#define RCU_AHB1SPEN_PGSPEN             BIT(6)                    /*!< GPIO port G clock enable when sleep mode */
#define RCU_AHB1SPEN_PHSPEN             BIT(7)                    /*!< GPIO port H clock enable when sleep mode */
#define RCU_AHB1SPEN_PISPEN             BIT(8)                    /*!< GPIO port I clock enable when sleep mode */
#define RCU_AHB1SPEN_CRCSPEN            BIT(12)                   /*!< CRC clock enable when sleep mode */
#define RCU_AHB1SPEN_FMCSPEN            BIT(15)                   /*!< FMC clock enable when sleep mode */
#define RCU_AHB1SPEN_SRAM0SPEN          BIT(16)                   /*!< SRAM0 clock enable when sleep mode */
#define RCU_AHB1SPEN_SRAM1SPEN          BIT(17)                   /*!< SRAM1 clock enable when sleep mode */
#define RCU_AHB1SPEN_BKPSRAMSPEN        BIT(18)                   /*!< BKPSRAM clock enable when sleep mode */
#define RCU_AHB1SPEN_SRAM2SPEN          BIT(19)                   /*!< SRAM2 clock enable when sleep mode */
#define RCU_AHB1SPEN_DMA0SPEN           BIT(21)                   /*!< DMA0 clock when sleep mode enable */
#define RCU_AHB1SPEN_DMA1SPEN           BIT(22)                   /*!< DMA1 clock when sleep mode enable */
#define RCU_AHB1SPEN_IPASPEN            BIT(23)                   /*!< IPA clock enable when sleep mode */
#define RCU_AHB1SPEN_ENETSPEN           BIT(25)                   /*!< ENET clock enable when sleep mode */
#define RCU_AHB1SPEN_ENETTXSPEN         BIT(26)                   /*!< Ethernet TX clock enable when sleep mode */
#define RCU_AHB1SPEN_ENETRXSPEN         BIT(27)                   /*!< Ethernet RX clock enable when sleep mode */
#define RCU_AHB1SPEN_ENETPTPSPEN        BIT(28)                   /*!< Ethernet PTP clock enable when sleep mode */
#define RCU_AHB1SPEN_USBHSSPEN          BIT(29)                   /*!< USBHS clock enable when sleep mode */
#define RCU_AHB1SPEN_USBHSULPISPEN      BIT(30)                   /*!< USBHS ULPI clock enable when sleep mode */

/* RCU_AHB2SPEN */
#define RCU_AHB2SPEN_DCISPEN            BIT(0)                    /*!< DCI clock enable when sleep mode */
#define RCU_AHB2SPEN_TRNGSPEN           BIT(6)                    /*!< TRNG clock enable when sleep mode */
#define RCU_AHB2SPEN_USBFSSPEN          BIT(7)                    /*!< USBFS clock enable when sleep mode */

/* RCU_AHB3SPEN */
#define RCU_AHB3SPEN_EXMCSPEN           BIT(0)                    /*!< EXMC clock enable when sleep mode */

/* RCU_APB1SPEN */
#define RCU_APB1SPEN_TIMER1SPEN         BIT(0)                    /*!< TIMER1 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER2SPEN         BIT(1)                    /*!< TIMER2 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER3SPEN         BIT(2)                    /*!< TIMER3 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER4SPEN         BIT(3)                    /*!< TIMER4 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER5SPEN         BIT(4)                    /*!< TIMER5 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER6SPEN         BIT(5)                    /*!< TIMER6 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER11SPEN        BIT(6)                    /*!< TIMER11 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER12SPEN        BIT(7)                    /*!< TIMER12 clock enable when sleep mode */
#define RCU_APB1SPEN_TIMER13SPEN        BIT(8)                    /*!< TIMER13 clock enable when sleep mode */
#define RCU_APB1SPEN_WWDGTSPEN          BIT(11)                   /*!< WWDGT clock enable when sleep mode */
#define RCU_APB1SPEN_SPI1SPEN           BIT(14)                   /*!< SPI1 clock enable when sleep mode */
#define RCU_APB1SPEN_SPI2SPEN           BIT(15)                   /*!< SPI2 clock enable when sleep mode */
#define RCU_APB1SPEN_USART1SPEN         BIT(17)                   /*!< USART1 clock enable when sleep mode*/
#define RCU_APB1SPEN_USART2SPEN         BIT(18)                   /*!< USART2 clock enable when sleep mode*/
#define RCU_APB1SPEN_UART3SPEN          BIT(19)                   /*!< UART3 clock enable when sleep mode*/
#define RCU_APB1SPEN_UART4SPEN          BIT(20)                   /*!< UART4 clock enable when sleep mode */
#define RCU_APB1SPEN_I2C0SPEN           BIT(21)                   /*!< I2C0 clock enable when sleep mode */
#define RCU_APB1SPEN_I2C1SPEN           BIT(22)                   /*!< I2C1 clock enable when sleep mode*/
#define RCU_APB1SPEN_I2C2SPEN           BIT(23)                   /*!< I2C2 clock enable when sleep mode */
#define RCU_APB1SPEN_CAN0SPEN           BIT(25)                   /*!< CAN0 clock enable when sleep mode*/
#define RCU_APB1SPEN_CAN1SPEN           BIT(26)                   /*!< CAN1 clock enable when sleep mode */
#define RCU_APB1SPEN_PMUSPEN            BIT(28)                   /*!< PMU clock enable when sleep mode */
#define RCU_APB1SPEN_DACSPEN            BIT(29)                   /*!< DAC clock enable when sleep mode */
#define RCU_APB1SPEN_UART6SPEN          BIT(30)                   /*!< UART6 clock enable when sleep mode */
#define RCU_APB1SPEN_UART7SPEN          BIT(31)                   /*!< UART7 clock enable when sleep mode */

/* RCU_APB2SPEN */
#define RCU_APB2SPEN_TIMER0SPEN         BIT(0)                    /*!< TIMER0 clock enable when sleep mode */
#define RCU_APB2SPEN_TIMER7SPEN         BIT(1)                    /*!< TIMER7 clock enable when sleep mode */
#define RCU_APB2SPEN_USART0SPEN         BIT(4)                    /*!< USART0 clock enable when sleep mode */
#define RCU_APB2SPEN_USART5SPEN         BIT(5)                    /*!< USART5 clock enable when sleep mode */
#define RCU_APB2SPEN_ADC0SPEN           BIT(8)                    /*!< ADC0 clock enable when sleep mode */
#define RCU_APB2SPEN_ADC1SPEN           BIT(9)                    /*!< ADC1 clock enable when sleep mode */
#define RCU_APB2SPEN_ADC2SPEN           BIT(10)                   /*!< ADC2 clock enable when sleep mode */
#define RCU_APB2SPEN_SDIOSPEN           BIT(11)                   /*!< SDIO clock enable when sleep mode */
#define RCU_APB2SPEN_SPI0SPEN           BIT(12)                   /*!< SPI0 clock enable when sleep mode */
#define RCU_APB2SPEN_SPI3SPEN           BIT(13)                   /*!< SPI3 clock enable when sleep mode */
#define RCU_APB2SPEN_SYSCFGSPEN         BIT(14)                   /*!< SYSCFG clock enable when sleep mode */
#define RCU_APB2SPEN_TIMER8SPEN         BIT(16)                   /*!< TIMER8 clock enable when sleep mode */
#define RCU_APB2SPEN_TIMER9SPEN         BIT(17)                   /*!< TIMER9 clock enable when sleep mode */
#define RCU_APB2SPEN_TIMER10SPEN        BIT(18)                   /*!< TIMER10 clock enable when sleep mode */
#define RCU_APB2SPEN_SPI4SPEN           BIT(20)                   /*!< SPI4 clock enable when sleep mode */
#define RCU_APB2SPEN_SPI5SPEN           BIT(21)                   /*!< SPI5 clock enable when sleep mode */
#define RCU_APB2SPEN_TLISPEN            BIT(26)                   /*!< TLI clock enable when sleep mode*/

/* RCU_BDCTL */
#define RCU_BDCTL_LXTALEN               BIT(0)                    /*!< LXTAL enable */
#define RCU_BDCTL_LXTALSTB              BIT(1)                    /*!< low speed crystal oscillator stabilization flag */
#define RCU_BDCTL_LXTALBPS              BIT(2)                    /*!< LXTAL bypass mode enable */
#define RCU_BDCTL_LXTALDRI              BIT(3)                    /*!< LXTAL drive capability */
#define RCU_BDCTL_RTCSRC                BITS(8,9)                 /*!< RTC clock entry selection */
#define RCU_BDCTL_RTCEN                 BIT(15)                   /*!< RTC clock enable */
#define RCU_BDCTL_BKPRST                BIT(16)                   /*!< backup domain reset */

/* RCU_RSTSCK */
#define RCU_RSTSCK_IRC32KEN             BIT(0)                    /*!< IRC32K enable */
#define RCU_RSTSCK_IRC32KSTB            BIT(1)                    /*!< IRC32K stabilization flag */
#define RCU_RSTSCK_RSTFC                BIT(24)                   /*!< reset flag clear */
#define RCU_RSTSCK_BORRSTF              BIT(25)                   /*!< BOR reset flag */
#define RCU_RSTSCK_EPRSTF               BIT(26)                   /*!< external pin reset flag */
#define RCU_RSTSCK_PORRSTF              BIT(27)                   /*!< power reset flag */
#define RCU_RSTSCK_SWRSTF               BIT(28)                   /*!< software reset flag */
#define RCU_RSTSCK_FWDGTRSTF            BIT(29)                   /*!< free watchdog timer reset flag */
#define RCU_RSTSCK_WWDGTRSTF            BIT(30)                   /*!< window watchdog timer reset flag */
#define RCU_RSTSCK_LPRSTF               BIT(31)                   /*!< low-power reset flag */

/* RCU_PLLSSCTL */
#define RCU_PLLSSCTL_MODCNT             BITS(0,12)                /*!< these bits configure PLL spread spectrum modulation 
                                                                       profile amplitude and frequency. the following criteria 
                                                                       must be met: MODSTEP*MODCNT=215-1 */
#define RCU_PLLSSCTL_MODSTEP            BITS(13,27)               /*!< these bits configure PLL spread spectrum modulation 
                                                                       profile amplitude and frequency. the following criteria 
                                                                       must be met: MODSTEP*MODCNT=215-1 */
#define RCU_PLLSSCTL_SS_TYPE            BIT(30)                   /*!< PLL spread spectrum modulation type select */
#define RCU_PLLSSCTL_SSCGON             BIT(31)                   /*!< PLL spread spectrum modulation enable */

/* RCU_PLLI2S */
#define RCU_PLLI2S_PLLI2SN              BITS(6,14)                /*!< the PLLI2S VCO clock multi factor */
#define RCU_PLLI2S_PLLI2SQ              BITS(24,27)               /*!< the PLLI2S Q output frequency division factor from PLLI2S VCO clock */
#define RCU_PLLI2S_PLLI2SR              BITS(28,30)               /*!< the PLLI2S R output frequency division factor from PLLI2S VCO clock */

/* RCU_PLLSAI */
#define RCU_PLLSAI_PLLSAIN              BITS(6,14)                /*!< the PLLSAI VCO clock multi factor */
#define RCU_PLLSAI_PLLSAIP              BITS(16,17)               /*!< the PLLSAI P output frequency division factor from PLLSAI VCO clock */
#define RCU_PLLSAI_PLLSAIQ              BITS(24,27)               /*!< the PLLSAI Q output frequency division factor from PLLSAI VCO clock */
#define RCU_PLLSAI_PLLSAIR              BITS(28,30)               /*!< the PLLSAI R output frequency division factor from PLLSAI VCO clock */

/* RCU_CFG1 */
#define RCU_CFG1_PLLSAIRDIV             BITS(16,17)               /*!< the divider factor from PLLSAIR clock */
#define RCU_CFG1_TIMERSEL               BIT(24)                   /*!< TIMER clock selection */

/* RCU_ADDCTL */
#define RCU_ADDCTL_CK48MSEL             BIT(0)                    /*!< 48MHz clock selection */
#define RCU_ADDCTL_PLL48MSEL            BIT(1)                    /*!< PLL48M clock selection */
#define RCU_ADDCTL_IRC48MEN             BIT(16)                   /*!< internal 48MHz RC oscillator enable */
#define RCU_ADDCTL_IRC48MSTB            BIT(17)                   /*!< internal 48MHz RC oscillator clock stabilization flag */
#define RCU_ADDCTL_IRC48MCAL            BITS(24,31)               /*!< internal 48MHz RC oscillator calibration value register */

/* RCU_ADDINT */
#define RCU_ADDINT_IRC48MSTBIF          BIT(6)                    /*!< IRC48M stabilization interrupt flag */
#define RCU_ADDINT_IRC48MSTBIE          BIT(14)                   /*!< internal 48 MHz RC oscillator stabilization interrupt enable */
#define RCU_ADDINT_IRC48MSTBIC          BIT(22)                   /*!< internal 48 MHz RC oscillator stabilization interrupt clear */

/* RCU_ADDAPB1RST */
#define RCU_ADDAPB1RST_CTCRST           BIT(27)                   /*!< CTC reset */
#define RCU_ADDAPB1RST_IREFRST          BIT(31)                   /*!< IREF reset */

/* RCU_ADDAPB1EN */
#define RCU_ADDAPB1EN_CTCEN             BIT(27)                   /*!< CTC clock enable */
#define RCU_ADDAPB1EN_IREFEN            BIT(31)                   /*!< IREF interface clock enable */

/* RCU_ADDAPB1SPEN */
#define RCU_ADDAPB1SPEN_CTCSPEN         BIT(27)                   /*!< CTC clock enable during sleep mode */
#define RCU_ADDAPB1SPEN_IREFSPEN        BIT(31)                   /*!< IREF interface clock enable during sleep mode */

/* RCU_VKEY */
#define RCU_VKEY_KEY                    BITS(0,31)                /*!< RCU_DSV key register */

/* RCU_DSV */
#define RCU_DSV_DSLPVS                  BITS(0,2)                 /*!< deep-sleep mode voltage select */

/* constants definitions */
/* define the peripheral clock enable bit position and its register index offset */
#define RCU_REGIDX_BIT(regidx, bitpos)      (((uint32_t)(regidx) << 6) | (uint32_t)(bitpos))
#define RCU_REG_VAL(periph)                 (REG32(RCU_BASE + ((uint32_t)(periph) >> 6)))
#define RCU_BIT_POS(val)                    ((uint32_t)(val) & 0x1FU)
/* define the voltage key unlock value */
#define RCU_VKEY_UNLOCK                 ((uint32_t)0x1A2B3C4DU)
/* APB1 additional peripherals */
//    RCU_CTCRST       = RCU_REGIDX_BIT(ADD_APB1RST_REG_OFFSET, 27U),         /*!< CTC clock reset */
//    RCU_IREFRST      = RCU_REGIDX_BIT(ADD_APB1RST_REG_OFFSET, 31U)          /*!< IREF clock reset */

/**
  * @}
  */ 
 /** @defgroup RCU_AHB1_Peripherals 
  * @{
  */
/* peripherals reset */
typedef enum
{
	RCU_AHB1Periph_GPIOA    = ((uint32_t)0x00000001),
	RCU_AHB1Periph_GPIOB    = ((uint32_t)0x00000002),
	RCU_AHB1Periph_GPIOC    = ((uint32_t)0x00000004),
  RCU_AHB1Periph_GPIOD    = ((uint32_t)0x00000008),
  RCU_AHB1Periph_GPIOE    = ((uint32_t)0x00000010),
  RCU_AHB1Periph_GPIOF    = ((uint32_t)0x00000020),
  RCU_AHB1Periph_GPIOG    = ((uint32_t)0x00000040),
  RCU_AHB1Periph_GPIOH    = ((uint32_t)0x00000080),
  RCU_AHB1Periph_GPIOI    = ((uint32_t)0x00000100),
  RCU_AHB1Periph_CRC      = ((uint32_t)0x00001000),
  RCU_AHB1Periph_FLITF    = ((uint32_t)0x00008000),
  RCU_AHB1Periph_SRAM0    = ((uint32_t)0x00010000),
  RCU_AHB1Periph_SRAM1    = ((uint32_t)0x00020000),
  RCU_AHB1Periph_BKPSRAM  = ((uint32_t)0x00040000),
  RCU_AHB1Periph_SRAM2    = ((uint32_t)0x00080000),
  RCU_AHB1Periph_DMA0          = ((uint32_t)0x00200000),
  RCU_AHB1Periph_DMA1          = ((uint32_t)0x00400000),
  RCU_AHB1Periph_DMA2D         = ((uint32_t)0x00800000),
  RCU_AHB1Periph_ETH_MAC       = ((uint32_t)0x02000000),
  RCU_AHB1Periph_ETH_MAC_Tx    = ((uint32_t)0x04000000),
  RCU_AHB1Periph_ETH_MAC_Rx    = ((uint32_t)0x08000000),
  RCU_AHB1Periph_ETH_MAC_PTP   = ((uint32_t)0x10000000),
  RCU_AHB1Periph_OTG_HS        = ((uint32_t)0x20000000),
  RCU_AHB1Periph_OTG_HS_ULPI   = ((uint32_t)0x40000000)
}RCU_AHB1PERIPH_ENUM;

/**
  * @}
  */ 
  
/** @defgroup RCU_AHB2_Peripherals 
  * @{
  */  
typedef enum
{
 RCU_AHB2Periph_DCMI   = ((uint32_t)0x00000001),
 RCU_AHB2Periph_CRYP   = ((uint32_t)0x00000010),
 RCU_AHB2Periph_HASH   = ((uint32_t)0x00000020),
 RCU_AHB2Periph_RNG    = ((uint32_t)0x00000040),
 RCU_AHB2Periph_OTG_FS = ((uint32_t)0x00000080)
}RCU_AHB2PERIPH_ENUM;
/**
  * @}
  */  
/** @defgroup RCU_APB1_Peripherals 
  * @{
  */ 
typedef enum
{
  RCU_APB1Periph_TIM1      = ((uint32_t)0x00000001),
  RCU_APB1Periph_TIM2      = ((uint32_t)0x00000002),
  RCU_APB1Periph_TIM3      = ((uint32_t)0x00000004),
  RCU_APB1Periph_TIM4      = ((uint32_t)0x00000008),
  RCU_APB1Periph_TIM5      = ((uint32_t)0x00000010),
  RCU_APB1Periph_TIM6      = ((uint32_t)0x00000020),
  RCU_APB1Periph_TIM11     = ((uint32_t)0x00000040),
  RCU_APB1Periph_TIM12     = ((uint32_t)0x00000080),
  RCU_APB1Periph_TIM13     = ((uint32_t)0x00000100),
  RCU_APB1Periph_WWDG      = ((uint32_t)0x00000800),
  RCU_APB1Periph_SPI1      = ((uint32_t)0x00004000),
  RCU_APB1Periph_SPI2      = ((uint32_t)0x00008000),
  RCU_APB1Periph_USART1    = ((uint32_t)0x00020000),
  RCU_APB1Periph_USART2    = ((uint32_t)0x00040000),
  RCU_APB1Periph_UART3     = ((uint32_t)0x00080000),
  RCU_APB1Periph_UART4     = ((uint32_t)0x00100000),
  RCU_APB1Periph_I2C0      = ((uint32_t)0x00200000),
  RCU_APB1Periph_I2C1      = ((uint32_t)0x00400000),
  RCU_APB1Periph_I2C2      = ((uint32_t)0x00800000),
  RCU_APB1Periph_CAN0      = ((uint32_t)0x02000000),
  RCU_APB1Periph_CAN1      = ((uint32_t)0x04000000),
  RCU_APB1Periph_PWR       = ((uint32_t)0x10000000),
  RCU_APB1Periph_DAC       = ((uint32_t)0x20000000),
  RCU_APB1Periph_UART6     = ((uint32_t)0x40000000),
  RCU_APB1Periph_UART7     = ( (int32_t)0x80000000U)
}RCU_APB1PERIPH_ENUM;
/**
  * @}
  */ 
 
#define RCU_AHB3Periph_FSMC                ((uint32_t)0x00000001)
/** @defgroup RCU_APB2_Peripherals 
  * @{
  */ 
typedef enum
{
  RCU_APB2Periph_TIM0     = ((uint32_t)0x00000001),
  RCU_APB2Periph_TIM7     = ((uint32_t)0x00000002),
  RCU_APB2Periph_USART0   = ((uint32_t)0x00000010),
  RCU_APB2Periph_USART5   = ((uint32_t)0x00000020),
  RCU_APB2Periph_ADC0     = ((uint32_t)0x00000100),
  RCU_APB2Periph_ADC1     = ((uint32_t)0x00000200),
  RCU_APB2Periph_ADC2     = ((uint32_t)0x00000400),
  RCU_APB2Periph_SDIO     = ((uint32_t)0x00000800),
  RCU_APB2Periph_SPI0     = ((uint32_t)0x00001000),
  RCU_APB2Periph_SPI3     = ((uint32_t)0x00002000),
  RCU_APB2Periph_SYSCFG   = ((uint32_t)0x00004000),
  RCU_APB2Periph_TIM8     = ((uint32_t)0x00010000),
  RCU_APB2Periph_TIM9     = ((uint32_t)0x00020000),
  RCU_APB2Periph_TIM10    = ((uint32_t)0x00040000),
  RCU_APB2Periph_SPI4     = ((uint32_t)0x00100000),
  RCU_APB2Periph_SPI5     = ((uint32_t)0x00200000),
  RCU_APB2Periph_SAI1     = ((uint32_t)0x00400000),
  RCU_APB2Periph_LTDC     = ((uint32_t)0x04000000)
}RCU_APB2PERIPH_ENUM;

typedef enum
{
	/* clock stabilization flags */
	RCU_FLAG_IRC16MSTB  = ((u32)0x00000021U), 
	RCU_FLAG_HXTALSTB   = ((u32)0x00000031U),
	RCU_FLAG_PLLSTB     = ((u32)0x00000039U),
	RCU_FLAG_PLLI2SSTB  = ((u32)0x0000003BU),
	RCU_FLAG_PLLSAISTB  = ((u32)0x0000003DU),
	RCU_FLAG_LXTALSTB   = ((u32)0x00000041U),		/*!< LXTAL stabilization flags */
	RCU_FLAG_IRC32KSTB  = ((u32)0x00000061U),		/*!< IRC32K stabilization flags */
	RCU_FLAG_IRC48MSTB  = ((u32)0x00000111U),   /*!< IRC48M stabilization flags */
	/* reset source flags */
	RCC_FLAG_BOR      = ((u32)0x00000079U),
	RCC_FLAG_PIN      = ((u32)0x0000007AU),
	RCC_FLAG_POR      = ((u32)0x0000007BU),
	RCC_FLAG_SFT      = ((u32)0x0000007CU),
	RCC_FLAG_IWDG     = ((u32)0x0000007DU),
	RCU_FLAG_WWDGT    = ((u32)0x0000007EU),
	RCU_FLAG_LP    	  = ((u32)0x7000000FU)
}RCU_FLAG_ENUM;

/* clock stabilization and ckm interrupt flags */
typedef enum
{
    RCU_INT_FLAG_IRC32KSTB = ((uint8_t)0x01),            /*!< IRC32K stabilization interrupt flag */
    RCU_INT_FLAG_LXTALSTB  = ((uint8_t)0x02),            /*!< LXTAL stabilization interrupt flag */
    RCU_INT_FLAG_IRC16MSTB = ((uint8_t)0x04),            /*!< IRC16M stabilization interrupt flag */
    RCU_INT_FLAG_HXTALSTB  = ((uint8_t)0x08),            /*!< HXTAL stabilization interrupt flag */
    RCU_INT_FLAG_PLLSTB    = ((uint8_t)0x10),            /*!< PLL stabilization interrupt flag */
    RCU_INT_FLAG_PLLI2SSTB = ((uint8_t)0x20),            /*!< PLLI2S stabilization interrupt flag */
    RCU_INT_FLAG_PLLSAISTB = ((uint8_t)0x40),            /*!< PLLSAI stabilization interrupt flag */
    RCU_INT_FLAG_CKM       = ((uint8_t)0x80)            /*!< HXTAL clock stuck interrupt flag */
   // RCU_INT_FLAG_IRC48MSTB = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 6U),         /*!< IRC48M stabilization interrupt flag */
}RCU_INT_FLAG_ENUM;

/* clock stabilization and stuck interrupt flags clear */
typedef enum
{
    RCU_INT_FLAG_IRC32KSTB_CLR = BIT(16),       /*!< IRC32K stabilization interrupt flags clear */
    RCU_INT_FLAG_LXTALSTB_CLR  = BIT(17),       /*!< LXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_IRC16MSTB_CLR = BIT(18),       /*!< IRC16M stabilization interrupt flags clear */
    RCU_INT_FLAG_HXTALSTB_CLR  = BIT(19),       /*!< HXTAL stabilization interrupt flags clear */
    RCU_INT_FLAG_PLLSTB_CLR    = BIT(20),       /*!< PLL stabilization interrupt flags clear */
    RCU_INT_FLAG_PLLI2SSTB_CLR = BIT(21),       /*!< PLLI2S stabilization interrupt flags clear */
    RCU_INT_FLAG_PLLSAISTB_CLR = BIT(22),       /*!< PLLSAI stabilization interrupt flags clear */
    RCU_INT_FLAG_CKM_CLR       = BIT(23)       /*!< CKM interrupt flags clear */
    //RCU_INT_FLAG_IRC48MSTB_CLR = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 22U),    /*!< internal 48 MHz RC oscillator stabilization interrupt clear */
}RCU_INT_FLAG_CLEAR_ENUM;

/* clock stabilization interrupt enable or disable */
typedef enum
{
    RCU_INT_IRC32KSTB       = BIT(8),           /*!< IRC32K stabilization interrupt */
    RCU_INT_LXTALSTB        = BIT(9),           /*!< LXTAL stabilization interrupt */
    RCU_INT_IRC16MSTB       = BIT(10),          /*!< IRC16M stabilization interrupt */
    RCU_INT_HXTALSTB        = BIT(11),          /*!< HXTAL stabilization interrupt */
    RCU_INT_PLLSTB          = BIT(12),          /*!< PLL stabilization interrupt */
    RCU_INT_PLLI2SSTB       = BIT(13),          /*!< PLLI2S stabilization interrupt */
    RCU_INT_PLLSAISTB       = BIT(14)         /*!< PLLSAI stabilization interrupt */
   // RCU_INT_IRC48MSTB       = RCU_REGIDX_BIT(ADDINT_REG_OFFSET, 14U),       /*!< internal 48 MHz RC oscillator stabilization interrupt */
}RCU_INT_ENUM;

/* rcu clock frequency */
typedef enum
{
    CK_SYS      = 0,                                                        /*!< system clock */
    CK_AHB,                                                                 /*!< AHB clock */
    CK_APB1,                                                                /*!< APB1 clock */
    CK_APB2,                                                                /*!< APB2 clock */
}rcu_clock_freq_enum;

/* RCU_CFG0 register bit define */
/* system clock source select */
#define CFG0_SCS(regval)                (BITS(0,1) & ((uint32_t)(regval) << 0))
#define RCU_CKSYSSRC_IRC16M             CFG0_SCS(0)                        /*!< system clock source select IRC16M */
#define RCU_CKSYSSRC_HXTAL              CFG0_SCS(1)                        /*!< system clock source select HXTAL */
#define RCU_CKSYSSRC_PLLP               CFG0_SCS(2)                        /*!< system clock source select PLLP */

/* system clock source select status */
#define CFG0_SCSS(regval)               (BITS(2,3) & ((uint32_t)(regval) << 2))
#define RCU_SCSS_IRC16M                 CFG0_SCSS(0)                       /*!< system clock source select IRC16M */
#define RCU_SCSS_HXTAL                  CFG0_SCSS(1)                       /*!< system clock source select HXTAL */
#define RCU_SCSS_PLLP                   CFG0_SCSS(2)                       /*!< system clock source select PLLP */

/* AHB prescaler selection */
#define CFG0_AHBPSC(regval)             (BITS(4,7) & ((uint32_t)(regval) << 4))
#define RCU_AHB_CKSYS_DIV1              CFG0_AHBPSC(0)                     /*!< AHB prescaler select CK_SYS */
#define RCU_AHB_CKSYS_DIV2              CFG0_AHBPSC(8)                     /*!< AHB prescaler select CK_SYS/2 */
#define RCU_AHB_CKSYS_DIV4              CFG0_AHBPSC(9)                     /*!< AHB prescaler select CK_SYS/4 */
#define RCU_AHB_CKSYS_DIV8              CFG0_AHBPSC(10)                    /*!< AHB prescaler select CK_SYS/8 */
#define RCU_AHB_CKSYS_DIV16             CFG0_AHBPSC(11)                    /*!< AHB prescaler select CK_SYS/16 */
#define RCU_AHB_CKSYS_DIV64             CFG0_AHBPSC(12)                    /*!< AHB prescaler select CK_SYS/64 */
#define RCU_AHB_CKSYS_DIV128            CFG0_AHBPSC(13)                    /*!< AHB prescaler select CK_SYS/128 */
#define RCU_AHB_CKSYS_DIV256            CFG0_AHBPSC(14)                    /*!< AHB prescaler select CK_SYS/256 */
#define RCU_AHB_CKSYS_DIV512            CFG0_AHBPSC(15)                    /*!< AHB prescaler select CK_SYS/512 */

/* APB1 prescaler selection */
#define CFG0_APB1PSC(regval)            (BITS(10,12) & ((uint32_t)(regval) << 10))
#define RCU_APB1_CKAHB_DIV1             CFG0_APB1PSC(0)                    /*!< APB1 prescaler select CK_AHB */
#define RCU_APB1_CKAHB_DIV2             CFG0_APB1PSC(4)                    /*!< APB1 prescaler select CK_AHB/2 */
#define RCU_APB1_CKAHB_DIV4             CFG0_APB1PSC(5)                    /*!< APB1 prescaler select CK_AHB/4 */
#define RCU_APB1_CKAHB_DIV8             CFG0_APB1PSC(6)                    /*!< APB1 prescaler select CK_AHB/8 */
#define RCU_APB1_CKAHB_DIV16            CFG0_APB1PSC(7)                    /*!< APB1 prescaler select CK_AHB/16 */

/* APB2 prescaler selection */
#define CFG0_APB2PSC(regval)            (BITS(13,15) & ((uint32_t)(regval) << 13))
#define RCU_APB2_CKAHB_DIV1             CFG0_APB2PSC(0)                    /*!< APB2 prescaler select CK_AHB */
#define RCU_APB2_CKAHB_DIV2             CFG0_APB2PSC(4)                    /*!< APB2 prescaler select CK_AHB/2 */
#define RCU_APB2_CKAHB_DIV4             CFG0_APB2PSC(5)                    /*!< APB2 prescaler select CK_AHB/4 */
#define RCU_APB2_CKAHB_DIV8             CFG0_APB2PSC(6)                    /*!< APB2 prescaler select CK_AHB/8 */
#define RCU_APB2_CKAHB_DIV16            CFG0_APB2PSC(7)                    /*!< APB2 prescaler select CK_AHB/16 */

/* RTC clock divider factor from HXTAL clock */
#define CFG0_RTCDIV(regval)             (BITS(16,20) & ((uint32_t)(regval) << 16))
#define RCU_RTC_HXTAL_NONE              CFG0_RTCDIV(0)                     /*!< no clock for RTC */
#define RCU_RTC_HXTAL_DIV2              CFG0_RTCDIV(2)                     /*!< RTCDIV clock select CK_HXTAL/2 */
#define RCU_RTC_HXTAL_DIV3              CFG0_RTCDIV(3)                     /*!< RTCDIV clock select CK_HXTAL/3 */
#define RCU_RTC_HXTAL_DIV4              CFG0_RTCDIV(4)                     /*!< RTCDIV clock select CK_HXTAL/4 */
#define RCU_RTC_HXTAL_DIV5              CFG0_RTCDIV(5)                     /*!< RTCDIV clock select CK_HXTAL/5 */
#define RCU_RTC_HXTAL_DIV6              CFG0_RTCDIV(6)                     /*!< RTCDIV clock select CK_HXTAL/6 */
#define RCU_RTC_HXTAL_DIV7              CFG0_RTCDIV(7)                     /*!< RTCDIV clock select CK_HXTAL/7 */
#define RCU_RTC_HXTAL_DIV8              CFG0_RTCDIV(8)                     /*!< RTCDIV clock select CK_HXTAL/8 */
#define RCU_RTC_HXTAL_DIV9              CFG0_RTCDIV(9)                     /*!< RTCDIV clock select CK_HXTAL/9 */
#define RCU_RTC_HXTAL_DIV10             CFG0_RTCDIV(10)                    /*!< RTCDIV clock select CK_HXTAL/10 */
#define RCU_RTC_HXTAL_DIV11             CFG0_RTCDIV(11)                    /*!< RTCDIV clock select CK_HXTAL/11 */
#define RCU_RTC_HXTAL_DIV12             CFG0_RTCDIV(12)                    /*!< RTCDIV clock select CK_HXTAL/12 */
#define RCU_RTC_HXTAL_DIV13             CFG0_RTCDIV(13)                    /*!< RTCDIV clock select CK_HXTAL/13 */
#define RCU_RTC_HXTAL_DIV14             CFG0_RTCDIV(14)                    /*!< RTCDIV clock select CK_HXTAL/14 */
#define RCU_RTC_HXTAL_DIV15             CFG0_RTCDIV(15)                    /*!< RTCDIV clock select CK_HXTAL/15 */
#define RCU_RTC_HXTAL_DIV16             CFG0_RTCDIV(16)                    /*!< RTCDIV clock select CK_HXTAL/16 */
#define RCU_RTC_HXTAL_DIV17             CFG0_RTCDIV(17)                    /*!< RTCDIV clock select CK_HXTAL/17 */
#define RCU_RTC_HXTAL_DIV18             CFG0_RTCDIV(18)                    /*!< RTCDIV clock select CK_HXTAL/18 */
#define RCU_RTC_HXTAL_DIV19             CFG0_RTCDIV(19)                    /*!< RTCDIV clock select CK_HXTAL/19 */
#define RCU_RTC_HXTAL_DIV20             CFG0_RTCDIV(20)                    /*!< RTCDIV clock select CK_HXTAL/20 */
#define RCU_RTC_HXTAL_DIV21             CFG0_RTCDIV(21)                    /*!< RTCDIV clock select CK_HXTAL/21 */
#define RCU_RTC_HXTAL_DIV22             CFG0_RTCDIV(22)                    /*!< RTCDIV clock select CK_HXTAL/22 */
#define RCU_RTC_HXTAL_DIV23             CFG0_RTCDIV(23)                    /*!< RTCDIV clock select CK_HXTAL/23 */
#define RCU_RTC_HXTAL_DIV24             CFG0_RTCDIV(24)                    /*!< RTCDIV clock select CK_HXTAL/24 */
#define RCU_RTC_HXTAL_DIV25             CFG0_RTCDIV(25)                    /*!< RTCDIV clock select CK_HXTAL/25 */
#define RCU_RTC_HXTAL_DIV26             CFG0_RTCDIV(26)                    /*!< RTCDIV clock select CK_HXTAL/26 */
#define RCU_RTC_HXTAL_DIV27             CFG0_RTCDIV(27)                    /*!< RTCDIV clock select CK_HXTAL/27 */
#define RCU_RTC_HXTAL_DIV28             CFG0_RTCDIV(28)                    /*!< RTCDIV clock select CK_HXTAL/28 */
#define RCU_RTC_HXTAL_DIV29             CFG0_RTCDIV(29)                    /*!< RTCDIV clock select CK_HXTAL/29 */
#define RCU_RTC_HXTAL_DIV30             CFG0_RTCDIV(30)                    /*!< RTCDIV clock select CK_HXTAL/30 */
#define RCU_RTC_HXTAL_DIV31             CFG0_RTCDIV(31)                    /*!< RTCDIV clock select CK_HXTAL/31 */

/* CKOUT0 Clock source selection */
#define CFG0_CKOUT0SEL(regval)          (BITS(21,22) & ((uint32_t)(regval) << 21))
#define RCU_CKOUT0SRC_IRC16M            CFG0_CKOUT0SEL(0)                  /*!< internal 16M RC oscillator clock selected */
#define RCU_CKOUT0SRC_LXTAL             CFG0_CKOUT0SEL(1)                  /*!< low speed crystal oscillator clock (LXTAL) selected */
#define RCU_CKOUT0SRC_HXTAL             CFG0_CKOUT0SEL(2)                  /*!< high speed crystal oscillator clock (HXTAL) selected */
#define RCU_CKOUT0SRC_PLLP              CFG0_CKOUT0SEL(3)                  /*!< CK_PLLP clock selected */

/* The CK_OUT0 divider */
#define CFG0_CKOUT0DIV(regval)          (BITS(24,26) & ((uint32_t)(regval) << 24))
#define RCU_CKOUT0_DIV1                 CFG0_CKOUT0DIV(0)                  /*!< CK_OUT0 is divided by 1 */
#define RCU_CKOUT0_DIV2                 CFG0_CKOUT0DIV(4)                  /*!< CK_OUT0 is divided by 2 */
#define RCU_CKOUT0_DIV3                 CFG0_CKOUT0DIV(5)                  /*!< CK_OUT0 is divided by 3 */
#define RCU_CKOUT0_DIV4                 CFG0_CKOUT0DIV(6)                  /*!< CK_OUT0 is divided by 4 */
#define RCU_CKOUT0_DIV5                 CFG0_CKOUT0DIV(7)                  /*!< CK_OUT0 is divided by 5 */

/* The CK_OUT1 divider */
#define CFG0_CKOUT1DIV(regval)          (BITS(27,29) & ((uint32_t)(regval) << 27))
#define RCU_CKOUT1_DIV1                 CFG0_CKOUT1DIV(0)                  /*!< CK_OUT1 is divided by 1 */
#define RCU_CKOUT1_DIV2                 CFG0_CKOUT1DIV(4)                  /*!< CK_OUT1 is divided by 2 */
#define RCU_CKOUT1_DIV3                 CFG0_CKOUT1DIV(5)                  /*!< CK_OUT1 is divided by 3 */
#define RCU_CKOUT1_DIV4                 CFG0_CKOUT1DIV(6)                  /*!< CK_OUT1 is divided by 4 */
#define RCU_CKOUT1_DIV5                 CFG0_CKOUT1DIV(7)                  /*!< CK_OUT1 is divided by 5 */

/* CKOUT1 Clock source selection */
#define CFG0_CKOUT1SEL(regval)          (BITS(30,31) & ((uint32_t)(regval) << 30))
#define RCU_CKOUT1SRC_SYSTEMCLOCK       CFG0_CKOUT1SEL(0)                  /*!< system clock selected */
#define RCU_CKOUT1SRC_PLLI2SR           CFG0_CKOUT1SEL(1)                  /*!< CK_PLLI2SR clock selected */
#define RCU_CKOUT1SRC_HXTAL             CFG0_CKOUT1SEL(2)                  /*!< high speed crystal oscillator clock (HXTAL) selected */
#define RCU_CKOUT1SRC_PLLP              CFG0_CKOUT1SEL(3)                  /*!< CK_PLLP clock selected */

/* RCU_CFG1 register bit define */
/* the divider factor from PLLI2SQ clock */
#define CFG1_PLLI2SQDIV(regval)         (BITS(0,4) & ((uint32_t)(regval) << 0))
#define RCU_PLLI2SQ_DIV1                CFG1_PLLI2SQDIV(0)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/1 */
#define RCU_PLLI2SQ_DIV2                CFG1_PLLI2SQDIV(1)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/2 */
#define RCU_PLLI2SQ_DIV3                CFG1_PLLI2SQDIV(2)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/3 */
#define RCU_PLLI2SQ_DIV4                CFG1_PLLI2SQDIV(3)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/4 */
#define RCU_PLLI2SQ_DIV5                CFG1_PLLI2SQDIV(4)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/5 */
#define RCU_PLLI2SQ_DIV6                CFG1_PLLI2SQDIV(5)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/6 */
#define RCU_PLLI2SQ_DIV7                CFG1_PLLI2SQDIV(6)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/7 */
#define RCU_PLLI2SQ_DIV8                CFG1_PLLI2SQDIV(7)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/8 */
#define RCU_PLLI2SQ_DIV9                CFG1_PLLI2SQDIV(8)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/9 */
#define RCU_PLLI2SQ_DIV10               CFG1_PLLI2SQDIV(9)                 /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/10 */
#define RCU_PLLI2SQ_DIV11               CFG1_PLLI2SQDIV(10)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/11 */
#define RCU_PLLI2SQ_DIV12               CFG1_PLLI2SQDIV(11)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/12 */
#define RCU_PLLI2SQ_DIV13               CFG1_PLLI2SQDIV(12)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/13 */
#define RCU_PLLI2SQ_DIV14               CFG1_PLLI2SQDIV(13)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/14 */
#define RCU_PLLI2SQ_DIV15               CFG1_PLLI2SQDIV(14)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/15 */
#define RCU_PLLI2SQ_DIV16               CFG1_PLLI2SQDIV(15)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/16 */
#define RCU_PLLI2SQ_DIV17               CFG1_PLLI2SQDIV(16)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/17 */
#define RCU_PLLI2SQ_DIV18               CFG1_PLLI2SQDIV(17)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/18 */
#define RCU_PLLI2SQ_DIV19               CFG1_PLLI2SQDIV(18)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/19 */
#define RCU_PLLI2SQ_DIV20               CFG1_PLLI2SQDIV(19)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/20 */
#define RCU_PLLI2SQ_DIV21               CFG1_PLLI2SQDIV(20)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/21 */
#define RCU_PLLI2SQ_DIV22               CFG1_PLLI2SQDIV(21)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/22 */
#define RCU_PLLI2SQ_DIV23               CFG1_PLLI2SQDIV(22)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/23 */
#define RCU_PLLI2SQ_DIV24               CFG1_PLLI2SQDIV(23)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/24 */
#define RCU_PLLI2SQ_DIV25               CFG1_PLLI2SQDIV(24)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/25 */
#define RCU_PLLI2SQ_DIV26               CFG1_PLLI2SQDIV(25)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/26 */
#define RCU_PLLI2SQ_DIV27               CFG1_PLLI2SQDIV(26)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/27 */
#define RCU_PLLI2SQ_DIV28               CFG1_PLLI2SQDIV(27)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/28 */
#define RCU_PLLI2SQ_DIV29               CFG1_PLLI2SQDIV(28)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/29 */
#define RCU_PLLI2SQ_DIV30               CFG1_PLLI2SQDIV(29)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/30 */
#define RCU_PLLI2SQ_DIV31               CFG1_PLLI2SQDIV(30)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/31 */
#define RCU_PLLI2SQ_DIV32               CFG1_PLLI2SQDIV(31)                /*!< CK_PLLI2SQDIV clock select CK_PLLI2SQ/32 */

/* the divider factor from PLLSAIR clock */
#define CFG1_PLLSAIRDIV(regval)         (BITS(16,17) & ((uint32_t)(regval) << 16))
#define RCU_PLLSAIR_DIV2                CFG1_PLLSAIRDIV(0)                 /*!< CK_PLLSAIRDIV clock select CK_PLLSAIR/2 */
#define RCU_PLLSAIR_DIV4                CFG1_PLLSAIRDIV(1)                 /*!< CK_PLLSAIRDIV clock select CK_PLLSAIR/4 */
#define RCU_PLLSAIR_DIV8                CFG1_PLLSAIRDIV(2)                 /*!< CK_PLLSAIRDIV clock select CK_PLLSAIR/8 */
#define RCU_PLLSAIR_DIV16               CFG1_PLLSAIRDIV(3)                 /*!< CK_PLLSAIRDIV clock select CK_PLLSAIR/16 */

/* TIMER clock selection */
#define RCU_TIMER_PSC_MUL2              ~RCU_CFG1_TIMERSEL                 /*!< if APB1PSC/APB2PSC in RCU_CFG0 register is 0b0xx(CK_APBx = CK_AHB) 
                                                                                or 0b100(CK_APBx = CK_AHB/2), the TIMER clock is equal to CK_AHB(CK_TIMERx = CK_AHB).
                                                                                or else, the TIMER clock is twice the corresponding APB clock (TIMER in APB1 domain: CK_TIMERx = 2 x CK_APB1; 
                                                                                TIMER in APB2 domain: CK_TIMERx = 2 x CK_APB2) */
#define RCU_TIMER_PSC_MUL4              RCU_CFG1_TIMERSEL                  /*!< if APB1PSC/APB2PSC in RCU_CFG0 register is 0b0xx(CK_APBx = CK_AHB), 
                                                                                0b100(CK_APBx = CK_AHB/2), or 0b101(CK_APBx = CK_AHB/4), the TIMER clock is equal to CK_AHB(CK_TIMERx = CK_AHB). 
                                                                                or else, the TIMER clock is four timers the corresponding APB clock (TIMER in APB1 domain: CK_TIMERx = 4 x CK_APB1;  
                                                                                TIMER in APB2 domain: CK_TIMERx = 4 x CK_APB2) */

/* RCU_PLLSSCTL register bit define */
/* PLL spread spectrum modulation type select */
#define RCU_SS_TYPE_CENTER              ((uint32_t)0x00000000U)            /*!< center type is selected */
#define RCU_SS_TYPE_DOWN                RCU_PLLSSCTL_SS_TYPE               /*!< down type is selected */

/* RCU_PLL register bit define */
/* The PLL VCO source clock prescaler */
#define RCU_PLLPSC_DIV_MIN              ((uint32_t)2U)                     /*!< PLLPSC_DIV min value */
#define RCU_PLLPSC_DIV_MAX              ((uint32_t)63U)                    /*!< PLLPSC_DIV max value */

/* The PLL VCO clock multi factor */
#define RCU_PLLN_MUL_MIN                ((uint32_t)64U)                    /*!< PLLN_MUL min value */
#define RCU_PLLN_MUL_MAX                ((uint32_t)500U)                   /*!< PLLN_MUL max value */
#define RCU_SS_MODULATION_CENTER_INC    ((uint32_t)5U)                     /*!< minimum factor of PLLN in center mode */
#define RCU_SS_MODULATION_DOWN_INC      ((uint32_t)7U)                     /*!< minimum factor of PLLN in down mode */

/* The PLLP output frequency division factor from PLL VCO clock */
#define RCU_PLLP_DIV_MIN                ((uint32_t)2U)                     /*!< PLLP_DIV min value */
#define RCU_PLLP_DIV_MAX                ((uint32_t)8U)                     /*!< PLLP_DIV max value */
                                         
/* PLL Clock Source Selection  */
#define RCU_PLLSRC_IRC16M               ((uint32_t)0x00000000U)            /*!< IRC16M clock selected as source clock of PLL, PLLSAI, PLLI2S */
#define RCU_PLLSRC_HXTAL                RCU_PLL_PLLSEL                     /*!< HXTAL clock selected as source clock of PLL, PLLSAI, PLLI2S */

/* The PLL Q output frequency division factor from PLL VCO clock */
#define RCU_PLLQ_DIV_MIN                ((uint32_t)2U)                     /*!< PLLQ_DIV min value */
#define RCU_PLLQ_DIV_MAX                ((uint32_t)15U)                    /*!< PLLQ_DIV max value */

#define CHECK_PLL_PSC_VALID(val)        (((val) >= RCU_PLLPSC_DIV_MIN)&&((val) <= RCU_PLLPSC_DIV_MAX))            
#define CHECK_PLL_N_VALID(val, inc)     (((val) >= (RCU_PLLN_MUL_MIN + (inc)))&&((val) <= RCU_PLLN_MUL_MAX))      
#define CHECK_PLL_P_VALID(val)          (((val) == 2U) || ((val) == 4U) || ((val) == 6U) || ((val) == 8U))         
#define CHECK_PLL_Q_VALID(val)          (((val) >= RCU_PLLQ_DIV_MIN)&&((val) <= RCU_PLLQ_DIV_MAX))                 

/* RCU_BDCTL register bit define */
/* LXTAL drive capability */
#define RCU_LXTALDRI_LOWER_DRIVE        ((uint32_t)0x00000000)             /*!< LXTAL drive capability is selected lower */
#define RCU_LXTALDRI_HIGHER_DRIVE       RCU_BDCTL_LXTALDRI                 /*!< LXTAL drive capability is selected higher */

/* RTC clock entry selection */
#define BDCTL_RTCSRC(regval)            (BITS(8,9) & ((uint32_t)(regval) << 8))
#define RCU_RTCSRC_NONE                 BDCTL_RTCSRC(0)                    /*!< no clock selected */
#define RCU_RTCSRC_LXTAL                BDCTL_RTCSRC(1)                    /*!< RTC source clock select LXTAL  */
#define RCU_RTCSRC_IRC32K               BDCTL_RTCSRC(2)                    /*!< RTC source clock select IRC32K */
#define RCU_RTCSRC_HXTAL_DIV_RTCDIV     BDCTL_RTCSRC(3)                    /*!< RTC source clock select HXTAL/RTCDIV */

/* RCU_PLLI2S register bit define */
/* The PLLI2S VCO clock multi factor */
#define RCU_PLLI2SN_MUL_MIN             50U
#define RCU_PLLI2SN_MUL_MAX             500U

/* The PLLI2S Q output frequency division factor from PLLI2S VCO clock */
#define RCU_PLLI2SQ_DIV_MIN             2U
#define RCU_PLLI2SQ_DIV_MAX             15U

/* The PLLI2S R output frequency division factor from PLLI2S VCO clock */
#define RCU_PLLI2SR_DIV_MIN             2U
#define RCU_PLLI2SR_DIV_MAX             7U

/* RCU_PLLSAI register bit define */
/* The PLLSAI VCO clock multi factor */
#define RCU_PLLSAIN_MUL_MIN             50U
#define RCU_PLLSAIN_MUL_MAX             500U

/* The PLLSAI P output frequency division factor from PLLSAI VCO clock */
#define RCU_PLLSAIP_DIV_MIN             2U
#define RCU_PLLSAIP_DIV_MAX             8U

/* The PLLSAI Q output frequency division factor from PLLSAI VCO clock */
#define RCU_PLLSAIQ_DIV_MIN             2U
#define RCU_PLLSAIQ_DIV_MAX             15U

/* The PLLSAI R output frequency division factor from PLLSAI VCO clock */
#define RCU_PLLSAIR_DIV_MIN             2U
#define RCU_PLLSAIR_DIV_MAX             7U

#define CHECK_PLLI2S_PSC_VALID(val)     (((val) >= RCU_PLLI2SPSC_DIV_MIN)&&((val) <= RCU_PLLI2SPSC_DIV_MAX))
#define CHECK_PLLI2S_N_VALID(val)       (((val) >= RCU_PLLI2SN_MUL_MIN)&&((val) <= RCU_PLLI2SN_MUL_MAX))
#define CHECK_PLLI2S_Q_VALID(val)       (((val) >= RCU_PLLI2SQ_DIV_MIN)&&((val) <= RCU_PLLI2SQ_DIV_MAX))
#define CHECK_PLLI2S_R_VALID(val)       (((val) >= RCU_PLLI2SR_DIV_MIN)&&((val) <= RCU_PLLI2SR_DIV_MAX))

#define CHECK_PLLSAI_N_VALID(val)       (((val) >= (RCU_PLLSAIN_MUL_MIN))&&((val) <= RCU_PLLSAIN_MUL_MAX))
#define CHECK_PLLSAI_P_VALID(val)       (((val) == 2U) || ((val) == 4U) || ((val) == 6U) || ((val) == 8U))
#define CHECK_PLLSAI_Q_VALID(val)       (((val) >= RCU_PLLSAIQ_DIV_MIN)&&((val) <= RCU_PLLSAIQ_DIV_MAX))
#define CHECK_PLLSAI_R_VALID(val)       (((val) >= RCU_PLLSAIR_DIV_MIN)&&((val) <= RCU_PLLSAIR_DIV_MAX))

/* RCU_ADDCTL register bit define */
/* 48MHz clock selection */ 
#define RCU_CK48MSRC_PLL48M             ((uint32_t)0x00000000U)            /*!< CK48M source clock select PLL48M */
#define RCU_CK48MSRC_IRC48M             RCU_ADDCTL_CK48MSEL                /*!< CK48M source clock select IRC48M */

/* PLL48M clock selection */
#define RCU_PLL48MSRC_PLLQ              ((uint32_t)0x00000000U)            /*!< PLL48M source clock select PLLQ */
#define RCU_PLL48MSRC_PLLSAIP           RCU_ADDCTL_PLL48MSEL               /*!< PLL48M source clock select PLLSAIP */

/* Deep-sleep mode voltage */
#define DSV_DSLPVS(regval)              (BITS(0,2) & ((uint32_t)(regval) << 0))
#define RCU_DEEPSLEEP_V_1_2             DSV_DSLPVS(0)                      /*!< core voltage is 1.2V in deep-sleep mode */
#define RCU_DEEPSLEEP_V_1_1             DSV_DSLPVS(1)                      /*!< core voltage is 1.1V in deep-sleep mode */
#define RCU_DEEPSLEEP_V_1_0             DSV_DSLPVS(2)                      /*!< core voltage is 1.0V in deep-sleep mode */
#define RCU_DEEPSLEEP_V_0_9             DSV_DSLPVS(3)                      /*!< core voltage is 0.9V in deep-sleep mode */
/** @defgroup RCC_I2S_Clock_Source
  * @{
  */
#define RCU_I2S2CLKSource_PLLI2S             ((uint8_t)0x00)
#define RCU_I2S2CLKSource_Ext                ((uint8_t)0x01)

#define IS_RCC_I2SCLK_SOURCE(SOURCE) (((SOURCE) == RCU_I2S2CLKSource_PLLI2S) || ((SOURCE) == RCU_I2S2CLKSource_Ext))  

void RCU_GetClocksFreq(RCU_ClocksTypeDef* RCU_Clocks);
/* Peripheral clocks configuration functions **********************************/
void RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource);

void RCU_AHB1PeriphClockCmd(RCU_AHB1PERIPH_ENUM RCU_AHB1Periph, ControlStatus NewState);
void RCU_AHB2PeriphClockCmd(RCU_AHB2PERIPH_ENUM RCU_AHB2Periph, ControlStatus NewState);
void RCU_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, ControlStatus NewState);
void RCU_APB1PeriphClockCmd(RCU_APB1PERIPH_ENUM RCU_APB1Periph, ControlStatus NewState);
void RCU_APB2PeriphClockCmd(RCU_APB2PERIPH_ENUM RCU_APB2Periph, ControlStatus NewState);

void RCU_AHB1PeriphResetCmd(RCU_AHB1PERIPH_ENUM RCU_AHB1Periph, ControlStatus NewState);
void RCU_AHB2PeriphResetCmd(RCU_AHB2PERIPH_ENUM RCU_AHB2Periph, ControlStatus NewState);
void RCU_AHB3PeriphResetCmd(uint32_t RCU_AHB3Periph, ControlStatus NewState);
void RCU_APB1PeriphResetCmd(RCU_APB1PERIPH_ENUM RCU_APB1Periph, ControlStatus NewState);
void RCU_APB2PeriphResetCmd(RCU_APB2PERIPH_ENUM RCU_APB2Periph, ControlStatus NewState);

void RCU_AHB1PeriphClockSPModeCmd(RCU_AHB1PERIPH_ENUM RCU_AHB1Periph, ControlStatus NewState);
void RCU_AHB2PeriphClockSPModeCmd(RCU_AHB2PERIPH_ENUM RCU_AHB2Periph, ControlStatus NewState);
void RCU_AHB3PeriphClockSPModeCmd(uint32_t RCU_AHB3Periph, ControlStatus NewState);
void RCU_APB1PeriphClockSPModeCmd(RCU_APB1PERIPH_ENUM RCU_APB1Periph, ControlStatus NewState);
void RCU_APB2PeriphClockSPModeCmd(RCU_APB2PERIPH_ENUM RCU_APB2Periph, ControlStatus NewState);

/* Interrupts and flags management functions **********************************/
void RCU_ITConfig(RCU_INT_ENUM RCU_IT, ControlStatus NewState);
FlagStatus RCU_GetFlagStatus(RCU_FLAG_ENUM RCU_FLAG);
void RCU_ClearFlag(void);
FlagStatus RCU_GetITStatus(RCU_INT_FLAG_ENUM RCU_IT);
void RCU_ClearITPendingBit(RCU_INT_FLAG_CLEAR_ENUM RCU_IT);
#endif /* GD32F4XX_RCU_H */
