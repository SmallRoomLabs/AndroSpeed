#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"


#define PLUSTIME    25
#define MINUSTIME   100
#define SPEEDHIT    15
#define SCOREFLASHES 10
#define NEXTDELAY   10

//
//
//
uint16_t Game1(void) {
    uint8_t     i,j;
    uint16_t    countdown;
    uint16_t    nextled;
    uint16_t    score;
    uint8_t     target;
    uint8_t     hit;
    uint8_t     released;


    for (countdown=0; countdown<1000; countdown+=3) {
        DispValue(countdown);
        __delay_ms(1);
    }

    score=0;
    countdown=999;
    nextled=countdown-NEXTDELAY;
    target=0;
    released=1;


    while (countdown>0) {
        DispValue(countdown);
        if (countdown==999) {
            for (i=0; i<20+(rand()%30); i++) {
                Delay10mS(10);
            }
        }
        if (nextled==countdown) {
            target=1+(rand()&0x03);
        }
        __delay_ms(10);
        ReadButtons();
        leds=buttons;
        leds|=1<<(target-1);
        if (buttons==0) released=1;
        if ((buttons!=0)  && (released==1)) {
            released=0;
            hit=0;
            if ((buttons&1) && (target==1)) hit=1;
            if ((buttons&2) && (target==2)) hit=1;
            if ((buttons&4) && (target==3)) hit=1;
            if ((buttons&8) && (target==4)) hit=1;
            if (hit) {
                score++;
                if (nextled-countdown<SPEEDHIT) {
                    countdown+=PLUSTIME;
                    if (countdown>999) countdown=999;
                }
                target=0;
                nextled=countdown-NEXTDELAY;
            } else {
                if (countdown>MINUSTIME) countdown-=MINUSTIME;
            }
        }

        countdown--;
    } // while countdown>0

    return score;
}


