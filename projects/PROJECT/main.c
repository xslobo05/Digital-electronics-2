#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "timer.h"
#include "nokia5110.h"
#include "usound.h"
#include "twi.h"
#include "uart.h"
#include <stdbool.h> 

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/

#define POCET_KOL 7+1
#define UART_BAUD_RATE 9600
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
    uint8_t adjust_round = 0;
    char round_number[3];

    char disp_min[3], disp_sec[3];

    if( round == 0){
        casy[0][0] = count_min;
        casy[0][1] = count_sec;
        itoa(casy[0][1],disp_sec,10);
        itoa(casy[0][0],disp_min,10);
        //nokia_lcd_set_cursor(0,10);
        //nokia_lcd_write_string("       ",1);
        nokia_lcd_set_cursor(30,0);
        if(track == 1){
            nokia_lcd_set_cursor(0,10);
            nokia_lcd_write_string("START",1);
        }
        if(track == 2){
            nokia_lcd_set_cursor(48,10);
            nokia_lcd_write_string("START",1);
        }
        nokia_lcd_render();
    }

    if( round > 0 &&  round < round_max){
        time_dif(count_min,count_sec,casy[0][0],casy[0][1],result);
        casy[round][0] = result[0];
        casy[round][1] = result[1];
        itoa(casy[round][1],disp_sec,10);
        itoa(casy[round][0],disp_min,10);
        itoa(round,round_number,10);
        
        if(round > 3){
            adjust_round = 4; //deleni dvema, pozor na pocet kol, ale flexibilnejsi
        }

        if(track == 1){
            if(round == 4){
                nokia_lcd_set_cursor(0,10);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(0,20);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(0,30);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(0,40);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(0,(round + 1 - adjust_round)*10);
                nokia_lcd_write_string(round_number,1); nokia_lcd_write_string(".",1);
                nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
            }else{
                nokia_lcd_set_cursor(0,(round + 1 - adjust_round)*10);
                nokia_lcd_write_string(round_number,1); nokia_lcd_write_string(".",1);
                nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
            }
        }

        if(track == 2){
            if(round == 4){
                nokia_lcd_set_cursor(48,10);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(48,20);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(48,30);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(48,40);nokia_lcd_write_string("      ",1);
                nokia_lcd_set_cursor(48,(round + 1 - adjust_round)*10);
                nokia_lcd_write_string(round_number,1); nokia_lcd_write_string(".",1);
                nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
            }else{
                nokia_lcd_set_cursor(48,(round + 1 - adjust_round)*10);
                nokia_lcd_write_string(round_number,1); nokia_lcd_write_string(".",1);
                nokia_lcd_write_string(disp_min,1); nokia_lcd_write_string(":",1); nokia_lcd_write_string(disp_sec,1);
            }
        }
        nokia_lcd_render();
    }
}

//HC05 TEST - start
    
//HC05 TEST - end




int main(void)
{
    uint32_t distance, rtc;
    char dist[5], zobr_kol[3], zobr_min[3],zobr_sek[3];
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

    sei();
    /* Inicializace DS3231 casoveho obvodu */
    twi_init();
    
    /* Inicializace HCSR04 senzoru vzdalenosti */
    usound_init();

    /* UART: asynchronous, 8-bit data, no parity, 1-bit stop */
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    //uart_puts("UART testing");
    //uart_puts_p("UART testing");

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
            itoa(kola1-1, zobr_kol,10);
            itoa(casy1[kola1-1][0], zobr_min,10);
            itoa(casy1[kola1-1][1], zobr_sek,10);
            uart_puts("\n\r\n\rDraha 1 prujezd, kolo: "); uart_puts(zobr_kol); uart_puts(", cas: "); uart_puts(zobr_min); uart_puts(":"); uart_puts(zobr_sek);
            _delay_ms(500);
        }
        else if(distance >=9 && distance <= 16){
            prujezd(2,POCET_KOL,kola2,pocitani_minut,pocitani_sekund,casy2);
            if (kola2 < POCET_KOL) kola2++;
            itoa(kola2-1, zobr_kol,10);
            itoa(casy2[kola2-1][0], zobr_min,10);
            itoa(casy2[kola2-1][1], zobr_sek,10);
            uart_puts("\n\r\n\rDraha 2 prujezd, kolo: "); uart_puts(zobr_kol); uart_puts(", cas: "); uart_puts(zobr_min); uart_puts(":"); uart_puts(zobr_sek);
            _delay_ms(500);
        }
        nokia_lcd_set_cursor(0,0);
        nokia_lcd_write_string("    ",1);
        nokia_lcd_set_cursor(0,0);
        nokia_lcd_write_string(dist,1);
        nokia_lcd_render();       
    }
}

/* Interrupts --------------------------------------------------------*/
/* Timer1 overflow interrupt routine.
 * Update state of the FSM. */
