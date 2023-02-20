/**
  **************************************************************************
  * @file     FA8801_i2c.h
  * @brief    FA8801 i2c header file
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Fitipower official website is the copyrighted work of Fitipower.
  * Fitipower authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Fitipower microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. Fitipower EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __FA8801_I2C_H
#define __FA8801_I2C_H


/* includes ------------------------------------------------------------------*/
#include "FA8801.h"

#pragma anon_unions

/** @addtogroup FA8801_periph_driver
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

#define I2C_ERRITEN                      ((uint32_t)0x01000000) /*!< i2c bus error interrupt */
#define I2C_EVTITEN                      ((uint32_t)0x02000000) /*!< i2c bus event interrupt */
#define I2C_BUFITEN                      ((uint32_t)0x04000000) /*!< i2c bus buffer interrupt */

/**
  * @}
  */

/** @defgroup I2C_flags_definition
  * @brief i2c flag
  * @{
  */

#define  I2C_SB_EVT                      ((uint32_t)0x00000001) /*!< Start bit event flag (master mode) */
#define  I2C_ADDR_EVT                    ((uint32_t)0x00000002) /*!< Address sent event (master mode)/matched event flag (slave mode) */
#define  I2C_ADDR10_EVT                  ((uint32_t)0x00000004) /*!< 10-bit address header sent event flag (master mode) */
#define  I2C_DBTF_EVT                    ((uint32_t)0x00000008) /*!< Data byte transfer finish event flag */
#define  I2C_STOPF_EVT                   ((uint32_t)0x00000010) /*!< STOP detection event flag (slave mode) */
#define  I2C_RXNE_EVT                    ((uint32_t)0x00000020) /*!< Rx data register not empty event */
#define  I2C_TXE_EVT                     ((uint32_t)0x00000040) /*!< Tx data register empty event */

#define  I2C_BERR_ERR                    ((uint32_t)0x00000100) /*!< Bus error flag */
#define  I2C_ARLO_ERR                    ((uint32_t)0x00000200) /*!< Arbitration lost error flag (master mode)*/
#define  I2C_AF_ERR                      ((uint32_t)0x00000400) /*!< Acknowledge failure flag */
#define  I2C_OVR_ERR                     ((uint32_t)0x00000800) /*!< Overrun/Underrun error flag */
#define  I2C_TIMEOUT_ERR                 ((uint32_t)0x00001000) /*!< Timeout error flag */

#define  I2C_MSM_STATUS                  ((uint32_t)0x00000001) /*!< Master or slave mode */
#define  I2C_BUSY_STATUS                 ((uint32_t)0x00000002) /*!< Address sent event (master mode)/matched event flag (slave mode) */
#define  I2C_TRM_EVT                     ((uint32_t)0x00000004) /*!< 10-bit address header sent event flag (master mode) */
#define  I2C_GCA_EVT                     ((uint32_t)0x00000008) /*!< Data byte transfer finish event flag */
#define  I2C_ADDRF_EVT                   ((uint32_t)0x00000010) /*!< STOP detection event flag (slave mode) */
#define  I2C_SDIR_EVT                    ((uint32_t)0x00000020) /*!< Rx data register not empty event */

/**
  * @}
  */

/** @defgroup I2C_exported_types
  * @{
  */

/**
  * @brief i2c address mode
  */
typedef enum
{
  I2C_ADDRESS_MODE_7BIT                  = 0x00, /*!< 7bit address mode */
  I2C_ADDRESS_MODE_10BIT                 = 0x01  /*!< 10bit address mode */
} i2c_address_mode_type;

/**
  * @brief i2c dma requests direction
  */
typedef enum
{
  I2C_DMA_REQUEST_TX                     = 0x00, /*!< dma transmit request */
  I2C_DMA_REQUEST_RX                     = 0x01  /*!< dma receive request */
} i2c_dma_request_type;

/**
  * @brief i2c reload end mode
  */
typedef enum
{
  I2C_AUTO_STOP_MODE                     = 0x02000000, /*!< auto generate stop mode */
  I2C_SOFT_STOP_MODE                     = 0x00000000, /*!< soft generate stop mode */
  I2C_RELOAD_MODE                        = 0x01000000  /*!< reload mode */
} i2c_reload_stop_mode_type;

/**
  * @brief i2c start mode
  */
typedef enum
{
  I2C_WITHOUT_START                      = 0x00000000, /*!< transfer data without start condition */
  I2C_GEN_START_READ                     = 0x00002400, /*!< read data and generate start */
  I2C_GEN_START_WRITE                    = 0x00002000  /*!< send data and generate start */
} i2c_start_mode_type;

