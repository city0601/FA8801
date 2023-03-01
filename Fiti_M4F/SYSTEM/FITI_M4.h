/**************************************************************************//**
 * @file     ARMCM4_FP.h
 * @brief    CMSIS Core Peripheral Access Layer Header File for
 *           ARMCM4 Device (configured for CM4 with FPU)
 * @version  V5.3.1
 * @date     09. July 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FITI_M4_H
#define FITI_M4_H

#ifdef __cplusplus
extern "C" {
#endif

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if defined (__CC_ARM)
 #pragma anon_unions
#endif

/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Processor Exceptions Numbers  ----------------------------- */
  NonMaskableInt_IRQn           = -14,     /*  2 Non Maskable Interrupt */
  HardFault_IRQn                = -13,     /*  3 HardFault Interrupt */
  MemoryManagement_IRQn         = -12,     /*  4 Memory Management Interrupt */
  BusFault_IRQn                 = -11,     /*  5 Bus Fault Interrupt */
  UsageFault_IRQn               = -10,     /*  6 Usage Fault Interrupt */
  SVCall_IRQn                   =  -5,     /* 11 SV Call Interrupt */
  DebugMonitor_IRQn             =  -4,     /* 12 Debug Monitor Interrupt */
  PendSV_IRQn                   =  -2,     /* 14 Pend SV Interrupt */
  SysTick_IRQn                  =  -1,     /* 15 System Tick Interrupt */

/* -------------------  Processor Interrupt Numbers  ------------------------------ */
  Interrupt0_IRQn               =   0,
  Interrupt1_IRQn               =   1,
  Interrupt2_IRQn               =   2,
  Interrupt3_IRQn               =   3,
  Interrupt4_IRQn               =   4,
  Interrupt5_IRQn               =   5,
  Interrupt6_IRQn               =   6,
  Interrupt7_IRQn               =   7,
  Interrupt8_IRQn               =   8,
  Interrupt9_IRQn               =   9
  /* Interrupts 10 .. 224 are left out */
} IRQn_Type;

/** @addtogroup Exported_types
  * @{
  */

typedef enum
{
  RESET = 0,
  SET = !RESET
}FlagStatus,ITStatus;

typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
}FunctionalState;


typedef enum 
{
 FALSE = 0, 
 TRUE = !FALSE
} CONFIRM_STATE;

typedef enum
{
  ERROR = 0,
  SUCCESS = !ERROR
}ErrorStatus;

/**
  * @}
  */

/** @addtogroup Exported_macro
  * @{
  */
#define REG8(addr)                       *(volatile uint8_t *)(addr)
#define REG16(addr)                      *(volatile uint16_t *)(addr)
#define REG32(addr)                      *(volatile uint32_t *)(addr)

#define MAKE_VALUE(reg_offset, bit_num)  (uint32_t)(((reg_offset) << 16) | (bit_num & 0x1F))

#define PERIPH_REG(periph_base, value)   REG32((periph_base + (value >> 16)))
#define PERIPH_REG_BIT(value)            (0x1U << (value & 0x1F))

/**
  * @}
  */

/* --------  Configuration of Core Peripherals  ----------------------------------- */
#define __CM4_REV                 0x0001U   /* Core revision r0p1 */
#define __MPU_PRESENT             1U        /* MPU present */
#define __VTOR_PRESENT            1U        /* VTOR present */
#define __NVIC_PRIO_BITS          3U        /* Number of Bits used for Priority Levels */
#define __Vendor_SysTickConfig    0U        /* Set to 1 if different SysTick Config is used */
#define __FPU_PRESENT             1U        /* FPU present */

#include "core_cm4.h"                       /* Processor and core peripherals */
#include "system_FITI_M4.h"                  /* System Header */

/** @addtogroup Peripheral_memory_map
  * @{
  */

#define FLASH_BASE                       ((uint32_t)0x08000000)
#define USD_BASE                         ((uint32_t)0x1FFFF800)
#define SRAM_BASE                        ((uint32_t)0x20000000)
#define PERIPH_BASE                      ((uint32_t)0x40000000)
#define NPU_PERIPH_BASE                  ((uint32_t)0x50000000)
#define DEBUG_BASE                       ((uint32_t)0xE0042000)

