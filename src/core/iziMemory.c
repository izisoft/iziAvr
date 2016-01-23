#include <string.h>

#include <util/atomic.h>
#include <core/iziMemory.h>
#include <core/kernel/iziKernelPriv.h>
#include <device/iziDevicePriv.h>

#define IZI_MALLOC_PATTERN		0xAE
#define IZI_FREE_PATTERN		0xBD

#ifdef IZI_MEMORY_MONITOR

volatile uint16_t gIziUsedMemory = 0;

void* iziMalloc(size_t size)
{
	void* memBlock;
	IZI_ATOMIC_BLOCK()
	{
		memBlock = malloc(size);
		if(memBlock != NULL)
		{
			memset(memBlock,IZI_MALLOC_PATTERN,size);
			gIziUsedMemory += (size+2);
		}
		else
		{
			IZI_FATAL(eIziMallocError);
		}
	}
	return memBlock;
}

void iziFree(void *ptr)
{
	size_t size = 0;
	if(ptr != NULL)
	{
		size = *((size_t*)(ptr-2));
		memset(ptr,IZI_FREE_PATTERN,size);
		IZI_ATOMIC_BLOCK()
		{
			free(ptr);
			gIziUsedMemory -= (size+2);
		}
	}
}

IziSize_t iziGetUsedMemory()
{
	uint16_t usedMemory;
	IZI_ATOMIC_BLOCK()
	{
		usedMemory = gIziUsedMemory;
	}
	return usedMemory;
}

#else

void* iziMalloc(size_t size)
{
	void* memBlock = NULL;
	IZI_ATOMIC_INSTRUCTION(memBlock = malloc(size));
	return memBlock;
}

void iziFree(void *ptr)
{
	IZI_ATOMIC_INSTRUCTION(free(ptr));
}

#endif
