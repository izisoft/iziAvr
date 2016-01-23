#ifndef IZILOGGER_H_
#define IZILOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** \def IZI_LOGGER_BUFFER_SIZE
 *
 */
#define IZI_LOGGER_BUFFER_SIZE 	(32)

/** \fn iziLoggerInit(uint32_t baudrate);
 *
 */
void iziLoggerInit(uint32_t baudrate);

/** \fn iziLog(const char *fmt,...)
 *
 * @param fmt
 */
void iziLog(const char *fmt,...);

#ifdef __cplusplus
}
#endif

#endif /* IZILOGGER_H_ */
