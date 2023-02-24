/**
  **************************************************************************
  * @file     Fiti_System_I2c_Middleware.c
  * @brief    the driver library of the i2c peripheral
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

#include "Fiti_System_I2c_Middleware.h"

/** @addtogroup Fiti_middlewares_i2c_application_library
  * @{
  */


#ifdef I2C_DMA_FUNC
/**
  * @brief get the dma transfer direction flag through the channel
  */
#define DMA_GET_REQUEST(DMA_CHANNEL) \
(((uint32_t)(DMA_CHANNEL) == ((uint32_t)hi2c->dma_tx_channel)) ? I2C_DMA_REQUEST_TX : I2C_DMA_REQUEST_RX)

/**
  * @brief get the dma transfer complete flag through the channel
  */
#define DMA_GET_TC_FLAG(DMA_CHANNEL) \
(((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL1))? DMA1_FDT1_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL2))? DMA1_FDT2_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL3))? DMA1_FDT3_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL4))? DMA1_FDT4_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL5))? DMA1_FDT5_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL6))? DMA1_FDT6_FLAG : \
                                                         DMA1_FDT7_FLAG)

/**
  * @brief get the dma half transfer flag through the channel
  */
#define DMA_GET_HT_FLAG(DMA_CHANNEL) \
(((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL1))? DMA1_HDT1_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL2))? DMA1_HDT2_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL3))? DMA1_HDT3_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL4))? DMA1_HDT4_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL5))? DMA1_HDT5_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL6))? DMA1_HDT6_FLAG : \
                                                         DMA1_HDT7_FLAG)

/**
  * @brief get the dma transfer error flag through the channel
  */
#define DMA_GET_TERR_FLAG(DMA_CHANNEL) \
(((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL1))? DMA1_DTERR1_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL2))? DMA1_DTERR2_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL3))? DMA1_DTERR3_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL4))? DMA1_DTERR4_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL5))? DMA1_DTERR5_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL6))? DMA1_DTERR6_FLAG : \
 ((uint32_t)(DMA_CHANNEL) == ((uint32_t)DMA1_CHANNEL7))? DMA1_DTERR7_FLAG : \
                                                         DMA1_DTERR7_FLAG)
#endif

/**
  * @brief i2c transmission status
  */
#define I2C_START                        0
#define I2C_END                          1

#define I2Cx_CLKCTRL                     0x20C01628
/**
  * @brief  initializes peripherals used by the i2c.
  * @param  none
  * @retval none
  */
__WEAK void i2c_lowlevel_init(i2c_handle_type* hi2c)
{
    /* config i2c */
    i2c_init(hi2c->i2cx, 0x0F, I2Cx_CLKCTRL);
}

/**
  * @brief  i2c peripheral initialization.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_config(i2c_handle_type* hi2c)
{
  /* reset i2c peripheral */
  i2c_reset(hi2c->i2cx);

  /* i2c peripheral initialization */
  i2c_lowlevel_init(hi2c);

  /* i2c peripheral enable */
  i2c_enable(hi2c->i2cx, TRUE);
}

/**
  * @brief  refresh i2c register.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_refresh_txdt_register(i2c_handle_type* hi2c)
{
  /* clear transmit data interrupt event flag */
  if (i2c_flag_get(hi2c->i2cx, I2C_TXIT_EVT) != RESET)
  {
    hi2c->i2cx->dr_bit.txdr = 0x00;
  }

  /* refresh txdt register*/
  if (i2c_flag_get(hi2c->i2cx, I2C_TXE_EVT) == RESET)
  {
    // hi2c->i2cx->sts_bit.tdbe = 1;
    hi2c->i2cx->dr_bit.txdr = 0x00;
  }
}

/**
  * @brief  reset ctrl2 register.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_reset_ctrl2_register(i2c_handle_type* hi2c)
{
  hi2c->i2cx->ctrl2_bit.saddr   = 0;
  hi2c->i2cx->ctrl2_bit.dacnt   = 0;
  hi2c->i2cx->ctrl2_bit.mdir    = 0;
  hi2c->i2cx->ctrl1_bit.rlden   = 0;
}

/**
  * @brief  wait for the transfer to end.
  * @param  hi2c: the handle points to the operation information.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_wait_end(i2c_handle_type* hi2c, uint32_t timeout)
{
  while(hi2c->status != I2C_END)
  {
    /* check timeout */
    if((timeout--) == 0)
    {
      return I2C_ERR_TIMEOUT;
    }
  }

  if(hi2c->error_code != I2C_OK)
  {
    return hi2c->error_code;
  }

  return I2C_OK;
}