/**
  * @brief i2c master/slave mode
  */
typedef enum
{
  I2C_SLAVE_MODE                         = 0x00, /*!< i2c slave mode */
  I2C_MASTER_MODE                        = 0x01  /*!< i2c master mode */
} i2c_msm_mode_type;

/**
  * @brief i2c transfer direction
  */
typedef enum
{
  I2C_DIR_TRANSMIT                       = 0x00, /*!< i2c write transfer direction */
  I2C_DIR_RECEIVE                        = 0x01  /*!< i2c read transfer direction */
} i2c_transfer_dir_type;

/**
  * @brief i2c busy status
  */
typedef enum
{
  I2C_NONE_BUSY                          = 0x00, /*!< i2c No communication on the bus */
  I2C_BUSY                               = 0x01  /*!< i2c Communication ongoing on the bus */
} i2c_busy_type;

/**
  * @brief i2c genreal call type
  */
typedef enum
{
  I2C_NO_GENCALL                          = 0x00, /*!< i2c No general call */
  I2C_RECEIVE_GENCALL                     = 0x01  /*!< i2c general call address received when GCEN=1 */
} i2c_gca_type;

/**
  * @brief i2c addr select flag
  */
typedef enum
{
  I2C_MATCH_ADDR1                          = 0x00, /*!< i2c No general call */
  I2C_MATCH_ADDR2                     = 0x01  /*!< i2c general call address received when GCEN=1 */
} i2c_addrf_type;


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
      __IO uint32_t gcen                 : 1; /* [3] GCEN : General call enable */
      __IO uint32_t rlden                : 1; /* [4] RLDEN : Reload function (Rx/Tx over 255 byte data) enable */
      __IO uint32_t dflten               : 1; /* [5] DFLTEN : Digital fileter enable (both in SDA/SCL) */
      __IO uint32_t stretchen            : 1; /* [6] STRETCHEN : Clock stretching enable (Slave mode) */
      __IO uint32_t acken                : 1; /* [7] ACKEN : Return acknowledge enable  */
      
      __IO uint32_t dfltval              : 1; /* [12:9] DFLTVAL: Digital filter value */
      __IO uint32_t swrst                : 1; /* [13] SWRST : Software reset, which can release I2C from error/locked state */
      __IO uint32_t reserved1            : 22; /* [31:14] Reserved */
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
      __IO uint32_t slvdcen              : 1; /* [11] SRxDCEN : Slave receiving data control enable (Slave mode) */
      __IO uint32_t startgen             : 1; /* [12] STARTGEN : START condition generation */
      __IO uint32_t stopgen              : 1; /* [13] STOPGEN : STOP condition generation */
      __IO uint32_t astopen              : 1; /* [14] ASTOPEN : Automatically send STOP condition */
      __IO uint32_t timouten             : 1; /* [15] TIMOUTEN : Timout function enable */

      __IO uint32_t dacnt                : 8; /* [23:16] DACNT : Transmit data counter */
      
      __IO uint32_t erriten              : 1; /* [23] ERRITEN : Error interrupt enable */
      __IO uint32_t evtiten              : 1; /* [24] EVTITEN : Event interrupt enable */
      __IO uint32_t bufiten              : 1; /* [25] BUFITEN : Buffer interrupt enable */
      __IO uint32_t reserved1            : 13; /* [31:26] Reserved */
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
    * @brief i2c status 1 register, offset:0x14
    */
  union
  {
    __I uint32_t sr1;
    struct
    {
      __I uint32_t sb                   : 1; /* [0] SB : Start bit event (master mode) */
      __I uint32_t addr                 : 1; /* [1] ADDR : Address sent event (master mode)/matched event(slave mode) */
      __I uint32_t addr10               : 1; /* [2] ADDR10 : 10-bit address header sent event(master mode) */
      __I uint32_t dbtf                 : 1; /* [3] DBTF : Data byte transfer finish event */
      __I uint32_t stopf                : 1; /* [4] STOPF : STOP detection event (slave mode) */
      __I uint32_t rxne                 : 1; /* [5] RxNE : Rx data register not empty event */
      __I uint32_t txe                  : 1; /* [6] TxE : Tx data register empty event */
      __I uint32_t reserved1            : 1; /* [7] Reserved */

      __I uint32_t berr                 : 1; /* [8] BERR : Bus error */
      __I uint32_t arlo                 : 1; /* [9] ARLO : Arbitration lost error (master mode) */
      __I uint32_t af                   : 1; /* [10] AF : Acknowledge failure */
      __I uint32_t ovr                  : 1; /* [11] OVR : Overrun/Underrun error */
      __I uint32_t timeout              : 1; /* [12] TIMEOUT: Timeout error */
      __I uint32_t reserved2            : 19;/* [31:13] Reserved */

    } sr1_bit;
  };

  /**
    * @brief i2c status 2 register, offset:0x18
    */
  union
  {
    __I uint32_t sr2;
    struct
    {
      __I uint32_t msm                  : 1; /* [0] MSM : Master or slave mode */
      __I uint32_t trm                  : 1; /* [1] TRM : Transmitted or received mode */
      __I uint32_t busy                 : 1; /* [2] BUSY : Bus busy */
      __I uint32_t gca                  : 1; /* [3] GCA : General call address (slave mode) */
      __I uint32_t addrf                : 1; /* [4] ADDRF : ADDR select flag (slave mode) */
      __I uint32_t reserved1            : 27;/* [31:5] Reserved */

    } sr2_bit;
  };

  /**
    * @brief i2c clkctrl register, offset:0x1C
    */
  union
  {
    __IO uint32_t clkctrl;
    struct
    {
      __IO uint32_t ccr                  : 12; /* [11:0] CCR : Clock control register in each speed mode */
      __IO uint32_t reserved1            : 2; /* [13:12] Reserved */
      __IO uint32_t spdm                 : 2; /* [15:14] SPDM : Speed mode */
      __IO uint32_t freq                 : 6; /* [21:16] FREQ : I2C Peripheral clock frequency */
      __IO uint32_t reserved2            : 10; /* [31:22] Reserved */
    } clkctrl_bit;
  };

} i2c_type;

