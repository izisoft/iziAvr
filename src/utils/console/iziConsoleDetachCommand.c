#include <string.h>

#include <izi/avr/core/memory.h>
#include <izi/avr/utils/console.h>

#include "iziConsoleCommon.h"

IziBool_t iziConsoleDetachCommand(const char* cmd)
{
	TIziConsoleCommand *iter = iziConsoleCommandList;
	TIziConsoleCommand *tmp;
	
	if(iter != NULL)
	{
		if(strcmp(iter->cmd,cmd) == 0)
		{
			iziConsoleCommandList = iter->next;
			iziFree(iter);
			return IziTrue;
		}
		while(iter->next != NULL)
		{
			if(strcmp(iter->next->cmd,cmd) == 0)
			{
				tmp = iter->next->next;
				iziFree(iter->next);
				iter->next = tmp;
				return IziTrue;
			}
			iter = iter->next;
		}
	}
	return IziFalse;
}

