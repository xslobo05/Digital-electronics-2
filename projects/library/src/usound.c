/*********************************************************************************/
/*  Ultrasonic range finder HC-SR04 lib for AVR									 */
/*  Autor: Kornuta Taras														 */
/*  Date: 21.04.2016															 */
/*	URL: http://digiua.com														 */
/*********************************************************************************/


#ifndef USOUND_H
#define USOUND_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>


// Ports initialization
#define US_DDR		DDRD
#define US_PORT		PORTD
#define US_PIN		PIND
#define US_TRIGER	PIND2
#define US_ECHO		PIND4


 void usound_init(void);				//Paste in Main function

 uint32_t readDistance(void);			//Return the distance in centimeters
										//Recommended use this function with 20Hz frequency 



#endif //USOUND_H





//#include "usound.h"


void usound_init(void)
{
	US_DDR |= (1 << US_TRIGER)|(0 << US_ECHO);
	US_PIN |= (1<<US_ECHO);
}

uint32_t readDistance(void)
{
	uint8_t readStatus=0;
	uint32_t disTime=0;

	_delay_us(2);
	US_PORT |= (1 << US_TRIGER);
	_delay_us(10);
	US_PORT &= ~(1 << US_TRIGER);

	
	while(readStatus == 0)
	{
		while(US_PIN&(1<<US_ECHO))
		{
			disTime++;
			readStatus = 1;
		}
	}	

	return disTime * 0.00862;	//return data in cm	
}
