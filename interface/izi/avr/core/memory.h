#ifndef IZI_MEMORY_H_
#define IZI_MEMORY_H_

#include <stdlib.h>

#include <izi/avr/types.h>
#include <izi/avr/config.h>

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

#endif /* IZI_MEMORY_H_ */