/**
  * @brief  wait for the flag to be set or reset, only BUSYF flag
  *         is waiting to be reset, and other flags are waiting to be set
  * @param  hi2c: the handle points to the operation information.
  * @param  flag: specifies the flag to check.
  *         this parameter can be one of the following values:
  *         - I2C_SB_EVT: Start bit event flag (master mode).
  *         - I2C_STOPF_EVT:STOP detection event flag (slave mode).       
  *         - I2C_BUSY_EVT: Bus busy event.
  *         - I2C_GCAR_EVT: General call address receive event(slave mode) flag.
  * 
  *         - I2C_TXE_EVT: Tx data register empty event.
  *         - I2C_RXNE_EVT: Rx data register not empty event.
  *         - I2C_TXIT_EVT: Transmit data interrupt event.
  *         - I2C_TDC_EVT: Transfer data complete event.
  *         - I2C_TCRLD_EVT: Transmission is complete, waiting to load data.
  *         - I2C_ADDR_EVT: Address sent event (master mode)/matched event flag (slave mode).
  * 
  *         - I2C_AF_ERR: Acknowledge failure flag.
  *         - I2C_BUSERR_ERR: Bus error flag.
  *         - I2C_ARLO_ERR: Arbitration lost error flag (master mode).
  *         - I2C_OVR_ERR: Overrun/Underrun error flag.
  *         - I2C_TIMEOUT_ERR: Timeout error flag.
  * @param  event_check: check other error flags while waiting for the flag.
  *         parameter as following values:
  *         - I2C_EVENT_CHECK_NONE
  *         - I2C_EVENT_CHECK_ACKFAIL
  *         - I2C_EVENT_CHECK_STOP
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_wait_flag(i2c_handle_type* hi2c, uint32_t flag, uint32_t event_check, uint32_t timeout)
{
  if(flag == I2C_BUSY_EVT)
  {
    while(i2c_flag_get(hi2c->i2cx, flag) != RESET)
    {
      /* check timeout */
      if((timeout--) == 0)
      {
        hi2c->error_code = I2C_ERR_TIMEOUT;

        return I2C_ERR_TIMEOUT;
      }
    }
  }
  else
  {
    while(i2c_flag_get(hi2c->i2cx, flag) == RESET)
    {
      /* check the ack fail flag */
      if(event_check & I2C_EVENT_CHECK_ACKFAIL)
      {
        if(i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
        {
          /* clear ack fail flag */
          i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);

          hi2c->error_code = I2C_ERR_ACKFAIL;

          return I2C_ERR_ACKFAIL;
        }
      }

      /* check the stop flag */
      if(event_check & I2C_EVENT_CHECK_STOP)
      {
        if(i2c_flag_get(hi2c->i2cx, I2C_STOPF_EVT) != RESET)
        {
          /* clear stop flag */
          i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

          i2c_reset_ctrl2_register(hi2c);

          hi2c->error_code = I2C_ERR_STOP;

          return I2C_ERR_STOP;
        }
      }

      /* check timeout */
      if((timeout--) == 0)
      {
        hi2c->error_code = I2C_ERR_TIMEOUT;

        return I2C_ERR_TIMEOUT;
      }
    }
  }

  return I2C_OK;
}
/**
  * @brief  start transfer in poll mode or interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  start: config gen start condition mode.
  *         parameter as following values:
  *         - I2C_WITHOUT_START: transfer data without start condition.
  *         - I2C_GEN_START_READ: read data and generate start.
  *         - I2C_GEN_START_WRITE: send data and generate start.
  * @retval i2c status.
  */
void i2c_start_transfer(i2c_handle_type* hi2c, uint16_t address, i2c_start_mode_type start)
{
  if (hi2c->pcount > MAX_TRANSFER_CNT)
  {
    hi2c->psize = MAX_TRANSFER_CNT;

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_RELOAD_MODE, start);
  }
  else
  {
    hi2c->psize = hi2c->pcount;

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_AUTO_STOP_MODE, start);
  }
}

/******************************************************************************************************************************************************
 * I2c Simple Master/Slave Transmit/Receive
*******************************************************************************************************************************************************/

