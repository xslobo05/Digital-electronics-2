/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-11-28
 * Last update: 2019-10-11
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Control LEDs using functions from GPIO and Timer 
 *              libraries. Do not use delay library any more.
 * TODO: Create and test functions to control 8-bit Timer/Counter0 and 
 *       16-bit Timer/Counter1 modules.
 * NOTE: Store timer.c and timer.h files in library/src and library/inc
 *       folders.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include "gpio.h"
#include "timer.h"
#include <avr/interrupt.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_GREEN   PB5
#define LED_RED     PB0
#define PIN_LOW 0
#define PIN_HIGH 1

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/
//uint8_t pwm = 0;
//uint8_t pwm2 = 0;
/* Functions ---------------------------------------------------------*/
int main(void)
{
    /* LED_GREEN */
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write(&PORTB, LED_GREEN, PIN_LOW);
    /* LED_RED */
    // TODO: Configure red LED
    GPIO_config_output(&DDRB,LED_RED);
    GPIO_write(&PORTB, LED_RED, PIN_HIGH);

    /* Set Timer0 */
    TIM_config_prescaler(TIM0, TIM_PRESC_64);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    /* Set Timer1 */
    // TODO: Configure Timer1 clock source and enable overflow interrupt
    TIM_config_prescaler(TIM1, TIM_PRESC_1024);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    /* Enable interrupts by setting the global interrupt mask */
    sei();

    /* Infinite loop */
    for (;;) {
    }

    return (0);
}

/**
 *  Brief: Timer/Counter0 overflow interrupt routine. Toggle green LED.
 */
ISR(TIMER0_OVF_vect)
{
    GPIO_toggle(&DDRB,LED_GREEN);
    //pwm2=GPIO_PWM(&DDRB,LED_RED,pwm);
    //pwm=pwm2;
}

/**
 *  Brief: Timer/Counter1 overflow interrupt routine. Toggle red LED.
 */
ISR(TIMER1_OVF_vect)
{
    // TODO: Toggle red LED
    GPIO_toggle(&DDRB,LED_RED);
}
