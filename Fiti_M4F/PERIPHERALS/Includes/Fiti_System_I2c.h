/**
  **************************************************************************
  * @file     Fiti_System_I2c.h
  * @brief    Fiti_System_I2c i2c header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __Fiti_System_I2C_H
#define __Fiti_System_I2C_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "Fiti_M4.h"

#pragma anon_unions

/** @addtogroup AT32F425_periph_driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/**
  * @brief maximum number of single transfers
  */
#define MAX_TRANSFER_CNT                 255 /*!< maximum number of single transfers */

/** @defgroup I2C_interrupts_definition
  * @brief i2c interrupt
  * @{
  */

#define I2C_TxDITEN                      ((uint32_t)0x01000000) /*!< i2c transmit data interrupt */
#define I2C_RxDITEN                      ((uint32_t)0x02000000) /*!< i2c receive data interrupt */
#define I2C_ERRITEN                      ((uint32_t)0x04000000) /*!< i2c bus error interrupt */
#define I2C_EVTITEN                      ((uint32_t)0x08000000) /*!< i2c bus event interrupt */
#define I2C_BUFITEN                      ((uint32_t)0x10000000) /*!< i2c bus buffer interrupt */

/**
  * @}
  */

/** @defgroup I2C_flags_definition
  * @brief i2c flag
  * @{
  */

// #define  I2C_MSM_STATUS                  ((uint32_t)0x00000001) /*!< Master or slave mode (Not trigger interrupt) */
// #define  I2C_TRM_STATUS                  ((uint32_t)0x00000002) /*!< Transmitted or received mode (Not trigger interrupt) */
#define  I2C_SB_EVT                      ((uint32_t)0x00000004) /*!< Start bit event flag (master mode) */
#define  I2C_STOPF_EVT                   ((uint32_t)0x00000008) /*!< STOP detection event flag (slave mode) */
#define  I2C_BUSY_EVT                    ((uint32_t)0x00000010) /*!< Bus busy event */
#define  I2C_GCAR_EVT                    ((uint32_t)0x00000020) /*!< General call address receive event(slave mode) flag */

#define  I2C_TXE_EVT                     ((uint32_t)0x00000100) /*!< Tx data register empty event */
#define  I2C_RXNE_EVT                    ((uint32_t)0x00000200) /*!< Rx data register not empty event */
#define  I2C_TXIT_EVT                    ((uint32_t)0x00000400) /*!< Transmit data interrupt event */
#define  I2C_TDC_EVT                     ((uint32_t)0x00000800) /*!< Transfer data complete event */
#define  I2C_TDRLD_EVT                   ((uint32_t)0x00001000) /*!< Transmission is complete, waiting to load data */

#define  I2C_AF_ERR                      ((uint32_t)0x00010000) /*!< Acknowledge failure flag */
#define  I2C_BUSERR_ERR                  ((uint32_t)0x00020000) /*!< Bus error flag */
#define  I2C_ARLO_ERR                    ((uint32_t)0x00040000) /*!< Arbitration lost error flag (master mode) */
#define  I2C_OVR_ERR                     ((uint32_t)0x00080000) /*!< Overrun/Underrun error flag */
#define  I2C_TIMEOUT_ERR                 ((uint32_t)0x00100000) /*!< Timeout error flag */
#define  I2C_ADDR_EVT                    ((uint32_t)0x00200000) /*!< Address sent event (master mode)/matched event flag (slave mode) */

/**
  * @}
  */

/** @defgroup I2C_exported_types
  * @{
  */

/**
  * @brief i2c transfer direction
  */
typedef enum
{
  I2C_SLAVE_MODE                         = 0x00, /*!< I2C slave mode */
  I2C_MASTER_MODE                        = 0x01  /*!< I2C master mode */
} i2c_msm_mode_type;

/**
  * @brief i2c address mode
  */
typedef enum
{
  I2C_ADDRESS_MODE_7BIT                  = 0x00, /*!< 7bit address mode */
  I2C_ADDRESS_MODE_10BIT                 = 0x01  /*!< 10bit address mode */
} i2c_address_mode_type;

/**
  * @brief i2c transfer direction
  */
typedef enum
{
  I2C_DIR_TRANSMIT                       = 0x00, /*!< master request a write transfer */
  I2C_DIR_RECEIVE                        = 0x01  /*!< master request a read transfer */
} i2c_transfer_dir_type;

/**
  * @brief i2c dma requests direction
  */
typedef enum
{
  I2C_DMA_REQUEST_TX                     = 0x00, /*!< dma transmit request */
  I2C_DMA_REQUEST_RX                     = 0x01  /*!< dma receive request */
} i2c_dma_request_type;


/**
  * @brief i2c clock timeout detection mode
  */