/**
  * @brief  the master transmits data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_transmit(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }
 
  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_WRITE);

  while (hi2c->pcount > 0)
  {
    /* wait for the tdis flag to be set */
    if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_2;
    }

    /* send data */
    i2c_data_send(hi2c->i2cx, *hi2c->pbuff++);
    hi2c->psize--;
    hi2c->pcount--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_3;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  the slave receive data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_receive(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* wait for the addr flag to be set */
  if (i2c_wait_flag(hi2c, I2C_ADDR_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* clear addr flag */
  i2c_flag_clear(hi2c->i2cx, I2C_ADDR_EVT);

  while (hi2c->pcount > 0)
  {
    /* wait for the rdbf flag to be set  */
    if(i2c_wait_flag(hi2c, I2C_RXNE_EVT, I2C_EVENT_CHECK_STOP, timeout) != I2C_OK)
    {
      /* disable acknowledge */
      i2c_ack_enable(hi2c->i2cx, FALSE);

      /* if data is received, read data */
      if (i2c_flag_get(hi2c->i2cx, I2C_RXNE_EVT) == SET)
      {
        /* read data */
        (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
        hi2c->pcount--;
      }

      return I2C_ERR_STEP_4;
    }

    /* read data */
    (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
    hi2c->pcount--;
  }

  /* wait for the stop flag to be set */
  if(i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    /* disable acknowledge */
    i2c_ack_enable(hi2c->i2cx, FALSE);

    return I2C_ERR_STEP_5;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    /* disable acknowledge */
    i2c_ack_enable(hi2c->i2cx, FALSE);

    return I2C_ERR_STEP_6;
  }

  return I2C_OK;
}

/**
  * @brief  the master receive data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_receive(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_READ);

  while (hi2c->pcount > 0)
  {
    /* wait for the rdbf flag to be set  */
    if(i2c_wait_flag(hi2c, I2C_RXNE_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_2;
    }

    /* read data */
    (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
    hi2c->pcount--;
    hi2c->psize--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_3;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  the slave transmits data through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_transmit(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* wait for the addr flag to be set */
  if (i2c_wait_flag(hi2c, I2C_ADDR_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    /* disable acknowledge */
    i2c_ack_enable(hi2c->i2cx, FALSE);
    return I2C_ERR_STEP_2;
  }

  /* clear addr flag */
  i2c_flag_clear(hi2c->i2cx, I2C_ADDR_EVT);

  /* if 10-bit address mode is used */
  if (hi2c->i2cx->ctrl2_bit.addr10en != RESET)
  {
    /* wait for the addr flag to be set */
    if (i2c_wait_flag(hi2c, I2C_ADDR_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
    {
      /* disable acknowledge */
      i2c_ack_enable(hi2c->i2cx, FALSE);

      return I2C_ERR_STEP_3;
    }

    /* clear addr flag */
    i2c_flag_clear(hi2c->i2cx, I2C_ADDR_EVT);
  }

  while (hi2c->pcount > 0)
  {
    /* wait for the tdis flag to be set */
    if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      /* disable acknowledge */
      i2c_ack_enable(hi2c->i2cx, FALSE);

      return I2C_ERR_STEP_5;
    }

    /* send data */
    i2c_data_send(hi2c->i2cx, *hi2c->pbuff++);
    hi2c->pcount--;
  }

  /* wait for the ackfail flag to be set */
  if(i2c_wait_flag(hi2c, I2C_AF_ERR, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_6;
  }

  /* clear ack fail flag */
  i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);

  /* wait for the stop flag to be set */
  if(i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    /* disable acknowledge */
    i2c_ack_enable(hi2c->i2cx, FALSE);

    return I2C_ERR_STEP_7;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    /* disable acknowledge */
    i2c_ack_enable(hi2c->i2cx, FALSE);

    return I2C_ERR_STEP_8;
  }

  /* refresh tx dt register */
  i2c_refresh_txdt_register(hi2c);

  return I2C_OK;
}

/******************************************************************************************************************************************************
 * Simple Memory Read/Write Function
*******************************************************************************************************************************************************/

/**
  * @brief  send memory address.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_address_send(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t mem_address, int32_t timeout)
{
  i2c_status_type err_code;
  
  if(mem_address_width == I2C_MEM_ADDR_WIDIH_8)
  {
    /* send memory address */
    i2c_data_send(hi2c->i2cx, mem_address & 0xFF);
  }
  else
  {
    /* send memory address */
    i2c_data_send(hi2c->i2cx, (mem_address >> 8) & 0xFF);  
    
    /* wait for the tdis flag to be set */
    err_code = i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout);
    
    if(err_code != I2C_OK)
    {
      return err_code;
    }
    
    /* send memory address */
    i2c_data_send(hi2c->i2cx, mem_address & 0xFF);  
  }
  
  return I2C_OK;
}

/**
  * @brief  write data to the memory device through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_write(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size + mem_address_width;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  hi2c->psize -= mem_address_width;
  hi2c->pcount -= mem_address_width;

  while (hi2c->pcount > 0)
  {
    /* wait for the tdis flag to be set */
    if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_4;
    }

    /* send data */
    i2c_data_send(hi2c->i2cx, *hi2c->pbuff++);
    hi2c->psize--;
    hi2c->pcount--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_5;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if(i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_6;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @brief  read data from memory device through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_read(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->pbuff = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, mem_address_width, I2C_SOFT_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  /* wait for the tdc flag to be set */
  if (i2c_wait_flag(hi2c, I2C_TDC_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_READ);

  while (hi2c->pcount > 0)
  {
    /* wait for the rdbf flag to be set  */
    if (i2c_wait_flag(hi2c, I2C_RXNE_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
    {
      return I2C_ERR_STEP_5;
    }

    /* read data */
    (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
    hi2c->pcount--;
    hi2c->psize--;

    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* wait for the tcrld flag to be set  */
      if (i2c_wait_flag(hi2c, I2C_TCRLD_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
      {
        return I2C_ERR_STEP_6;
      }

      /* continue transfer */
      i2c_start_transfer(hi2c, address, I2C_WITHOUT_START);
    }
  }

  /* wait for the stop flag to be set  */
  if (i2c_wait_flag(hi2c, I2C_STOPF_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_7;
  }

  /* clear stop flag */
  i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

  /* reset ctrl2 register */
  i2c_reset_ctrl2_register(hi2c);

  return I2C_OK;
}

/**
  * @}
  */

#ifdef I2C_INT_FUNC

/******************************************************************************************************************************************************
 * I2c Master/Slave Transmit/Receive with Interrupt
*******************************************************************************************************************************************************/
/**
  * @brief  the master transmits data through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_transmit_int(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_MA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_WRITE);

  /* enable interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN, TRUE);

  return I2C_OK;
}

/**
  * @brief  the slave receive data through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_receive_int(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_SLA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* enable interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_RxDITEN, TRUE);

  return I2C_OK;
}

/**
  * @brief  the master receive data through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_receive_int(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_MA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_READ);

  /* enable interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_RxDITEN, TRUE);

  return I2C_OK;
}

/**
  * @brief  the slave transmits data through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_transmit_int(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_SLA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* enable interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN, TRUE);

  i2c_refresh_txdt_register(hi2c);

  return I2C_OK;
}


/**
  * @brief  write data to the memory device through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_write_int(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_MA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size + mem_address_width;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if (i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }
  
  hi2c->psize--;
  hi2c->pcount--;

  /* enable interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN, TRUE);

  return I2C_OK;
}

/**
  * @brief  read data from memory device through interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_read_int(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_INT_MA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, mem_address_width, I2C_SOFT_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  /* wait for the tdc flag to be set */
  if (i2c_wait_flag(hi2c, I2C_TDC_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* start transfer */
  i2c_start_transfer(hi2c, address, I2C_GEN_START_READ);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_RxDITEN, TRUE);

  return I2C_OK;
}


/**
  * @brief  master interrupt processing function in interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @retval i2c status.
  */
i2c_status_type i2c_master_irq_handler_int(i2c_handle_type* hi2c)
{
  if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
  {
    /* clear ackfail flag */
    i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);

    /* refresh tx register */
    i2c_refresh_txdt_register(hi2c);

    if(hi2c->pcount != 0)
    {
      hi2c->error_code = I2C_ERR_ACKFAIL;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_TXIT_EVT) != RESET)
  {
    /* send data */
    i2c_data_send(hi2c->i2cx, *hi2c->pbuff++);
    hi2c->pcount--;
    hi2c->psize--;
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_TCRLD_EVT) != RESET)
  {
    if ((hi2c->psize == 0) && (hi2c->pcount != 0))
    {
      /* continue transfer */
      i2c_start_transfer(hi2c, i2c_transfer_addr_get(hi2c->i2cx), I2C_WITHOUT_START);
    }
    else
    {
      return I2C_ERR_TCRLD;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_RXNE_EVT) != RESET)
  {
    /* read data */
    (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
    hi2c->pcount--;
    hi2c->psize--;
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_TDC_EVT) != RESET)
  {
    if (hi2c->pcount == 0)
    {
      if (hi2c->i2cx->ctrl1_bit.astopen == 0)
      {
        /* generate stop condtion */
        i2c_stop_generate(hi2c->i2cx);
      }
    }
    else
    {
      return I2C_ERR_TDC;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_STOPF_EVT) != RESET)
  {
    /* clear stop flag */
    i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

    /* reset ctrl2 register */
    i2c_reset_ctrl2_register(hi2c);

    if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
    {
      /* clear ackfail flag */
      i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);
    }

    /* refresh tx dt register */
    i2c_refresh_txdt_register(hi2c);

    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN | I2C_RxDITEN, FALSE);

    /* transfer complete */
    hi2c->status = I2C_END;
  }

  return I2C_OK;
}

/**
  * @brief  slave interrupt processing function in interrupt mode.
  * @param  hi2c: the handle points to the operation information.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_irq_handler_int(i2c_handle_type* hi2c)
{
  if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
  {
    /* transfer complete */
    if (hi2c->pcount == 0)
    {
      i2c_refresh_txdt_register(hi2c);

      /* clear ackfail flag */
      i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);
    }
    /* the transfer has not been completed */
    else
    {
      /* clear ackfail flag */
      i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_ADDR_EVT) != RESET)
  {
    /* clear addr flag */
    i2c_flag_clear(hi2c->i2cx, I2C_ADDR_EVT);
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_TXIT_EVT) != RESET)
  {
    if (hi2c->pcount > 0)
    {
      /* send data */
      hi2c->i2cx->dr_bit.txdr = (*(hi2c->pbuff++));
      hi2c->psize--;
      hi2c->pcount--;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_RXNE_EVT) != RESET)
  {
    if (hi2c->pcount > 0)
    {
      /* read data */
      (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);
      hi2c->pcount--;
      hi2c->psize--;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_STOPF_EVT) != RESET)
  {
    /* clear stop flag */
    i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN | I2C_RxDITEN, FALSE);

    /* reset ctrl2 register */
    i2c_reset_ctrl2_register(hi2c);

    /* refresh tx dt register */
    i2c_refresh_txdt_register(hi2c);

    /* if data is received, read data */
    if (i2c_flag_get(hi2c->i2cx, I2C_RXNE_EVT) != RESET)
    {
      /* read data */
      (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);

      if ((hi2c->psize > 0))
      {
        hi2c->pcount--;
        hi2c->psize--;
      }
    }

    /* transfer complete */
    hi2c->status = I2C_END;
  }

  return I2C_OK;
}

/**
  * @brief  interrupt procession function.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_evt_irq_handler(i2c_handle_type* hi2c)
{
  switch(hi2c->mode)
  {
    case I2C_INT_MA_TX:
    case I2C_INT_MA_RX:
    {
      i2c_master_irq_handler_int(hi2c);
    }break;
    case I2C_INT_SLA_TX:
    case I2C_INT_SLA_RX:
    {
      i2c_slave_irq_handler_int(hi2c);
    }break;
#ifdef I2C_DMA_FUNC
    case I2C_DMA_MA_TX:
    case I2C_DMA_MA_RX:
    {
      i2c_master_irq_handler_dma(hi2c);
    }break;
    case I2C_DMA_SLA_TX:
    case I2C_DMA_SLA_RX:
    {
      i2c_slave_irq_handler_dma(hi2c);
    }break;
#endif
    default:break;
  }
}

/**
  * @brief  dma reveive complete interrupt function.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_err_irq_handler(i2c_handle_type* hi2c)
{ 
  /* buserr */
  if (i2c_flag_get(hi2c->i2cx, I2C_BUSERR_ERR) != RESET)
  {
    hi2c->error_code = I2C_ERR_INTERRUPT;
    
    /* clear flag */
    i2c_flag_clear(hi2c->i2cx, I2C_BUSERR_ERR);
    
    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, FALSE);
  }

  /* arlost */
  if (i2c_flag_get(hi2c->i2cx, I2C_ARLO_ERR) != RESET)
  {
    hi2c->error_code = I2C_ERR_INTERRUPT;
    
    /* clear flag */
    i2c_flag_clear(hi2c->i2cx, I2C_ARLO_ERR);
    
    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, FALSE);
  }

  /* ouf */
  if (i2c_flag_get(hi2c->i2cx, I2C_OVR_ERR) != RESET)
  {
    hi2c->error_code = I2C_ERR_INTERRUPT;
    
    /* clear flag */
    i2c_flag_clear(hi2c->i2cx, I2C_OVR_ERR);
    
    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, FALSE);
  }

  /* timeout */
  if (i2c_flag_get(hi2c->i2cx, I2C_TIMEOUT_ERR) != RESET)
  {
    hi2c->error_code = I2C_ERR_INTERRUPT;
    
    /* clear flag */
    i2c_flag_clear(hi2c->i2cx, I2C_TIMEOUT_ERR);
    
    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, FALSE);
  }
}