#define APB1PERIPH_BASE                  (PERIPH_BASE + 0x00000)
#define APB2PERIPH_BASE                  (PERIPH_BASE + 0x10000)
#define AHBPERIPH1_BASE                  (PERIPH_BASE + 0x30000)
#define AHBPERIPH2_BASE                  (PERIPH_BASE + 0x8000000)

#define NPU_APB1PERIPH_BASE              (NPU_PERIPH_BASE + 0x00000)
#define NPU_AHB1PERIPH_BASE              (NPU_PERIPH_BASE + 0x10000)
#define NPU_AXI1PERIPH_BASE              (NPU_PERIPH_BASE + 0x20000)

/* apb1 bus base address */
#define TMR1_BASE                        (APB1PERIPH_BASE + 0x0000)
#define TMR2_BASE                        (APB1PERIPH_BASE + 0x1000)
#define TMR3_BASE                        (APB1PERIPH_BASE + 0x2000)
#define TMR4_BASE                        (APB1PERIPH_BASE + 0x3000)
#define TMR5_BASE                        (APB1PERIPH_BASE + 0x4000)
#define TMR6_BASE                        (APB1PERIPH_BASE + 0x5000)
#define TMR7_BASE                        (APB1PERIPH_BASE + 0x6000)
#define TMR8_BASE                        (APB1PERIPH_BASE + 0x7000)
#define UART1_BASE											(APB1PERIPH_BASE + 0x8000)
#define UART2_BASE                      (APB1PERIPH_BASE + 0x9000)
#define UART3_BASE                      (APB1PERIPH_BASE + 0xA000)
#define UART4_BASE                      (APB1PERIPH_BASE + 0xB000)
#define I2C1_BASE                        (APB1PERIPH_BASE + 0xC000)
#define I2C2_BASE                        (APB1PERIPH_BASE + 0xD000)
#define I2C3_BASE                        (APB1PERIPH_BASE + 0xE000)
#define I2C4_BASE                        (APB1PERIPH_BASE + 0xF000)
/* apb2 bus base address */
#define SPI1_BASE                        (APB2PERIPH_BASE + 0x0000)
#define SPI2_BASE                        (APB2PERIPH_BASE + 0x1000)
#define SPI3_BASE                        (APB2PERIPH_BASE + 0x2000)
#define ADC1_BASE                        (APB2PERIPH_BASE + 0x3000)
#define DAC1_BASE                        (APB2PERIPH_BASE + 0x4000)
#define SCFG_BASE                        (APB2PERIPH_BASE + 0x5000)
#define EXINT_BASE                       (APB2PERIPH_BASE + 0x6000)
#define RTC_BASE                         (APB2PERIPH_BASE + 0x7000)
#define WDT_BASE                         (APB2PERIPH_BASE + 0x8000)
#define PWC_BASE                         (APB2PERIPH_BASE + 0x9000)
#define CRM_BASE                         (APB2PERIPH_BASE + 0xA000)
#define GPIOA_BASE                       (APB2PERIPH_BASE + 0xB000)
#define GPIOB_BASE                       (APB2PERIPH_BASE + 0xC000)
#define GPIOC_BASE                       (APB2PERIPH_BASE + 0xD000)
/* ahb bus base address */
#define DMA1_BASE                        (AHBPERIPH1_BASE + 0x0000)
#define OTGFS1_BASE                      (AHBPERIPH1_BASE + 0x1000)

/* npu apb bus base address */
#define I2S1_BASE                        (NPU_APB1PERIPH_BASE + 0x0000)
/* npu ahb bus base address */
#define DMA2_BASE                        (NPU_AHB1PERIPH_BASE + 0x0000)
/* npu axi bus base address */
#define QSPI_BASE                        (NPU_AXI1PERIPH_BASE + 0x0000)
#define DDRC_BASE                        (NPU_AXI1PERIPH_BASE + 0x1000)
#define ISP_BASE                        	(NPU_AXI1PERIPH_BASE + 0x2000)

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#include "Fiti_SYSTEM_DEF.h"
#include "Fiti_SYSTEM_CONF.h"

#ifdef __cplusplus
}
#endif

#endif  /* ARMCM4_FP_H */