typedef enum
{
  I2C_TIMEOUT_DETCET_LOW                 = 0x00, /*!< detect low level timeout */
  I2C_TIMEOUT_DETCET_HIGH                = 0x01  /*!< detect high level timeout */
} i2c_timeout_detcet_type;


/**
  * @brief i2c reload end mode
  */
typedef enum
{
  I2C_AUTO_STOP_MODE                     = 0x00004000, /*!< auto generate stop mode */
  I2C_SOFT_STOP_MODE                     = 0x00000000, /*!< soft generate stop mode */
  I2C_RELOAD_MODE                        = 0x00002000  /*!< reload mode */
} i2c_reload_stop_mode_type;

/**
  * @brief i2c start mode
  */
typedef enum
{
  I2C_WITHOUT_START                      = 0x00000000, /*!< transfer data without start condition */
  I2C_GEN_START_READ                     = 0x00002400, /*!< read data and generate start, ctrl2.startgen = 1, ctrl2.mdir = 1*/
  I2C_GEN_START_WRITE                    = 0x00002000  /*!< send data and generate start, ctrl2.startgen = 1, ctrl2.mdir = 0 */
} i2c_start_mode_type;

/**
  * @brief type define i2c register all
  */
typedef struct
{
  /**
    * @brief i2c ctrl1 register, offset:0x00
    */
  union
  {
    __IO uint32_t ctrl1;
    struct
    {
      __IO uint32_t i2cen                : 1; /* [0] I2CEN : I2C function enable */
      __IO uint32_t dmatxen              : 1; /* [1] DMATxEN : DMA transmitting data enable */
      __IO uint32_t dmarxen              : 1; /* [2] DMARxEN : DMA receiving data enable */
      __IO uint32_t slvctrlen            : 1; /* [3] SLVCTRLEN : Slave receiving data control enable (Slave mode) */
      __IO uint32_t stretchen            : 1; /* [4] STRETCHEN : Clock stretching enable (Slave mode) */
      __IO uint32_t gcaen                : 1; /* [5] GCAEN : General call address enable */
      __IO uint32_t reserved1            : 2; /* [7:6] Reserved */

      __IO uint32_t dfltval              : 4; /* [11:8] DFLTVAL: Digital filter value */  
      __IO uint32_t dflten               : 1; /* [12] DFLTEN : Digital fileter enable (both in SDA/SCL) */
      __IO uint32_t rlden                : 1; /* [13] RLDEN : Reload function (Rx/Tx over 255 byte data) enable */
      __IO uint32_t astopen              : 1; /* [14] ASTOPEN : Automatically send STOP condition */
      __IO uint32_t swrst                : 1; /* [15] SWRST : Software reset, which can release I2C from error/locked state */
      __IO uint32_t reserved2            : 22; /* [31:16] Reserved */
    } ctrl1_bit;
  };

  /**
    * @brief i2c ctrl2 register, offset:0x04
    */
  union
  {
    __IO uint32_t ctrl2;
    struct
    {
      __IO uint32_t saddr                : 10; /* [9:0] SADDR : The slave address sent by the master */
      __IO uint32_t mdir                 : 1; /* [10] MDIR : Master data transmit direction */
      __IO uint32_t addr10en             : 1; /* [11] ADDR10EN : Host send 10-bit address mode enable (Master mode) */
      __IO uint32_t timouten             : 1; /* [12] TIMOUTEN : Timout function enable */
      __IO uint32_t startgen             : 1; /* [13] STARTGEN : START condition generation */
      __IO uint32_t stopgen              : 1; /* [14] STOPGEN : STOP condition generation */      
      __IO uint32_t nacken               : 1; /* [15] NACKEN : Return non-acknowledge enable  */

      __IO uint32_t dacnt                : 8; /* [23:16] DACNT : Transmit data counter */

      __IO uint32_t txditen              : 1; /* [24] TxDITEN : Transmit data interrupt enable */
      __IO uint32_t rxditen              : 1; /* [25] RxDITEN : Receive data interrupt enable */
      __IO uint32_t erriten              : 1; /* [26] ERRITEN : Error interrupt enable */
      __IO uint32_t evtiten              : 1; /* [27] EVTITEN : Event interrupt enable */
      __IO uint32_t bufiten              : 1; /* [28] BUFITEN : Buffer interrupt enable */
      __IO uint32_t reserved2            : 3; /* [31:29] Reserved */
    } ctrl2_bit;
  };

  /**
    * @brief i2c oaddr1 register, offset:0x08
    */
  union
  {
    __IO uint32_t oaddr1;
    struct
    {
      __IO uint32_t addr1                : 10;/* [9:0] ADDR1 : Interface address 1 */
      __IO uint32_t reserved1            : 5; /* [14:10] Reserved */
      __IO uint32_t addr1mode            : 1; /* [15] ADDR1MODE : Address mode (slave mode) */
      __IO uint32_t reserved2            : 16;/* [31:16] Reserved */
    } oaddr1_bit;
  };

  /**
    * @brief i2c oaddr2 register, offset:0x0c
    */
  union
  {
    __IO uint32_t oaddr2;
    struct
    {
      __IO uint32_t addr2                : 7; /* [6:0] ADDR2 : Interface address 2 (only 7 bits address mode) */
      __IO uint32_t reserved1            : 8; /* [14:7] Reserved */
      __IO uint32_t addr2en              : 1; /* [15] ADDR2EN: Interface address 2 enable */
      __IO uint32_t reserved2            : 16;/* [31:16] Reserved */
    } oaddr2_bit;
  };

  /**
    * @brief i2c data register, offset:0x10
    */
  union
  {
    __IO uint32_t dr;
    struct
    {
      __IO uint32_t txdr                 : 8; /* [7:0] Transmit 8 bits Data Register */
      __I uint32_t rxdr                  : 8; /* [15:8] Receive 8 bits Data Register */
      __IO uint32_t reserved1            : 16;/* [31:16] Reserved */
    } dr_bit;
  };

  /**
    * @brief i2c status register, offset:0x18
    */
  union
  {
    __IO uint32_t sr;
    struct
    {
      __I uint32_t msm                   : 1; /* [0] MSM : Master or slave mode */
      __I uint32_t trm                   : 1; /* [1] TRM : Transmitted or received mode */
      __I uint32_t sb                    : 1; /* [2] SB : Start bit event (master mode) */
      __I uint32_t stop                  : 1; /* [3] STOP : STOP detection event (slave mode) */
      __I uint32_t busy                  : 1; /* [4] BUSY : Bus busy */
      __I uint32_t gcar                  : 1; /* [5] GCAR : General call address (slave mode) */
      __I uint32_t reserved1             : 2; /* [7:6] Reserved */

      __I uint32_t txe                   : 1; /* [8] TxE : Tx data register empty event */
      __I uint32_t rxne                  : 1; /* [9] RxNE : Rx data register not empty event */
      __I uint32_t txit                  : 1; /* [10] TxE :Transmit data interrupt event */      
      __I uint32_t tdc                   : 1; /* [11] TDC : Transfer data complete event */
      __I uint32_t tcrld                 : 1; /* [12] TCRLD : Transmission is complete, waiting to load data */
      __I uint32_t reserved2             : 3; /* [15:13] Reserved */

      __I uint32_t af                    : 1; /* [16] AF : Acknowledge failure */
      __I uint32_t buserr                : 1; /* [17] BUSERR : Bus error */
      __I uint32_t arlo                  : 1; /* [18] ARLO : Arbitration lost error (master mode) */
      __I uint32_t ovr                   : 1; /* [19] OVR : Overrun/Underrun error */
      __I uint32_t timeout               : 1; /* [20] TIMEOUT: Timeout error */
      __I uint32_t addrevt               : 1; /* [21] ADDREVT : Address sent event (master mode)/matched event(slave mode) */
      __I uint32_t reserved3             : 19;/* [23:22] Reserved */

      __I uint32_t addrselm              : 1; /* [24] ADDREVT : ADDR select flag, match ADDR1/2 (slave mode) */
      __I uint32_t saddrm                : 1; /* [31:25] SADDRM : Slave address matching value */
    } sr_bit;
  };

  /**
    * @brief i2c clr register, offset:0x1c
    */
  union
  {
    __IO uint32_t clr;
    struct
    {
      __IO uint32_t reserved1            : 2; /* [1:0] Reserved */
      __IO uint32_t sbc                  : 1; /* [2] SBC: Set 1 to clear I2C_SR_SB register */
      __IO uint32_t stopc                : 1; /* [3] STOPC: Set 1 to clear I2C_SR_STOP register */
      __IO uint32_t busyc                : 1; /* [4] BUSYC: Set 1 to clear I2C_SR_BUSY register */
      __IO uint32_t reserved2            : 11; /* [15:5] Reserved */

      // __IO uint32_t txec                 : 1; /* [8] TXEC: Set 1 to clear I2C_SR_TXE register */
      // __IO uint32_t rxnec                : 1; /* [9] RXNEC: Set 1 to clear I2C_SR_RXNE register */
      // __IO uint32_t txitc                : 1; /* [10] TXITC: Set 1 to clear I2C_SR_TXITE register */
      // __IO uint32_t reserved3            : 5; /* [15:11] Reserved */

      __IO uint32_t afc                  : 1; /* [16] AFC: Set 1 to clear I2C_SR_AF register */
      __IO uint32_t buserrc              : 1; /* [17] BUSERRC: Set 1 to clear I2C_SR_BUSERR register */
      __IO uint32_t arloc                : 1; /* [18] ARLOC: Set 1 to clear I2C_SR_ARLO register */ 
      __IO uint32_t ovrc                 : 1; /* [19] OVRC: Set 1 to clear I2C_SR_OVR register */
      __IO uint32_t timeoutc             : 1; /* [20] TIMEOUTC: Set 1 to clear I2C_SR_TIMEOUT register */
      __IO uint32_t addrc                : 1; /* [21] ADDRC: Set 1 to clear I2C_SR_ADDR register */
      __IO uint32_t reserved4            : 10;/* [31:22] Reserved */
    } clr_bit;
  };


  /**
    * @brief i2c clkctrl register, offset:0x10
    */
  union
  {
    __IO uint32_t clkctrl;
    struct
    {
      __IO uint32_t scllvt               : 8; /* [7:0] SCLLVT: SCL low level time */
      __IO uint32_t sclhvt               : 8; /* [15:8] SCLHVT: SCL high level time */
      __IO uint32_t sdadlt               : 4; /* [19:16] SDADLT: SDA output delay time */
      __IO uint32_t scldlt               : 4; /* [23:20] SCLDLT: SCL output delay time */
      __IO uint32_t div                  : 4; /* [31:24] DIV : Clock divider value */
    } clkctrl_bit;
  };

  /**
    * @brief i2c timeout register, offset:0x14
    */
  union
  {
    __IO uint32_t tmr;
    struct
    {
      __IO uint32_t tmotime               : 12;/* [11:0] TMOTIME : Timeout detection time */
      __IO uint32_t tmomode               : 1; /* [12] TMOMODE: Timeout detection mode */
      __IO uint32_t reserved1             : 2; /* [31:13] Reserved */
    } tmr_bit;
  };


} i2c_type;

