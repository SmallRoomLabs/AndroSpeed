#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"

#define TOP     0x01
#define MID     0x40
#define BOTTOM  0x08

#define MAXSPEED    40
#define MAXDENSITY  120
#define MAXHITS     3


extern volatile uint16_t tick;

//
//
//
uint16_t Game4(void) {
    uint16_t    score=0;
    uint8_t     ship;                   // Current ship position 0..2
    uint8_t     astTop,astMid,astBot;   // bitmaps of asteroids
    uint8_t     speed;                  // Game speed
    uint8_t     density;                // Density of astroids 0=few 255=jam packed
    uint8_t     wfr;                    // 1=wait for button release
    uint8_t     r;
    uint8_t     cnt;
    uint8_t     hits;

    cnt=0;
    ship=1;
    wfr=0;
    density=50;
    speed=60;
    astTop=0;
    astMid=0;
    astBot=0;
    hits=0;

    for (;;) {
        disp[0]=disp[1]=disp[2]=0;
        if (astTop&0x01) disp[0]|=TOP;
        if (astTop&0x02) disp[1]|=TOP;
        if (astTop&0x04) disp[2]|=TOP;
        if (astMid&0x01) disp[0]|=MID;
        if (astMid&0x02) disp[1]|=MID;
        if (astMid&0x04) disp[2]|=MID;
        if (astBot&0x01) disp[0]|=BOTTOM;
        if (astBot&0x02) disp[1]|=BOTTOM;
        if (astBot&0x04) disp[2]|=BOTTOM;

        if (tick & 0x10) disp[2-ship]|=BOTTOM;
        
        ReadButtons();              // Read buttons and move ship
         if (!wfr) {
            if (buttons) wfr=1;
            if (buttons&0x02) {
                if (ship>0) ship--;
            }
            if (buttons&0x08) {
                if (ship<2) ship++;
            }
            if ( ((ship==0) && (astBot&0x04)) || ((ship==1) && (astBot&0x02)) || ((ship==2) && (astBot&0x01)) ) {
                for (r=0; r<15; r++) {
                    DispMsg("HIT",3);
                    DispMsg("   ",3);
                }
                astBot=astMid=astTop=0;
                hits++;
                if (hits>=MAXHITS) return score/10;
                continue;
            }
        }
        if (!buttons) wfr=0;

        cnt++;
        if (cnt>speed) {
            score++;
            if ((score%10)==0) {    // Make game harder
                if (rand()&1) {
                    if (speed>MAXSPEED) speed--;
                } else {
                    if (density<MAXDENSITY) density++; 
                }
                
            }
            cnt=0;
            astBot=astMid;
            astMid=astTop;
            astTop=0;
            r=rand()&0xff;
            if (r<density) setbit(astTop,1);
            r=rand()&0xff;
            if (r<density) setbit(astTop,2);
            r=rand()&0xff;
            if (r<density) setbit(astTop,3);
            if (astTop==0x07) {
                r=rand()%3;
                clrbit(astTop,r);
            }
            cnt=0;
        }
        __delay_ms(5);
    }

}


//
//
//
uint16_t XGame4(void) { // Unfinished NIM game - 3661 bytes
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
        // Display the values of the piles
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




