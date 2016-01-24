# iziAvr
Lightweight preemptive scheduler for **Atmel AVR 8-bit** microcontrollers family.


## Configuration

#### IZI_SYSTEM_TICK_SOURCE

Indicates which interrupt will be taken as the system tick. Depending on device resources it 
can take value between *0* and *4*:

| Value  | Macro                     | Description |
| :----: | ------------------------- | --------------- |
|   0    | IZI_SYSTEM_TICK_TC0       | Timer Counter 0 output compare |
|   1    | IZI_SYSTEM_TICK_TC1A      | Timer Counter 1 output compare A |
|   2    | IZI_SYSTEM_TICK_TC1B      | Timer Counter 1 output compare B |
|   3    | IZI_SYSTEM_TICK_TC2       | Timer Counter 2 output compare |
|   4    | IZI_SYSTEM_TICK_TC2_ASYNC | Timer Counter 2 output compare in asynchronous working mode <sup>1</sup> |

*<sup>1</sup> Asynchronous mode assumes using external low frequency oscillator (32768 Hz)*
 
#### IZI_SYSTEM_TICK_RATE

Number of system ticks per second. It can be any number between 1 and 1000 (default value is 256).

#### IZI_KERNEL_SCHEDULER

Type of task scheduler used. Currently two scheduler implementations are available:

| Value  | Macro                     | Description |
| :----: | ------------------------- | --------------- |
|   0    | IZI_KERNEL_SCHEDULER_HPF  | *High Priority First* |
|   1    | IZI_KERNEL_SCHEDULER_FTS  | *Fair Time Share* |

#### IZI_KERNEL_SIZE


## Examples

Blink LED connected to 5<sup>th</sup> pin of port A.
```c

#include <core/iziKernel.h>
#include <driver/iziPin.h>

void blink()
{
	iziPinSetType(eIziPinA5, eIziPinOut);
	
	for(;;)
	{
		iziPinToggle(eIziPinA5);
		iziTaskDelay(100);
	}
}

int iziMain()
{
	iziTaskCreate(blink, eIziPrioLow, 100, NULL);
	iziKernelStart();
	return 0;
}
```