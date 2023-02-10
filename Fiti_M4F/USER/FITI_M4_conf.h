/*
 * *****************************************************************************************
 *
 * 		Copyright (C) 2014-2021 FitiPower Technology Co.,Ltd.
 * 		
 * @file      FITI_M4_conf.h
 * @author    TCON SOC1 FW Team
 * @version   V1.0.1
 * @date      2021-07-01 09:00:00
 * @brief     Library configuration file.
 ******************************************************************************************
 */

/* Define to prevent recursive inclusion--------------------------------------*/
#ifndef FITI_M4_CONF_H
#define FITI_M4_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "FITI_M4_uart.h"			  //UART
#include "FITI_M4_wdog.h"			  //WatchDog
#include "FITI_M4_timer.h"			  //Timer
#include "FITI_M4_rtc.h"			    //RTC
#include "FITI_M4_i2c.h"			    //I2C
#include "FITI_M4_spi.h"			    //SPI
#include "FITI_M4_ethernet.h"		//Ethernet
#include "FITI_M4_gpio.h"			  //GPIO
#include "FITI_M4_ddr3.h"			  //DDR3
#include "FITI_M4_spi_flash.h"		//SPI-Flash
#include "FITI_M4_misc.h"			  //NVIC

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/

//#define USE_FULL_ASSERT
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* FITI_M4_CONF_H */

/*************************FitiPower*****END OF FILE*********************/
