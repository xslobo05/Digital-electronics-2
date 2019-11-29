/*
#include "project.h"

uint8_t time_dif(uint8_t min1, uint8_t sec1, uint8_t min2, uint8_t sec2, uint8_t *result)
// time1 - time2 = result 
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
*/