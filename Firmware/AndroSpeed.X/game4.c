#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"





//
//
//
uint16_t Game4(void) {
    uint8_t     pile[3];
    uint8_t     oldPile;
    uint8_t     i;
    uint8_t     flashcnt;
    uint8_t     sp;
    uint8_t     wfr;
    uint8_t     mode;
    uint8_t     sum;

    for (i=0; i<3; i++) {
        pile[i]=4+(rand()%6);
    }

    sp=2;
    flashcnt=0;
    wfr=0;
    mode=0;
    oldPile=pile[sp];
    for (;;) {
        Delay10mS(5);
        // Display teh values of the piles
        disp[0]=charmap[pile[0]+16];
        disp[1]=charmap[pile[1]+16];
        disp[2]=charmap[pile[2]+16];
        // flash the current selected pile. Slow if just going left/right, fast when inc/decrementing the pile
        if (flashcnt++&(0x01<<(1-mode))) disp[sp]=0;

        ReadButtons();
        if (!wfr) { // Don't handle the buttons if the WaitForRelease flag is set
            // Top left button in pile selection mode. GO LEFT.
            if (buttons==1 && mode==0 && sp>0) {sp--; oldPile=pile[sp]; wfr=1;}
            // Bottom left button in pile selection mode. GO RIGHT
            if (buttons==2 && mode==0 && sp<2) {sp++; oldPile=pile[sp]; wfr=1;}

            // Top left button in inc/dec mode. ACCEPT
            if (buttons==1 && mode==1) {wfr=1; if (pile[sp]!=oldPile) mode=2; else mode=0;}
            // Bottom left button in inc/dec mode. CANCEL
            if (buttons==2 && mode==1) {pile[sp]=oldPile; mode=0; wfr=1;}

            // Top right button. INCREMENT VALUE
            if (buttons==4 && pile[sp]<oldPile) {mode=1; pile[sp]++; wfr=1;}
            // Bottom right button. DECREMENT VALUE
            if (buttons==8 && pile[sp]>0) {mode=1; pile[sp]--; wfr=1;}
        }
        if (buttons==0) wfr=0;  // In no button if pressed then clear WaitForRelease flag

        if (mode==2) {  // User accepted his turn, now let the computer play
            disp[sp]=charmap[pile[sp]+16];
            while(ReadButtons());   // Wait for button release before doing anything
            for (i=0; i<5; i++) {   // Signal computer thinking by movin the dot back and forth
                disp[2]^=0x80; Delay10mS(20); disp[2]^=0x80;
                disp[1]^=0x80; Delay10mS(20); disp[1]^=0x80;
                disp[0]^=0x80; Delay10mS(20); disp[0]^=0x80;
                disp[1]^=0x80; Delay10mS(20); disp[1]^=0x80;
            }
            disp[2]^=0x80; Delay10mS(20); disp[2]^=0x80;
            // Calculate the binary sum of all pils
            sum=pile[0]^pile[1]^pile[2];

            for (i=0; i<8; i++) {
                if (testbit(sum,i)) {
                    if (testbit(pile[0],i)) {pile[0]-=(1<<i); mode=0; break; }
                    if (testbit(pile[1],i)) {pile[1]-=(1<<i); mode=0; break; }
                    if (testbit(pile[2],i)) {pile[2]-=(1<<i); mode=0; break; }
                }
            }
            if (mode!=0) {
                mode=0;
                if (pile[0]>0) {pile[0]-=(1+(rand()%pile[0]));}
                else if (pile[1]>0) {pile[1]-=(1+(rand()%pile[1]));}
                else if (pile[2]>0) {pile[2]-=(1+(rand()%pile[2]));}
            }
            oldPile=pile[sp];
        }
    }
    
    return 0;
}




