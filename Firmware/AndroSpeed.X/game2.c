#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"


#define ROUNDS  3

//
//
//
uint16_t Game2(void) {
    uint16_t    score;
    char msg[]="NO-";
    uint8_t round;
    uint8_t dly;
    uint8_t i;
    uint16_t count;


    score=0;
    for (round=1; round<=ROUNDS; round++) {
        msg[2]='0'+round;
        DispMsg(msg, 100);
        dly=30+(rand()%40);
        for (i=0; i<dly; i++) {
            DispMsg("   ",10);
            ReadButtons();
            if (buttons!=0) {               // Game over if cheating
                for (i=0; i<10; i++) {
                    DispMsg("CHT",10);
                    DispMsg("   ",10);
                }
                return 0;
            }
        }
        leds=0x0f;  // Turn on all leds
        for (count=0; count<=333; count++) {
            DispValue(count);
            if ((ReadButtons()>0) || (count==333)) {
                leds=0;
                Delay10mS(200);
                score+=(333-count);
                DispMsg("   ",75);
                break;
            }
        }
    }



    return score;
}



