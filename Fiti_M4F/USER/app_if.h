/*
 * *****************************************************************************************
 *
 * 		Copyright (C) 2014-2020 FitiPower Technology Co.,Ltd.
 * 		
 * @file			app_if.h
 * @author		TCON SOC1 FW Team
 * @version		V1.0.0
 * @date			2020-4-1 09:00:00
 * @brief			Application call interface.
 ******************************************************************************************
 */

#ifndef APP_IF_H
#define APP_IF_H

/* Includes ------------------------------------------------------------------*/
#include "FITI_M4.h"
#include <stdio.h>

/* Declarations ------------------------------------------------------------------*/
extern void UartInit(uint32_t baudrate);
extern void app_call_if(void);

#endif
