/* - File: keypad_driver.h 

       - Description
           - A sort of device driver for the keypad
             used in the projects.
             
       - Author: Joe Driscoll 
*/


#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H


#include <stdint.h>
#include "STM32L1xx.h"


#define KEYPAD_NUM_COLUMNS  4
#define KEYPAD_NUM_ROWS     4


#define ROW_BITFIELD ((uint16_t) (0xF0))
#define COL_BITFIELD ((uint16_t) (0x0F))


#define ROW_OFFSET 4
#define COL_OFFSET 0


/* Interconnect-dependent definitions. */
#define KEYPAD_GPIO             GPIOC
#define KEYPAD_ROW_INPUT_DATA   (KEYPAD_GPIO->IDR & ROW_BITFIELD)
#define KEYPAD_COL_INPUT_DATA   (KEYPAD_GPIO->IDR & COL_BITFIELD)
#define KEYPAD_INPUT_DATA       (KEYPAD_COL_INPUT_DATA | KEYPAD_ROW_INPUT_DATA)


/* Definitions for setting up GPIO connections. */
#define GPIOB_RCC_EN 0x02
#define RCC_AHBENR

typedef struct
{
    uint16_t row_bitfield;
    uint16_t col_bitfield;
    GPIO_TypeDef gpio;
    uint16_t (*read_keypress)(void);
    
} *keypad_interface;   


/* Function for creating a keypad interface - only non-static function in file. */
keypad_interface create_keypad_interface(GPIO_TypeDef gpio, uint16_t r_bitfield, uint16_t c_bitfield);


/* Helper function for writing a value to a GPIO's ODR using the BSRR. */
static void write_to_odr(GPIO_TypeDef gpio, uint16_t value, uint16_t shift, uint16_t bitmask);


/* Reads the key pressed on the keypad and returns the number it represents. */
static uint16_t read_keypress(void);


/* Decode row-col pair to know what key was pressed. */
static uint16_t decode_row_col(uint16_t row, uint16_t col);

#endif