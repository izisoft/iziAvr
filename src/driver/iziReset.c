#include <avr/io.h>
#include <avr/wdt.h>

#include <izi/avr/driver/reset.h>
#include <izi/avr/types.h>

void iziReset(TIziResetType resetType)
{
	cli();
	wdt_enable(WDTO_15MS);
	for(;;);
}
