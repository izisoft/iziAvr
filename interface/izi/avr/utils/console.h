#ifndef IZI_CONSOLE_H_
	#define IZI_CONSOLE_H_

#include <izi/avr/core/task.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup iziUtils
 * @{
 */

/** \file iziConsole.h
 */

/** \def IZI_CONSOLE_BUFFER_SIZE
 *
 */
#define IZI_CONSOLE_BUFFER_SIZE 	(32)

typedef void (*TIziConsoleCallback)();

/** \fn iziConsoleInit(uint32_t baudrate)
 *	Initialization function for iziConsole utility.
 *	It handles proper UART initialization and task creation.
 *	Function can be called after attaching commands.
 *	\param baudrate
 *	Baudrate for UART interface.
 *	\param stackSize
 *	Stack size for console task (should be at least 128).
 *	\return 
 *	Function returns pointer to only instance of iziConsole task.
 */
TIziTask* iziConsoleInit(uint32_t baudrate, uint16_t stackSize);

/** \fn iziConsolePrintCmdList()
 * Function prints the list of available commands.
 */
void iziConsolePrintCmdList();

/** \fn iziConsolePrintf(const char* fmt,...)
 *	Handles formated printing to console.
 *	Function is basically a wrapper for standard printf function
 *	with it basic functionality. 
 *	\param fmt
 *	Pointer to printing format.
 *	\param ...
 *	List of format arguments.
 */
void iziConsolePrintf(const char* fmt,...);

/** \fn iziConsolePrintf_P(const char* fmt,...) 
 *	Handles formated printing to console with format stored in flash memory.
 *	Function similar to iziConsolePrintf, but format is stored in program space.
 *	\param fmt
 *	Pointer to printing format (stored in program space).
 *	\param ...
 *	List of format arguments.
 *	\see iziConsolePrintf
 */
void iziConsolePrintf_P(const char* fmt,...);

/** \fn iziConsoleAttachCommand(const char* cmd, void (*handler)())
 *	Function adds new command to iziConsole command list. After retreiving
 *	command from user iziConsole utility search through list to find if
 *	such command is binded with proper handler.
 *	\param cmd
 *	Command string.
 *	\param handler
 *	Pointer to handler function, that should be binded with command.
 *	\return
 *	Functions indicates if adding new command to command list suceeded.
 *	\note
 *	Command parameter should be static const char* which can't be modified
 *	during runtime.\n
 *	GOOD:
 *	\code
 *	status = iziConsoleAttachCommand("reset",doReset);
 *	\endcode
 *	WRONG:
 *	\code
 *	const char* command = "reset";
 *	status = iziConsoleAttachCommand(command,doReset);
 *	\endcode
 */
IziBool_t iziConsoleAttachCommand(const char* cmd, TIziConsoleCallback callback);

/** \fn iziConsoleDetachCommand(const char* cmd)
 *	Function removes command from command list.
 *	\param cmd
 *	Command string.
 *	\return
 *	If command was present on command list function returns IziTrue and 
 *	IziFalse otherwise.
 *	\see iziConsoleAttachCommand
 */
IziBool_t iziConsoleDetachCommand(const char* cmd);

/** \fn iziConsoleGetArgument(uint8_t argNum)
 *	Function gives user acces to command arguments.
 *	\param argNum
 *	Number of argument on argument list. First argument after
 *	command has number 1.
 *	\return 
 *	Function returns pointer to NULL terminated string containing
 *	proper command argumend. If accessing argument fails (e.g. argNum
 *	exceeds total number of arguments) function returns NULL pointer.
 */ 
const char* iziConsoleGetArgument(uint8_t argNum);

/** \fn iziConsoleGetArgumentAsInt(uint8_t argNum)
 *	Function alluws user to access command arguments intepred as integers.
 *	\param argNum
 *	Number of argument on argument list. First argument after
 *	command has number 1.
 *	\return
 *	Function return convertion result of proper argument to integer number.
 *	If convertion fails or wanted argument is not present function returns 0.
 */ 
int iziConsoleGetArgumentAsInt(uint8_t argNum);

/** \fn iziConsoleArgumentCount()
 *	Function informing user about total argument count in current command.
 *	\return
 *	Total number of command arguments.
 */
uint8_t iziConsoleArgumentCount();

/// @}

#ifdef __cplusplus
}
#endif

#endif //IZI_CONSOLE_H_
