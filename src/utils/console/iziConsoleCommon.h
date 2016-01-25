#ifndef IZI_CONSOLECOMMON_H_
#define IZI_CONSOLECOMMON_H_

#include <izi/avr/utils/console.h>

typedef struct SIziConsoleCommand TIziConsoleCommand;
struct SIziConsoleCommand
{
	const char* cmd;
	TIziConsoleCallback callback;
	TIziConsoleCommand* next;
};

extern TIziConsoleCommand *iziConsoleCommandList;

#endif /* IZI_CONSOLECOMMON_H_ */