/**
  * @}
  */

#define I2C1                             ((i2c_type *) I2C1_BASE)
#define I2C2                             ((i2c_type *) I2C2_BASE)

/** @defgroup I2C_exported_functions
  * @{
  */

void i2c_reset(i2c_type *i2c_x);
void i2c_init(i2c_type *i2c_x, uint8_t dfilters, uint32_t clk);
void i2c_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_dma_enable(i2c_type *i2c_x, i2c_dma_request_type dma_req, confirm_state new_state);
void i2c_slave_data_ctrl_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_clock_stretch_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_general_call_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_dflt_val_set(i2c_type *i2c_x, uint8_t dfilters);
void i2c_dflt_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_reload_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_auto_stop_enable(i2c_type *i2c_x, confirm_state new_state);

void i2c_transfer_addr_set(i2c_type *i2c_x, uint16_t address);
uint16_t i2c_transfer_addr_get(i2c_type *i2c_x);
void i2c_transfer_dir_set(i2c_type *i2c_x, i2c_transfer_dir_type i2c_direction);
void i2c_addr10_header_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_timeout_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_start_generate(i2c_type *i2c_x);
void i2c_stop_generate(i2c_type *i2c_x);
void i2c_ack_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_cnt_set(i2c_type *i2c_x, uint8_t cnt);
void i2c_interrupt_enable(i2c_type *i2c_x, uint32_t source, confirm_state new_state);
flag_status i2c_interrupt_get(i2c_type *i2c_x, uint16_t source);

void i2c_own_address1_set(i2c_type *i2c_x, i2c_address_mode_type mode, uint16_t address);
void i2c_own_address2_set(i2c_type *i2c_x, uint8_t address);
void i2c_own_address2_enable(i2c_type *i2c_x, confirm_state new_state);

void i2c_data_send(i2c_type *i2c_x, uint8_t data);
uint8_t i2c_data_receive(i2c_type *i2c_x);

i2c_msm_mode_type i2c_msm_mode_get(i2c_type *i2c_x);
i2c_transfer_dir_type i2c_transfer_dir_get(i2c_type *i2c_x);
flag_status i2c_flag_get(i2c_type *i2c_x, uint32_t flag);
uint8_t i2c_matched_addr_get(i2c_type *i2c_x);
void i2c_flag_clear(i2c_type *i2c_x, uint32_t flag);

void i2c_timeout_time_set(i2c_type *i2c_x, uint16_t timeout);
void i2c_timeout_mode_set(i2c_type *i2c_x, i2c_timeout_detcet_type mode);

void i2c_transmit_set(i2c_type *i2c_x, uint16_t address, uint8_t cnt, i2c_reload_stop_mode_type rld_stop, i2c_start_mode_type start);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
