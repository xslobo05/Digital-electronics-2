#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "uart.h"
#include "nokia5110.h"
#include "usound.h"

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/* Call assembly functions to generate psudo-random values. */


/* Main --------------------------------------------------------------*/
/* Generate a sequence of LFSR preudo-random values using 4- and 8-bit
 * structure. */



int main(void)
{
    uint32_t distance;
    char dist[5];

    /* Inicializace displeje */
    nokia_lcd_init();                               
    nokia_lcd_clear();
    nokia_lcd_write_string("Vzdalenost[cm]",1);
    nokia_lcd_render();
    
    /* Inicializace HCSR04 senzoru vzdalenosti */
    usound_init();

    for(;;){
        
    /* Snímání vzdálenosti a zobrazení na displeji */
        _delay_ms(100);                         
        distance = readDistance();              
        itoa(distance,dist,10);
        nokia_lcd_set_cursor(0,20);
        nokia_lcd_write_string("       ",3);
        nokia_lcd_set_cursor(0,20);
        nokia_lcd_write_string(dist,3);
        nokia_lcd_render();        
    }
}

/* Interrupts --------------------------------------------------------*/
/* Timer1 overflow interrupt routine.
 * Update state of the FSM. */


