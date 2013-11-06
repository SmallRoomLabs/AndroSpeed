#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"

#define TIMEOUT 250
#define OFFDELAY 33
#define ONDELAY  33

const uint8_t wave[]="______-__^-_^^--^^_-^__-______";

//
//
//
uint16_t Game3(void) {
    uint8_t     score;
    uint8_t     sequence[12];   // Each byte is 4 tones = 48 tones
    uint8_t     i,j;
    uint8_t     n;

    score=0;
    // Generate a new full sequence
    for (i=0; i<sizeof(sequence); i++) {
        sequence[i]=rand()&0xff;
    }

    for (score=0; score<sizeof(sequence)*4; score++ ) {
        DispMsg("   ",100);
        // Play sequence up to current length
        for (i=0; i<score+1; i++) {
            n=sequence[i/4];
            n=n>>((i&0x03)*2);
            n=n&0x03;
            leds=0;
            setbit(leds,n);
            Delay10mS(ONDELAY);
        }

        leds=0;

        // Now get it back from the user
        for (i=0; i<score+1; i++) {
            n=sequence[i/4];
            n=n>>((i&0x03)*2);
            n=n&0x03;

            for (j=0; j<TIMEOUT; j++) {
                Delay10mS(1);
                ReadButtons();
                if ((n==0) && (buttons==1)) {leds=buttons; break;}
                if ((n==1) && (buttons==2)) {leds=buttons; break;}
                if ((n==2) && (buttons==4)) {leds=buttons; break;}
                if ((n==3) && (buttons==8)) {leds=buttons; break;}
                if (buttons!=0) {
                DispMsg("ERR",150);
                return score;
                }
            }
            while (ReadButtons()!=0); // Wait until released.
            leds=0;
            if (j==TIMEOUT) {
                DispMsg("TIM",150);
                return score;
            }
        }

        // Whoo! succesfully repetated the sequence - do the "Wave" :-)
        Delay10mS(100);
        for (i=0;i<30;i+=3) {
            DispMsg(&wave[i],10);
        }

    }   // Make sequence 1 step longer

    return score;
}

