#ifndef IZIMEMORY_H_
#define IZIMEMORY_H_

#include <stdlib.h>

#include <iziAvr.h>
#include <iziConf.h>

#ifdef __cplusplus
extern "C" {
#endif

void* iziMalloc(size_t size);

void iziFree(void *ptr);

#ifdef IZI_MEMORY_MONITOR

IziSize_t iziGetUsedMemory();

#else

#define iziGetUsedMemory() (0)

#endif

#ifdef __cplusplus
}
#endif

#endif /* IZIMEMORY_H_ */
