#include <izi/avr/utils/console.h>
#include <izi/avr/core/memory.h>

#include "iziConsoleCommon.h"

IziBool_t iziConsoleAttachCommand(const char* cmd, TIziConsoleCallback callback)
{
	TIziConsoleCommand *iter = iziConsoleCommandList;
	TIziConsoleCommand *newCmd;
	
	newCmd = iziMalloc(sizeof(TIziConsoleCommand));
	newCmd->cmd = cmd;
	newCmd->callback = callback;
	newCmd->next = NULL;
	
	if(iter == NULL)
	{
		iziConsoleCommandList = newCmd;
	}
	else
	{
		while(iter->next != NULL)
		{
			iter = iter->next;
		}
		iter->next = newCmd;
	}	
	return IziTrue;
}
