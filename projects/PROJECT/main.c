#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "uart.h"
#include "nokia5110.h"
#include "usound.h"
#include "twi.h"
#include <stdbool.h> 

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/

#define POCET_KOL 3+1
/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
uint8_t time_dif(uint8_t min1, uint8_t sec1, uint8_t min2, uint8_t sec2, uint8_t *result)
/* time1 - time2 = result */
{
    uint8_t min_result, sec_result;
    
    if (min2 > min1)    //vrati nulu kdyz by vysel zaporny cas
    {
        return 0;
    }

    else if (sec1 < sec2 && min1 > 0)    //pri presahu minut
    {
        sec_result = 60 - (sec2-sec1);
        min_result = min1 - min2 -1;
        
    }
    else
    {
        sec_result = sec1 - sec2;
        min_result = min1 - min2;
        
    }
    result[0] = min_result;
    result[1] = sec_result;
    return 1;
}
 /* Fce vypocte dobu kola a zobrazi na dipleji */
void prujezd(uint8_t track, uint8_t round_max,uint8_t round, uint8_t count_min, uint8_t count_sec, uint8_t casy[POCET_KOL][2])
{
    uint8_t result[2];

    char disp_min[3], disp_sec[3];

    if( round == 0){
        casy[0][0] = count_min;
        casy[0][1] = count_sec;
        itoa(casy[0][1],disp_sec,10);
        itoa(casy[0][0],disp_min,10);
        nokia_lcd_set_cursor(0,10);
        nokia_lcd_write_string("       ",1);
        nokia_lcd_set_cursor(30,0);
        nokia_lcd_write_string("START",1);
        nokia_lcd_render();
    }

    if( round > 0 &&  round < round_max){
        time_dif(count_min,count_sec,casy[0][0],casy[0][1],result);
        casy[round][0] = result[0];
        casy[round][1] = result[1];
        itoa(casy[round][1],disp_sec,10);
        itoa(casy[round][0],disp_min,10);
        
        if(track == 1){
        nokia_lcd_set_cursor(0,(round + 1)*10);
        //nokia_lcd_write_string("       ",1);
        nokia_lcd_set_cursor(0,(round + 1)*10);
        nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
        }

        if(track == 2){
        nokia_lcd_set_cursor(40,(round + 1)*10);
        //nokia_lcd_write_string("       ",1);
        nokia_lcd_set_cursor(40,(round + 1)*10);
        nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
        }
        
        nokia_lcd_render();
    
    }




}

int main(void)
{
    uint32_t distance, rtc;
    char dist[5];
    //char zobrazeni_sekund[3], zobrazeni_minut[3];
    bool detekce=1;
    uint8_t adresa_clocku = 0x68;
    uint8_t pocitani_minut=0, pocitani_desitek=0, pocitani_sekund=0;
    uint8_t casy1[POCET_KOL][2], kola1=0, casy2[POCET_KOL][2], kola2=0;
    int i,j;
    //uint8_t result[2];

    /* Inicializace displeje */
    nokia_lcd_init();                               
    nokia_lcd_clear();
    //nokia_lcd_write_string("Vzdalenost[cm]",1);
    //nokia_lcd_render();

    /* Inicializace DS3231 casoveho obvodu */
    twi_init();
    
    /* Inicializace HCSR04 senzoru vzdalenosti */
    usound_init();

    for(i=0;i<POCET_KOL;i++)
    {
        for(j=0;j<2;j++){
            casy1[i][j] = 0;
            casy2[i][j] = 0;
        }
    }

    for(;;){
        
    /* Snímání vzdálenosti a zobrazení na displeji */
        _delay_ms(100);                         
        distance = readDistance();              
        itoa(distance,dist,10);

        twi_start((adresa_clocku<<1) + TWI_WRITE);
        twi_write(0);
        twi_stop();
        twi_start((adresa_clocku<<1) + TWI_READ);
        rtc=twi_read_nack();
        twi_stop();

    /* Převod času z RTC na minuty a sekundy */
        pocitani_sekund = 0b00001111 & rtc;
        rtc = rtc >> 4;
        pocitani_desitek = rtc;
   
        pocitani_sekund=10*pocitani_desitek + pocitani_sekund;
        if(pocitani_sekund == 59)   detekce=1;
    
        if (pocitani_sekund == 0 && detekce)
        {
            pocitani_minut++;
            detekce=0;
        }

    /* Detekce průjezdu a měření času */
        if(distance <= 8){
            prujezd(1,POCET_KOL,kola1,pocitani_minut,pocitani_sekund,casy1);
            if (kola1 < POCET_KOL) kola1++;
            _delay_ms(1000);
        }
        if(distance >=9 && distance <= 16){
            prujezd(2,POCET_KOL,kola2,pocitani_minut,pocitani_sekund,casy2);
            if (kola2 < POCET_KOL) kola2++;
            _delay_ms(1000);
        }
        
        
    

        nokia_lcd_set_cursor(0,0);
        nokia_lcd_write_string("   ",1);
        nokia_lcd_set_cursor(0,0);
        nokia_lcd_write_string(dist,1);
        nokia_lcd_render();       
    }
}

/* Interrupts --------------------------------------------------------*/
/* Timer1 overflow interrupt routine.
 * Update state of the FSM. */