/**
  * @}
  */

#define I2C1                             ((i2c_type *) I2C1_BASE)
#define I2C2                             ((i2c_type *) I2C2_BASE)
#define I2C3                             ((i2c_type *) I2C3_BASE)
#define I2C4                             ((i2c_type *) I2C4_BASE)

/** @defgroup I2C_exported_functions
  * @{
  */

void i2c_init(i2c_type *i2c_x, uint8_t dfilters, uint32_t clk);
void i2c_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_dma_enable(i2c_type *i2c_x, i2c_dma_request_type dma_req, confirm_state new_state);
void i2c_general_call_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_reload_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_dfilter_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_clock_stretch_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_ack_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_reset(i2c_type *i2c_x);

void i2c_transfer_addr_set(i2c_type *i2c_x, uint16_t address);
uint16_t i2c_transfer_addr_get(i2c_type *i2c_x);
void i2c_transfer_dir_set(i2c_type *i2c_x, i2c_transfer_dir_type i2c_direction);
void i2c_slave_data_ctrl_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_start_generate(i2c_type *i2c_x);
void i2c_stop_generate(i2c_type *i2c_x);
void i2c_auto_stop_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_timeout_enable(i2c_type *i2c_x, confirm_state new_state);
void i2c_cnt_set(i2c_type *i2c_x, uint8_t cnt);
void i2c_interrupt_enable(i2c_type *i2c_x, uint32_t source, confirm_state new_state);
flag_status i2c_interrupt_get(i2c_type *i2c_x, uint16_t source);

void i2c_own_address1_set(i2c_type *i2c_x, i2c_address_mode_type mode, uint16_t address);
void i2c_own_address2_set(i2c_type *i2c_x, uint8_t address);
void i2c_own_address2_enable(i2c_type *i2c_x, confirm_state new_state);

void i2c_data_send(i2c_type *i2c_x, uint8_t data);
uint8_t i2c_data_receive(i2c_type *i2c_x);
flag_status i2c_flag_get(i2c_type *i2c_x, uint32_t flag);

i2c_msm_mode_type i2c_ms_mode_get(i2c_type *i2c_x);
i2c_transfer_dir_type i2c_transfer_dir_get(i2c_type *i2c_x);
i2c_busy_type i2c_busy_get(i2c_type *i2c_x);
i2c_gca_type i2c_gca_get(i2c_type *i2c_x);
i2c_addrf_type i2c_addrf_get(i2c_type *i2c_x);

void i2c_clk_set(i2c_type *i2c_x, uint8_t mode, uint32_t pclk,  uint32_t ccr);
void i2c_transmit_set(i2c_type *i2c_x, uint16_t address, uint8_t cnt, i2c_reload_stop_mode_type rld_stop, i2c_start_mode_type start);
void i2c_flag_clear(i2c_type *i2c_x, uint32_t flag);

#endif
