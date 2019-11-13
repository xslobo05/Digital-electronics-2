/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2017-10-27
 * Last update: 2019-11-08
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Scan TWI bus for all connected slave devices and transmit info
 *    to UART.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"
#include "twi.h"

/* Typedef -----------------------------------------------------------*/
typedef enum {
    IDLE_STATE = 1,
    TRY_STATE,
    ACK_STATE
} state_t;

/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
state_t current_state = IDLE_STATE;

/* Function prototypes -----------------------------------------------*/
void fsm_twi_scanner(void);

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Test all TWI slave addresses and scan 
 *          connected devices.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // TWI
    twi_init();

    /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART.
    //uart_puts("\r\n     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Update state of TWI Finite
 *         State Machine.
 */
ISR(TIMER1_OVF_vect)
{
    uint8_t addr_temp = 0x5c;
    uint8_t addr_hum = 0x68; //actually hum=clock
    uint8_t temp_int, temp_frac, hum_int, hum_frac;
    char uart_string_temp[3];
    char uart_string_hum[3];

    twi_start((addr_temp<<1) + TWI_WRITE);
    twi_write(2);
    twi_stop();

    twi_start((addr_temp<<1) + TWI_READ);
    temp_int=twi_read_ack();
    temp_frac=twi_read_nack();

    twi_stop();

    itoa(temp_int, uart_string_temp, 16);
    uart_puts(uart_string_temp);
    uart_putc('.');
    itoa(temp_frac, uart_string_temp, 16);
    uart_puts(uart_string_temp);
    uart_puts("\r\n");

    //end_temp

    twi_start((addr_hum<<1) + TWI_WRITE);
    twi_write(0);
    twi_stop();

    twi_start((addr_hum<<1) + TWI_READ);
    hum_int=twi_read_ack();
    hum_frac=twi_read_nack();

    twi_stop();

    itoa(hum_frac, uart_string_hum, 16);
    uart_puts(uart_string_hum);
    uart_putc(':');
    itoa(hum_int, uart_string_hum, 16);
    uart_puts(uart_string_hum);
    uart_("");
}