#endif

#ifdef I2C_DMA_FUNC
/**
  * @brief  dma transfer cofiguration.
  * @param  hi2c: the handle points to the operation information.
  * @param  dma_channelx: dma channel to be cofigured.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @retval none.
  */
void i2c_dma_config(i2c_handle_type* hi2c, dma_channel_type* dma_channel, uint8_t* pdata, uint16_t size)
{
  /* disable the dma channel */
  dma_channel_enable(dma_channel, FALSE);

  /* disable the transfer complete interrupt */
  dma_interrupt_enable(dma_channel, DMA_FDT_INT, FALSE);

  /* configure the dma channel with the buffer address and the buffer size */
  hi2c->dma_init_struct.memory_base_addr     = (uint32_t)pdata;
  hi2c->dma_init_struct.direction            = (dma_channel == hi2c->dma_tx_channel) ? DMA_DIR_MEMORY_TO_PERIPHERAL : DMA_DIR_PERIPHERAL_TO_MEMORY;
  hi2c->dma_init_struct.peripheral_base_addr = (dma_channel == hi2c->dma_tx_channel) ? (uint32_t)&hi2c->i2cx->txdt : (uint32_t)&hi2c->i2cx->rxdt;
  hi2c->dma_init_struct.buffer_size          = (uint32_t)size;
  dma_init(dma_channel, &hi2c->dma_init_struct);

  /* enable the transfer complete interrupt */
  dma_interrupt_enable(dma_channel, DMA_FDT_INT, TRUE);

  /* enable the dma channel */
  dma_channel_enable(dma_channel, TRUE);
}

