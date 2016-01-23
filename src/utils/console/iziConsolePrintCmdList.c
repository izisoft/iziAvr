#include <utils/iziConsole.h>
#include <device/iziDevicePriv.h>

#include "iziConsoleCommon.h"

void iziConsolePrintCmdList()
{
	TIziConsoleCommand *tmpCmd = iziConsoleCommandList;
	while(tmpCmd)
	{
		iziConsolePrintf_P(IZI_PSTR("%s\n\r"),tmpCmd->cmd);
		tmpCmd = tmpCmd->next;
	}
}
