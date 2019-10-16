#include "gpio.h"
#define DDR(x) (*(&x - 1))

void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name |= _BV(pin_num);
}

void GPIO_toggle(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name ^= _BV(pin_num);
}

/*uint8_t GPIO_PWM(volatile uint8_t *reg_name, uint8_t pin_num, uint8_t pwm)
{
    uint8_t pwm3=0;
    for(uint8_t i = 0; i <= 100; i++)
    {
        if i<=pwm
        {
           *reg_name |= _BV(pin_num); 
        }
        else 
        {
            *reg_name &= ~_BV(pin_num); 
        }       
    }
    
    pwm++;
    if pwm==100 pwm = 0;
    pwm3=pwm;
    return pwm3;
}*/

void GPIO_write(volatile uint8_t *reg_name, uint8_t pin_num,uint8_t pin_val)
{
    if(pin_val == 1)
    {
        *reg_name |= _BV(pin_num);
    }
    else
    {
        *reg_name &= ~_BV(pin_num);
    }
}

void GPIO_config_input_nopull(volatile uint8_t *reg_name, uint8_t pin_num)
{
    DDR(*reg_name) &= ~ _BV(pin_num);
    *reg_name &= ~ _BV(pin_num);
}

void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num)
{
    DDR(*reg_name) &= ~ _BV(pin_num);
    *reg_name |= _BV(pin_num);
}