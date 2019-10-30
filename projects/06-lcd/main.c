/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-16
 * Last update: 2019-10-25
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Decimal counter with data output on LCD display.
 * 
 * Note:
 *    Modified version of Peter Fleury's LCD library with R/W pin 
 *    connected to GND. Newline symbol "\n" is not implemented, use
 *    lcd_gotoxy() function instead.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>             // itoa() function
#include "timer.h"
#include "lcd.h"
#include <stdio.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on LCD display.
 *  Input:  None
 *  Return: None
 */

uint8_t count = 0;
char str[4];

int main(void)
{
    
    
    /* LCD display
     * TODO: See Peter Fleury's online manual for LCD library 
     * http://homepage.hispeed.ch/peterfleury/avr-software.html
     * Initialize display and test different types of cursor */
    lcd_init(LCD_DISP_ON);

    // Display string without auto linefeed
    lcd_puts("Counter  ");
    //itoa(count,str,10);
    //lcd_puts(str);
    

    // TODO: Display variable value in decimal, binary, and hexadecimal

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE); 

    /* TODO: Design at least two user characters and store them in 
     *       the display memory */    
    uint8_t i;

    uint8_t  lcd_user_symbols [8*2] = {  
                                        0x00,
                                        0x00,
                                        0x0A,
                                        0x00,
                                        0x0E,
                                        0x0A,
                                        0x00,
                                        0x00,
                                            0x00,
                                            0x13,
                                            0x13,
                                            0x00,
                                            0x00,
                                            0x1A,
                                            0x1B,
                                            0x00
                                    };
    
    lcd_command(1<<LCD_CGRAM);


    for(i=0;i<16;i++)
    {
        lcd_data(lcd_user_symbols [i]);
    }

    lcd_gotoxy(0,1);
    lcd_putc(0x00);


    lcd_gotoxy(1,1);
    lcd_putc(0x01);


    // Enables interrupts by setting the global interrupt mask
    sei();

    //itoa(count, str, 10);
    //lcd_gotoxy(9,0);
    //lcd_puts(str);

    // Infinite loop
    for (;;) {
    }
    
    return (0);
}

/**


    uint8_t  lcd_user_symbols2 [8*2]  = {  
                                            0x00,
                                            0x13,
                                            0x13,
                                            0x00,
                                            0x00,
                                            0x1A,
                                            0x1B,
                                            0x00
                                    };
    
    lcd_command(1<<LCD_CGRAM);

    for(i=0;i<8;i++)
    {
        lcd_data(lcd_user_symbols2 [i]);
    }




 *  Brief: Timer1 overflow interrupt routine. Increment counter value.
 */
ISR(TIMER1_OVF_vect)
{
    // TODO: Increment counter value form 0 to 255
    
    itoa(count, str, 10);
    lcd_gotoxy(9,0);
    lcd_puts(str);
    count++;
    if(count==0){
        lcd_gotoxy(10,0);
        lcd_puts("  "); 
    }
}