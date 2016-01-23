#include <avr/io.h>
#include <avr/wdt.h>

#include <iziAvr.h>

#include <driver/iziReset.h>

//=====================================================================
void iziReset(TIziResetType resetType)
{
	cli();
	wdt_enable(WDTO_15MS);
	for(;;);
}
