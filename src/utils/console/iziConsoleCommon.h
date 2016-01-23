#ifndef IZICONSOLECOMMON_H_
#define IZICONSOLECOMMON_H_

#include <utils/iziConsole.h>

typedef struct SIziConsoleCommand TIziConsoleCommand;
struct SIziConsoleCommand
{
	const char* cmd;
	TIziConsoleCallback callback;
	TIziConsoleCommand* next;
};

extern TIziConsoleCommand *iziConsoleCommandList;

#endif /* IZICONSOLECOMMON_H_ */
