/*
 * *****************************************************************************************
 *
 * 		Copyright (C) 2014-2020 FitiPower Technology Co.,Ltd.
 * 		
 * @file			main.c
 * @author		TCON SOC1 FW Team
 * @version		V1.0.0
 * @date			2020-4-1 09:00:00
 * @brief			Main body.
 ******************************************************************************************
 */
 
/* Includes ------------------------------------------------------------------*/
#include "app_if.h"

int main(void)
{
	SystemInit();	//Initialize system
	#if 0
	UartInit(115200);		//Init UART0
	
	printf("\r\n******************************************************************************************\r\n");
	printf(" 		         Copyright (C) 2014-2021 FitiPower Technology Co.,Ltd.                  \r\n");
	printf("\r\n******************************************************************************************\r\n");
	printf("\r\n------------FITI_EMPU_M3 Peripherals Applications Demo----------\r\n");
	printf("\r\n");
	#endif
	app_call_if();//call application interface
	while(1);
	return 0;
}
