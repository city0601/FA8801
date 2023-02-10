/*
 * *****************************************************************************************
 *
 * 		Copyright (C) 2014-2020 FitiPower Technology Co.,Ltd.
 * 		
 * @file        FITI_M4_ddr3.h
 * @author	    TCON SOC1 FW Team
 * @version	    V1.0.0
 * @date        2020-4-1 09:00:00
 * @brief       This file contains all the functions prototypes for the DDR3 firmware library.
 ******************************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FITI_M4_DDR3_H
#define FITI_M4_DDR3_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "FITI_M4.h"

/* Functions ------------------------------------------------------------------*/
/**
  * @brief DDR3 Initialization.
  */
extern uint8_t DDR3_Init(void);
	 
/**
  * @brief Read data from DDR3.
  */
extern void DDR3_Read(uint32_t addr, uint32_t *rd_buff);
	 
/**
  * @brief Write data into DDR3
  */
extern void DDR3_Write(uint32_t addr, uint32_t *wr_buff);

#ifdef __cplusplus
}
#endif

#endif /* FITI_M4_DDR3_H */

/*************************FitiPower*****END OF FILE*********************/