/**
  * @brief  start transfer in dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  start: config gen start condition mode.
  *         parameter as following values:
  *         - I2C_WITHOUT_START: transfer data without start condition.
  *         - I2C_GEN_START_READ: read data and generate start.
  *         - I2C_GEN_START_WRITE: send data and generate start.
  * @retval i2c status.
  */
void i2c_start_transfer_dma(i2c_handle_type* hi2c, dma_channel_type* dma_channelx, uint16_t address, i2c_start_mode_type start)
{
  if (hi2c->pcount > MAX_TRANSFER_CNT)
  {
    hi2c->psize = MAX_TRANSFER_CNT;

    /* config dma */
    i2c_dma_config(hi2c, dma_channelx, hi2c->pbuff, hi2c->psize);

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_RELOAD_MODE, start);
  }
  else
  {
    hi2c->psize = hi2c->pcount;

    /* config dma */
    i2c_dma_config(hi2c, dma_channelx, hi2c->pbuff, hi2c->psize);

    i2c_transmit_set(hi2c->i2cx, address, hi2c->psize, I2C_AUTO_STOP_MODE, start);
  }
}


/**
  * @brief  the master transmits data through dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_transmit_dma(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_MA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, FALSE);

  /* start transfer */
  i2c_start_transfer_dma(hi2c, hi2c->dma_tx_channel, address, I2C_GEN_START_WRITE);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, TRUE);

  return I2C_OK;
}

