/*******************************************************	
	Description: Printf
	
	Author: Adamir Hamulic
	Date: 2023

********************************************************/


#include <stdio.h>
#include "misc.h"
#include "uart.h"




/*********************************************************************
 * @fn      _write
 *
 * @brief   Support Printf Function - Send over TX DMA
 *
 * @param   *buf - UART send Data.
 *          size - Data length.
 *
 * @return  size - Data length
 */
__attribute__((used)) 
int _write(int fd, char *buf, int size)
{
	uart_send_data_dma(buf, size);

    return size;
}

/*********************************************************************
 * @fn      _sbrk
 *
 * @brief   Change the spatial position of data segment.
 *
 * @return  size: Data length
 */
void *_sbrk(ptrdiff_t incr)
{
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
    return NULL - 1;

    curbrk += incr;
    return curbrk - incr;
}
