/**
  **************************************************************************
  * @file     FA8801_i2c.c
  * @brief    contains all the functions for the i2c firmware library
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

#include "FITI_M4.h"
#include "FA8801_i2c.h"

/** @addtogroup FA8801_periph_driver
  * @{
  */

/** @defgroup I2C
  * @brief I2C driver modules
  * @{
  */

#define I2C_MODULE_ENABLED

#ifdef I2C_MODULE_ENABLED

/** @defgroup I2C_private_functions
  * @{
  */

/**
  * @brief  init i2c digit filters and clock control register.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  dfilters: number of digit filters (0x00~0x0F).
  * @param  clk: i2c clock control register (0x00000000~0xFFFFFFFF).
  * @retval none
  */
void i2c_init(i2c_type *i2c_x, uint8_t dfilters, uint32_t clk)
{
  /* disable i2c peripheral */
  i2c_x->ctrl1_bit.i2cen = FALSE;

  /* write clkctrl register*/
  i2c_x->clkctrl_bit.ccr = clk;

  /* write digital filter register*/
  i2c_x->ctrl1_bit.dfltval = dfilters;

  i2c_dfilter_enable(i2c_x, TRUE);
}

/**
  * @brief  enable or disable peripheral.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.i2cen = new_state;
}

/**
  * @brief  enable or disable dma requests.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  dma_req: dma transfer request.
  *         this parameter can be one of the following values:
  *         - I2C_DMA_REQUEST_TX: dma transmit request.
  *         - I2C_DMA_REQUEST_RX: dma receive request.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_dma_enable(i2c_type *i2c_x, i2c_dma_request_type dma_req, confirm_state new_state)
{
  if(dma_req == I2C_DMA_REQUEST_TX)
  {
    i2c_x->ctrl1_bit.dmatxen = new_state;
  }
  else
  {
    i2c_x->ctrl1_bit.dmarxen = new_state;
  }
}

/**
  * @brief  enable or disable general call mode.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_general_call_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.gcen = new_state;
}

/**
  * @brief  enable or disable cnt reload mode.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_reload_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.rlden = new_state;
}

/**
  * @brief  enable or disable digital filter.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_dfilter_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.dflten = new_state;
}

/**
  * @brief  enable or disable clock stretch.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_clock_stretch_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.stretchen = new_state;
}

/**
  * @brief  enable or disable ack.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void i2c_ack_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl1_bit.acken  = new_state;
}

/**
  * @brief  reset the i2c register
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @retval none
  */
void i2c_reset(i2c_type *i2c_x)
{
  if(i2c_x == I2C1)
  {
    // crm_periph_reset(CRM_I2C1_PERIPH_RESET, TRUE);
    // crm_periph_reset(CRM_I2C1_PERIPH_RESET, FALSE);
    i2c_x->ctrl1_bit.swrst  = TRUE;
  }
  else if(i2c_x == I2C2)
  {
    // crm_periph_reset(CRM_I2C2_PERIPH_RESET, TRUE);
    // crm_periph_reset(CRM_I2C2_PERIPH_RESET, FALSE);
    i2c_x->ctrl1_bit.swrst  = TRUE;
  }
}

/**
  * @brief  config the slave address to be transmitted.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  address: slave address.
  * @retval none
  */
void i2c_transfer_addr_set(i2c_type *i2c_x, uint16_t address)
{
  i2c_x->ctrl2_bit.saddr = address & 0x03FF;
}

/**
  * @brief  get the slave address to be transmitted.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @retval slave address
  */
uint16_t i2c_transfer_addr_get(i2c_type *i2c_x)
{
  return i2c_x->ctrl2_bit.saddr;
}

/**
  * @brief  config the master transfer direction.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  i2c_direction: transfer request direction.
  *         this parameter can be one of the following values:
  *         - I2C_DIR_TRANSMIT: master request a write transfer.
  *         - I2C_DIR_RECEIVE: master request a read transfer.
  * @retval none
  */
void i2c_transfer_dir_set(i2c_type *i2c_x, i2c_transfer_dir_type i2c_direction)
{
  i2c_x->ctrl2_bit.mdir = i2c_direction;
}

/**
  * @brief   enable or disable slave data control.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void i2c_slave_data_ctrl_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl2_bit.slvdcen  = new_state;
}

/**
  * @brief  generate start condition.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @retval none
  */
void i2c_start_generate(i2c_type *i2c_x)
{
  i2c_x->ctrl2_bit.startgen = TRUE;
}

/**
  * @brief  generate stop condition.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @retval none
  */
void i2c_stop_generate(i2c_type *i2c_x)
{
  i2c_x->ctrl2_bit.stopgen = TRUE;
}

/**
  * @brief  enable or disable auto send stop mode.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void i2c_auto_stop_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl2_bit.astopen  = new_state;
}

/**
  * @brief  enable or disable extend bus timeout.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_ext_timeout_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->ctrl2_bit.timouten = new_state;
}

/**
  * @brief  config the transfer cnt .
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  cnt: transfer cnt.
  * @retval none
  */
