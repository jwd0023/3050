/* - File: interrupt_handlers.h 

       - Description
           - Provides various definitions and function declarations
             used in interrupt handling.
             
       - Author: Joe Driscoll 
*/


#ifndef INTERRUPT_HANDLERS_H
#define INTERRUPT_HANDLERS_H


#include <stdint.h>
#include "STM32L1xx.h"
#include "pin_definitions.h"


/* Variable used by main function that only interrupt handlers can access. */
static uint16_t count_direction;


/* Provides read access to count_direction from external files. */
uint8_t get_count_direction(void);


/* Toggles a bitfield using BSRR.*/
void toggle_GPIO_bitfield(GPIO_TypeDef *gpio, uint16_t bitfield);


#endif