/**
  * @brief  the slave receive data through dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_receive_dma(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_SLA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, FALSE);

  /* config dma */
  i2c_dma_config(hi2c, hi2c->dma_rx_channel, hi2c->pbuff, size);

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, TRUE);

  return I2C_OK;
}

/**
  * @brief  the master receive data through dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  address: slave address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_master_receive_dma(i2c_handle_type* hi2c, uint16_t address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_MA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, FALSE);

  /* start transfer */
  i2c_start_transfer_dma(hi2c, hi2c->dma_rx_channel, address, I2C_GEN_START_READ);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, TRUE);

  return I2C_OK;
}

/**
  * @brief  the slave transmits data through dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_transmit_dma(i2c_handle_type* hi2c, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_SLA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, FALSE);

  /* config dma */
  i2c_dma_config(hi2c, hi2c->dma_tx_channel, hi2c->pbuff, size);

  /* enable acknowledge */
  i2c_ack_enable(hi2c->i2cx, TRUE);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_EVTITEN | I2C_BUFITEN | I2C_ERRITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, TRUE);

  return I2C_OK;
}

/**
  * @brief  write data to the memory device through dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_write_dma(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_MA_TX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, FALSE);

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* transfer config */
  i2c_transmit_set(hi2c->i2cx, address, mem_address_width, I2C_RELOAD_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  /* wait for the tcrld flag to be set */
  if (i2c_wait_flag(hi2c, I2C_TCRLD_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* start transfer */
  i2c_start_transfer_dma(hi2c, hi2c->dma_tx_channel, address, I2C_WITHOUT_START);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, TRUE);

  return I2C_OK;
}

/**
  * @brief  read data from memory device through polling mode.
  * @param  hi2c: the handle points to the operation information.
  * @param  mem_address_width: memory address width.
  *         this parameter can be one of the following values:
  *         - I2C_MEM_ADDR_WIDIH_8:  memory address is 8 bit 
  *         - I2C_MEM_ADDR_WIDIH_16:  memory address is 16 bit 
  * @param  address: memory device address.
  * @param  mem_address: memory address.
  * @param  pdata: data buffer.
  * @param  size: data size.
  * @param  timeout: maximum waiting time.
  * @retval i2c status.
  */
i2c_status_type i2c_memory_read_dma(i2c_handle_type* hi2c, i2c_mem_address_width_type mem_address_width, uint16_t address, uint16_t mem_address, uint8_t* pdata, uint16_t size, uint32_t timeout)
{
  /* initialization parameters */
  hi2c->mode   = I2C_DMA_MA_RX;
  hi2c->status = I2C_START;

  hi2c->pbuff  = pdata;
  hi2c->pcount = size;

  hi2c->error_code = I2C_OK;

  /* wait for the busy flag to be reset */
  if(i2c_wait_flag(hi2c, I2C_BUSY_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_1;
  }

  /* start transfer */
  i2c_transmit_set(hi2c->i2cx, address, mem_address_width, I2C_SOFT_STOP_MODE, I2C_GEN_START_WRITE);

  /* wait for the tdis flag to be set */
  if(i2c_wait_flag(hi2c, I2C_TXIT_EVT, I2C_EVENT_CHECK_ACKFAIL, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_2;
  }

  /* send memory address */
  if(i2c_memory_address_send(hi2c, mem_address_width, mem_address, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_3;
  }

  /* wait for the tdc flag to be set */
  if (i2c_wait_flag(hi2c, I2C_TDC_EVT, I2C_EVENT_CHECK_NONE, timeout) != I2C_OK)
  {
    return I2C_ERR_STEP_4;
  }

  /* disable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, FALSE);

  /* start transfer */
  i2c_start_transfer_dma(hi2c, hi2c->dma_rx_channel, address, I2C_GEN_START_READ);

  /* enable i2c interrupt */
  i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN, TRUE);

  /* enable dma request */
  i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, TRUE);

  return I2C_OK;
}

/**
  * @brief  master interrupt processing function in dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @retval i2c status.
  */
i2c_status_type i2c_master_irq_handler_dma(i2c_handle_type* hi2c)
{
  if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
  {
    /* clear ackfail flag */
    i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);

    /* enable stop interrupt to wait for stop generate stop */
    i2c_interrupt_enable(hi2c->i2cx, I2C_EVTITEN, TRUE);

    /* refresh tx dt register */
    i2c_refresh_txdt_register(hi2c);

    if(hi2c->pcount != 0)
    {
      hi2c->error_code = I2C_ERR_ACKFAIL;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_TCRLD_EVT) != RESET)
  {
    /* disable tdc interrupt */
    i2c_interrupt_enable(hi2c->i2cx, I2C_EVTITEN, FALSE);

    if (hi2c->pcount != 0)
    {
      /* continue transfer */
      i2c_start_transfer(hi2c, i2c_transfer_addr_get(hi2c->i2cx), I2C_WITHOUT_START);

      /* enable dma request */
      if (hi2c->dma_init_struct.direction == DMA_DIR_MEMORY_TO_PERIPHERAL)
      {
        i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_TX, TRUE);
      }
      else
      {
        i2c_dma_enable(hi2c->i2cx, I2C_DMA_REQUEST_RX, TRUE);
      }
    }
    else
    {
      return I2C_ERR_TCRLD;
    }
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_STOPF_EVT) != RESET)
  {
    /* clear stop flag */
    i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

    /* reset ctrl2 register */
    i2c_reset_ctrl2_register(hi2c);

    if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
    {
      /* clear ackfail flag */
      i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);
    }

    /* refresh tx dt register */
    i2c_refresh_txdt_register(hi2c);

    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN | I2C_RxDITEN, FALSE);

    /* transfer complete */
    hi2c->status = I2C_END;
  }

  return I2C_OK;
}