void i2c_cnt_set(i2c_type *i2c_x, uint8_t cnt)
{
  i2c_x->ctrl2_bit.dacnt = cnt;
}

/**
  * @brief  enable or disable interrupts.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  i2c_int: interrupts sources.
  *         this parameter can be one of the following values:
  *         - I2C_ERRITEN: bus error interrupt.
  *         - I2C_EVTITEN: bus event interrupt.
  *         - I2C_BUFITEN: bus buffer interrupt.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_interrupt_enable(i2c_type *i2c_x, uint32_t source, confirm_state new_state)
{
  if (new_state != FALSE)
  {
    i2c_x->ctrl2 |= source;
  }
  else
  {
    i2c_x->ctrl2 &= (uint32_t)~source;
  }
}


/**
  * @brief  get interrupt status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  source
  *         this parameter can be one of the following values:
  *         - I2C_ERRITEN: bus error interrupt.
  *         - I2C_EVTITEN: bus event interrupt.
  *         - I2C_BUFITEN: bus buffer interrupt.
  * @retval flag_status (SET or RESET)
  */
flag_status i2c_interrupt_get(i2c_type *i2c_x, uint16_t source)
{
  if((i2c_x->ctrl2 & source) != I2C_RESET)
  {
    return I2C_SET;
  }
  else
  {
    return I2C_RESET;
  }
}


/**
  * @brief  config i2c own address 1.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  mode: i2c address mode.
  *         this parameter can be one of the following values:
  *         - I2C_ADDRESS_MODE_7BIT: 7bit address.
  *         - I2C_ADDRESS_MODE_10BIT: 10bit address.
  * @param  address: own address 1, such as 0xB0.
  * @retval none
  */
void i2c_own_address1_set(i2c_type *i2c_x, i2c_address_mode_type mode, uint16_t address)
{
  /* config address mode */
  i2c_x->oaddr1_bit.addr1mode = mode;

  /* config address */
  i2c_x->oaddr1_bit.addr1 = address & 0x03FF;
}

/**
  * @brief  config i2c own address 2 and mask.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  address: own address 2, such as 0xC0.
  * @retval none
  */
void i2c_own_address2_set(i2c_type *i2c_x, uint8_t address)
{
  i2c_x->oaddr2_bit.addr2 = (address >> 1) & 0x7F;
}

/**
  * @brief  enable or disable own address 2.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  new_state (TRUE or FALSE).
  * @retval none
  */
void i2c_own_address2_enable(i2c_type *i2c_x, confirm_state new_state)
{
  i2c_x->oaddr2_bit.addr2en = new_state;
}

/**
  * @brief  send a byte through the i2c periph.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  data: byte to be transmitted.
  * @retval none
  */
void i2c_data_send(i2c_type *i2c_x, uint8_t data)
{
  i2c_x->dr_bit.txdr = data;
}

/**
  * @brief  receive a byte through the i2c periph.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @retval the value of the received data.
  */
uint8_t i2c_data_receive(i2c_type *i2c_x)
{
  return (uint8_t)i2c_x->dr_bit.rxdr;
}

/**
  * @brief  get interrupt status flag.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - I2C_SB_EVT: Start bit event flag (master mode)
  *         - I2C_ADDR_EVT: Address sent event (master mode)/matched event flag (slave mode)
  *         - I2C_ADDR10_EVT: 10-bit address header sent event flag (master mode)
  *         - I2C_DBTF_EVT: Data byte transfer finish event flag
  *         - I2C_STOPF_EVT: STOP detection event flag (slave mode)
  *         - I2C_RXNE_EVT: Rx data register not empty event
  *         - I2C_TXE_EVT: Tx data register empty event

  *         - I2C_BERR_ERR: Bus error flag
  *         - I2C_ARLO_ERR: Arbitration lost error flag (master mode)
  *         - I2C_AF_ERR: Acknowledge failure flag
  *         - I2C_OVR_ERR: Overrun/Underrun error flag
  *         - I2C_TIMEOUT_ERR: Timeout error flag
  * @retval the new state of flag (SET or RESET).
  */
flag_status i2c_flag_get(i2c_type *i2c_x, uint32_t flag)
{
  if((i2c_x->sr1 & flag) != I2C_RESET)
  {
    return I2C_SET;
  }
  else
  {
    return I2C_RESET;
  }
}

/**
  * @brief  clear event/error flag status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  flag: specifies the flag to clear.
  *         this parameter can be one of the following values:
  *         - I2C_SB_EVT: Start bit event flag (master mode)
  *         - I2C_ADDR_EVT: Address sent event (master mode)/matched event flag (slave mode)
  *         - I2C_ADDR10_EVT: 10-bit address header sent event flag (master mode)
  *         - I2C_DBTF_EVT: Data byte transfer finish event flag
  *         - I2C_STOPF_EVT: STOP detection event flag (slave mode)
  *         - I2C_RXNE_EVT: Rx data register not empty event
  *         - I2C_TXE_EVT: Tx data register empty event

  *         - I2C_BERR_ERR: Bus error flag
  *         - I2C_ARLO_ERR: Arbitration lost error flag (master mode)
  *         - I2C_AF_ERR: Acknowledge failure flag
  *         - I2C_OVR_ERR: Overrun/Underrun error flag
  *         - I2C_TIMEOUT_ERR: Timeout error flag
  * @retval none
  */
