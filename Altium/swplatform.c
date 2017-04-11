/*
 *	Software Platform Generated File
 *	--------------------------------
 */


#include "swplatform.h"

/* Global variables to access Software Platform stacks */
uart8_t * drv_uart8_3;
uart8_t * drv_uart8_4;
uart8_t * drv_uart8_2;
uart8_t * drv_uart8_main;
uart8_t * drv_uart8_1;
int       serial_1;

/* Initialize all stacks in the Software Platform */
void swplatform_init_stacks(void)
{
    drv_uart8_3    = uart8_open(DRV_UART8_3);
    drv_uart8_4    = uart8_open(DRV_UART8_4);
    drv_uart8_2    = uart8_open(DRV_UART8_2);
    drv_uart8_main = uart8_open(DRV_UART8_MAIN);
    drv_uart8_1    = uart8_open(DRV_UART8_1);
    serial_1       = open("/dev/SERIAL_1", O_RDWR);
}
