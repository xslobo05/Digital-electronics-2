/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-09-27
 * Last update: 2019-09-26
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Blink a LED with delay function.
 * TODO: Verify functionality of LED blinker.
 * NOTE: Macro _BV converts a bit number into a byte value (see AVR Libc
 * Reference Manual).
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_GREEN   PB5
#define LED_OUT     PB0
#define BUT      PD2
#define BLINK_DELAY 250

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
  * Brief:  Main program. Blink a LED with delay function.
  * Input:  None
  * Return: None
  */
int main(void)
{
    /* Set output pin */
   /* DDRB |= _BV(LED_GREEN);        
    DDRB |= _BV(LED_OUT);*/
    GPIO_config_output(&DDRB,LED_GREEN);
    GPIO_config_output(&DDRB,LED_OUT);

    GPIO_config_input_pullup(&PORTD, BUT);

    /*DDRD &= ~_BV(BUT);
    PORTD |= _BV(BUT);5555555*/

    /* Turn LED off */
    PORTB &= ~_BV(LED_GREEN);       /* PORTB = PORTB and (0010 0000) */
    PORTB &= ~_BV(LED_OUT);

    /* Infinite loop */
    for (;;)
    {

      if (bit_is_set(PIND , BUT))  
      {
        GPIO_toggle(&PORTB,LED_GREEN);  
        _delay_ms(BLINK_DELAY);     
      }
      else{
        GPIO_toggle(&PORTB,LED_OUT);
        _delay_ms(BLINK_DELAY);
      }
    }
    return (0);
}
