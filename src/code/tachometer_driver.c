/* - File: tachometer_driver.c

       - Description
           - Provides interface to tachometer measurements.
             
       - Author: Joe Driscoll 
*/


#include "STM32L1xx.h"
#include "timing_functions.h"
#include "stdlib.h"
#include "pin_definitions.h"
#include "keypad_driver.h"    
#include "bcd_timer.h"
#include "setup.h"


/* Allows data acquisition to be turned on and off. */
#if DATA_ACQUISITION_MODE

#define DATA_ACQUISITION_TIME        5
#define DATA_ACQUISITION_BUFFER_SIZE (DATA_ACQUISITION_TIME * 100)


static uint16_t data_acquisition_buffer[DATA_ACQUISITION_BUFFER_SIZE];
static uint32_t data_acquisition_index = 0;


#endif


/* BCD timer needs to interrupt every 100ms (10 timer interrupts) - use counter to do this. */
static uint8_t interrupt_count = 0;


/* Used to setup tachometer driver. */
void setup_tachometer_driver(void)
{
    /* Setup ADC for amplitude measurements. */
    setup_adc();
    
    /* Setup timer and set function defined in this file to handler. */
    setup_TIM11();
    
    enable_timer(TIM11);
}


/* TIM11 interrupt handler - triggers ADC measurement. */
void TIM11_IRQHandler(void)
{
    /* Start first conversion. */
    ADC1->CR2 |= ADC_CR2_SWSTART;
    
    /* Wait for EOC. */
    while ((ADC1->SR & ADC_SR_EOC) == 0);
    
    /* Do some data recording if data acquisition mode is on. */
    #if DATA_ACQUISITION_MODE
       
    if ((data_acquisition_index < DATA_ACQUISITION_BUFFER_SIZE) && (get_key_pressed() == 0xA) || get_key_pressed() == 0x8)
    {
        data_acquisition_buffer[data_acquisition_index++] = ADC1->DR;   
    }
    else if (get_key_pressed() == 0x0)
    {
        data_acquisition_index = 0;
    }
    
    #endif
 
    /* Need to make control step here. */
    controller_step(((uint32_t) (3 * ADC1->DR)) >> 2);

    /* Increment number of times this interrupt has triggered (for BCD timer). */
    interrupt_count = MOD(interrupt_count + 1, 10);
    
    /* Increment and output value of bcd counter. */
    if (interrupt_count == 0)
    {
        write_to_odr(GPIOC, (uint16_t) increment_timer(), NO_SHIFT, 0xFF);
    }
    
    /* Clear EOC to be safe. */
    ADC1->SR &= ~(0x2);
    
    /* Clear other interrupt flags and data (like timer count). */
    clear_timer(TIM11);
    clear_TIM11_interrupt();
}


/* Sets up the ADC. */
static void setup_adc(void)
{
    /* Turn on clock to ADC. */
    RCC->APB2ENR |= (0x1 << 9);
    
    /* Make ADC use PA0 as channel. */
    ADC1->SQR5 &= ~ADC_SQR5_SQ1;
    
    /* Turn on ADC and wait for power on. */
    ADC1->CR2 |= ADC_CR2_ADON;
    while((ADC1->SR & ADC_SR_ADONS) == 0);
}