/**
  * @brief  slave interrupt processing function in dma mode.
  * @param  hi2c: the handle points to the operation information.
  * @retval i2c status.
  */
i2c_status_type i2c_slave_irq_handler_dma(i2c_handle_type* hi2c)
{
  if (i2c_flag_get(hi2c->i2cx, I2C_AF_ERR) != RESET)
  {
    /* clear ackfail flag */
    i2c_flag_clear(hi2c->i2cx, I2C_AF_ERR);
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_ADDR_EVT) != RESET)
  {
    /* clear addr flag */
    i2c_flag_clear(hi2c->i2cx, I2C_ADDR_EVT);
  }
  else if (i2c_flag_get(hi2c->i2cx, I2C_STOPF_EVT) != RESET)
  {
    /* clear stop flag */
    i2c_flag_clear(hi2c->i2cx, I2C_STOPF_EVT);

    /* disable interrupts */
    i2c_interrupt_enable(hi2c->i2cx, I2C_ERRITEN | I2C_EVTITEN | I2C_BUFITEN | I2C_TxDITEN | I2C_RxDITEN, FALSE);

    /* reset ctrl2 register */
    i2c_reset_ctrl2_register(hi2c);

    /* refresh tx dt register */
    i2c_refresh_txdt_register(hi2c);

    /* if data is received, read data */
    if (i2c_flag_get(hi2c->i2cx, I2C_RXNE_EVT) != RESET)
    {
      /* read data */
      (*hi2c->pbuff++) = i2c_data_receive(hi2c->i2cx);

      if ((hi2c->psize > 0))
      {
        hi2c->pcount--;
        hi2c->psize--;
      }
    }

    /* transfer complete */
    hi2c->status = I2C_END;
  }

  return I2C_OK;
}
/**
  * @brief  dma processing function.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_dma_tx_rx_irq_handler(i2c_handle_type* hi2c, dma_channel_type* dma_channel)
{
  /* transfer complete */
  if (dma_flag_get(DMA_GET_TC_FLAG(dma_channel)) != RESET)
  {
    /* disable the transfer complete interrupt */
    dma_interrupt_enable(dma_channel, DMA_FDT_INT, FALSE);

    /* clear the transfer complete flag */
    dma_flag_clear(DMA_GET_TC_FLAG(dma_channel));

    /* disable dma request */
    i2c_dma_enable(hi2c->i2cx, DMA_GET_REQUEST(dma_channel), FALSE);

    /* disable dma channel */
    dma_channel_enable(dma_channel, FALSE);

    switch(hi2c->mode)
    {
      case I2C_DMA_MA_TX:
      case I2C_DMA_MA_RX:
      {
        /* update the number of transfers */
        hi2c->pcount -= hi2c->psize;

        /* transfer complete */
        if (hi2c->pcount == 0)
        {
          /* enable stop interrupt */
          i2c_interrupt_enable(hi2c->i2cx, I2C_EVTITEN, TRUE);
        }
        /* the transfer has not been completed */
        else
        {
          /* update the buffer pointer of transfers */
          hi2c->pbuff += hi2c->psize;

          /* set the number to be transferred */
          if (hi2c->pcount > MAX_TRANSFER_CNT)
          {
            hi2c->psize = MAX_TRANSFER_CNT;
          }
          else
          {
            hi2c->psize = hi2c->pcount;
          }

          /* config dma channel, continue to transfer data */
          i2c_dma_config(hi2c, dma_channel, hi2c->pbuff, hi2c->psize);

          /* enable tdc interrupt */
          i2c_interrupt_enable(hi2c->i2cx, I2C_EVTITEN, TRUE);
        }
      }break;
      case I2C_DMA_SLA_TX:
      case I2C_DMA_SLA_RX:
      {

      }break;

      default:break;
    }
  }
}

/**
  * @brief  dma transmission complete interrupt function.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_dma_tx_irq_handler(i2c_handle_type* hi2c)
{
  i2c_dma_tx_rx_irq_handler(hi2c, hi2c->dma_tx_channel);
}

/**
  * @brief  dma reveive complete interrupt function.
  * @param  hi2c: the handle points to the operation information.
  * @retval none.
  */
void i2c_dma_rx_irq_handler(i2c_handle_type* hi2c)
{
  i2c_dma_tx_rx_irq_handler(hi2c, hi2c->dma_rx_channel);
}
#endif