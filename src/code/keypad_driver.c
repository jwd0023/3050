#include "keypad_driver.h"
#include "stdlib.h"


/* Helper function for writing a value to a GPIO's ODR using the BSRR. */
static void write_to_odr(uint16_t value, uint16_t shift, uint16_t bitmask)
{
     uint32_t masked_value = value & bitmask;
     KEYPAD_GPIO->BSRR |= (masked_value << shift) | ((masked_value ^ bitmask) << (16 + shift));
}


/* Reads the key pressed on the keypad and returns the number it represents. */
uint16_t read_keypress(void)
{
    /* Iterate columns and read every row per interation. */
    for (uint8_t column = 0; column < KEYPAD_NUM_COLUMNS; column++)
    {
        /* Drive column low. */
        write_to_odr(0x0, column, 0x1);
        
        /* Short delay - hopefully no optimization flags are on. (this is according to specification)*/
        for (int k = 0; k < 4; k++);
        
        /* Are any rows driven low? */
        
    }
    
    /* If function reaches here, then there was an error. Always output F to detect error on testing. */
    return 0xF;
}


/* Decode row-col pair to know what key was pressed. 
       - Note: Does not work with pound or asterisk keys. */
uint16_t decode_row_col(uint16_t row, uint16_t col)
{   
    /* Is the key a letter? */
    if (col == 4)
    {
        return 0xA + (row - 1);
    }
    else
    {
        return col + 3 * (row - 1);
    }    
}
