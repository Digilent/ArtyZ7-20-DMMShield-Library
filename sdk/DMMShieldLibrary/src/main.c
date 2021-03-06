/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Digilent

  @File Name
    main.c

  @Description
        This file is the application entry point.
        It contains the definition of the UART dispatch commands demo function, used for communicating with the DMM module and is called from main function
        It also implements a demo function for EPROM functionality, which is not called by the main function.

  @Author
    Cristian Fatu
    cristian.fatu@digilent.ro


  @Versioning:
 	 Cristian Fatu - 2018/06/29 - Initial release, DMMShield Library

 */
/* ************************************************************************** */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "spi.h"
#include "gpio.h"
#include "dmmcmd.h"
#include "utils.h"
#include "uart.h"
#include "errors.h"
#include "eprom.h"


void Demo_UART_Dispatch();
void Demo_UserEPROM();




int main()
{
	init_platform();

	Demo_UART_Dispatch();
//	Demo_UserEPROM();

    cleanup_platform();
    return 0;
}

/***	Demo_UART_Dispatch()
**
**	Parameters:
**		none
**
**	Return Value:
**          none
**
**	Description:
**		This function implements the main demo UART command dispatch interpreter.
**      It calls the initialization function for DMMCMD module DMMCMD_Init().
**      In an infinite - while - loop the function calls DMMCMD_CheckForCommand to check with null parameter for uartCmdFurtherProcess.
**
*/
void Demo_UART_Dispatch()
{
    GPIO_Init();
    DMMCMD_Init();
    ERRORS_Init("OK", "ERROR");

    UART_PutString("Command loop\r\n");
	XGpio_DiscreteSet(&Gpio, GPIO_OUTPUT_CHANNEL, 0x00);
    while(1)
    {
        DMMCMD_CheckForCommand();
    }
}


/***	Demo_UserEPROM()
**
**	Parameters:
**		none
**
**	Return Value:
**          none
**
**	Description:
**		This function implements an EPROM demo.
**      It demonstrates how to write / retrieve data from user space of EPROM (address space 00 - 31).
**
*/
void Demo_UserEPROM()
{
    char sUserText[] = "01020304050607080910111213141516171819202122232425262728293031"; // 62 chars
    char sReceivedText[62+1]; // 62 chars + terminating 0
    u8 bErrCode;
    EPROM_Init();
    UART_Init(115200);
    UART_PutString("EPROM demo\r\n");
    UART_PutString("Stored string:\r\n");
    UART_PutString(sUserText);
    UART_PutString("\r\n");
    // write data to user area of EPROM, 31 words starting from address 0
    EPROM_WriteEnable();
    bErrCode = EPROM_WriteWords(0, (uint16_t *)sUserText, 31);
    if(bErrCode == ERRVAL_SUCCESS)
    {
		// read back the data from the user area of EPROM, 31 words starting from address 0
    	EPROM_ReadWords(0, (uint16_t *)sReceivedText, 31);
    	sReceivedText[62] = 0;	// 0 terminator

		UART_PutString("Retrieved string:\r\n");
		UART_PutString(sUserText);
	    UART_PutString("\r\n");
		if(!strcmp(sUserText, sReceivedText))
		{
			UART_PutString("Identical\r\n");
		}
		else
		{
			UART_PutString("Mismatches found\r\n");
		}

    }
}




