/* Linker script to configure memory regions. */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
	FLASH (rx)    : ORIGIN = 0x00000000, LENGTH = 0x80000
	RAM (rwx)     : ORIGIN = 0x20000000, LENGTH = 0x0FF00
	SAVERAM (rwx) : ORIGIN = 0x2000FF00, LENGTH = 0x00100
}

/* export saveram memory locations */

saveram        = ORIGIN(SAVERAM);
saveram_origin = ORIGIN(SAVERAM);
saveram_length = LENGTH(RAM);



INCLUDE "nrf_common.ld"