//void i2c_flag_clear(i2c_type *i2c_x, uint32_t flag)
//{
//	uint32_t sr1_tmp;
//	sr1_tmp = i2c_x->sr1;
//  sr1_tmp &= ~flag;
//	i2c_x->sr1 = sr1_tmp;
//	
//}

/**
  * @brief  get the i2c master/slave mode.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval the value of the master/slave direction
  */
i2c_msm_mode_type i2c_ms_mode_get(i2c_type *i2c_x)
{
  if (i2c_x->sr2_bit.msm == 0)
  {
    return I2C_SLAVE_MODE;
  }
  else
  {
    return I2C_MASTER_MODE;
  }
}

/**
  * @brief  slave get the i2c transfer direction.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval the value of the slave direction
  *         - I2C_DIR_TRANSMIT: master request a write transfer, slave enters receiver mode.
  *         - I2C_DIR_RECEIVE: master request a read transfer, slave enters transmitter mode.
  */
i2c_transfer_dir_type i2c_transfer_dir_get(i2c_type *i2c_x)
{
  if (i2c_x->sr2_bit.trm == 0)
  {
    return I2C_DIR_TRANSMIT;
  }
  else
  {
    return I2C_DIR_RECEIVE;
  }
}

/**
  * @brief  get the i2c busy status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval the value of the busy status
  */
i2c_busy_type i2c_busy_get(i2c_type *i2c_x)
{
  if (i2c_x->sr2_bit.busy == 0)
  {
    return I2C_NONE_BUSY;
  }
  else
  {
    return I2C_BUSY;
  }
}

/**
  * @brief  get the i2c general call status
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval the value of the bgeneral call status
  */
i2c_gca_type i2c_gca_get(i2c_type *i2c_x)
{
  if (i2c_x->sr2_bit.gca == 0)
  {
    return I2C_NO_GENCALL;
  }
  else
  {
    return I2C_RECEIVE_GENCALL;
  }
}

/**
  * @brief  slave get the i2c received address matched with addr1 or addr2
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @retval the value that received address matched with addr1 or addr2
  */
i2c_addrf_type i2c_addrf_get(i2c_type *i2c_x)
{
  if (i2c_x->sr2_bit.addrf == 0)
  {
    return I2C_MATCH_ADDR1;
  }
  else
  {
    return I2C_MATCH_ADDR2;
  }
}

/**
  * @brief  Set i2c clock 
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2, I2C3.
  * @param  mode: Speed mode
  *         0b00 : Standard speed (100 kHz max.)
  *         0b01 : Fast speed (400 kHz max.)
  *         0b10 : Fast speed plus (1 MHz max.)
  *         0b11 : Not allowed
  * @param  pclk: I2C Peripheral clock frequency
  *         0b00000 / 0b00001 / Higher than 0b101010 : Not allowed
  *         0b00010 : 2 MHz
  *         …
  *         0b110010 : 50 MHz
  * @param  ccr: Clock control register in each speed mode
  * @retval none
  */
void i2c_clk_set(i2c_type *i2c_x, uint8_t mode, uint32_t pclk,  uint32_t ccr)
{
  i2c_x->clkctrl_bit.spdm = mode;

  i2c_x->clkctrl_bit.freq = pclk;

  i2c_x->clkctrl_bit.ccr = ccr;

}

/**
  * @brief  config data transfer.
  * @param  i2c_x: to select the i2c peripheral.
  *         this parameter can be one of the following values:
  *         I2C1, I2C2.
  * @param  address: slave address.
  * @param  cnt: transfer conuter(0~255)
  * @param  rld_stop: config reload and gen stop condition mode.
  *         this parameter can be one of the following values:
  *         - I2C_AUTO_STOP_MODE: auto generate stop mode.
  *         - I2C_SOFT_STOP_MODE: soft generate stop mode.
  *         - I2C_RELOAD_MODE:  reload mode.
  * @param  start: config gen start condition mode.
  *         this parameter can be one of the following values:
  *         - I2C_WITHOUT_START: transfer data without start condition.
  *         - I2C_GEN_START_READ: read data and generate start.
  *         - I2C_GEN_START_WRITE: send data and generate start.
  * @retval none
  */
void i2c_transmit_set(i2c_type *i2c_x, uint16_t address, uint8_t cnt, i2c_reload_stop_mode_type rld_stop, i2c_start_mode_type start)
{
  uint32_t temp;

  /* copy ctrl2 value to temp */
  temp = i2c_x->ctrl2;

  /* clear ctrl2_bit specific bits */
  temp &= ~0x03FF67FF;

  /* transfer mode and address set */
  temp |= address | rld_stop | start;

  /* transfer counter set */
  temp |= (uint32_t)cnt << 16;

  /* update ctrl2 value */
  i2c_x->ctrl2 = temp;
}


/**
  * @}
  */

#endif

/**
  * @}
  */

/**
  * @}
  */
