#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"

const uint16_t labyrinth[] = {
    0b1111111111111110,
    0b1000000000000010,
    0b1011111111101010,
    0b1010001000001010,
    0b1010101011111110,
    0b1000101000000010,
    0b1111101111111010,
    0b1000100010001010,
    0b1110111010101010,
    0b1010001000101010,
    0b1011101111101010,
    0b1000101000101010,
    0b1011101010111010,
    0b1000000010000000,
    0b1111111111111110
};


const  uint8_t pixmap[]={0x01,0x40,0x08};

void plot(uint8_t x, uint8_t y) {
    disp[2-x]|=pixmap[y];
}

//
//
//
uint16_t Game4(void) {
    uint16_t    score;
    uint8_t     i;
    int8_t      myx,myy;
    int8_t      x,y;
    uint8_t     color;
    uint8_t     flashcnt;
    uint8_t      redraw;

    score=0;
    redraw=1;

    DispMsg("   ",1);
    myy=1;    // Start position
    myx=1;
    flashcnt=0;
    for (;;) {
        Delay10mS(5);
        if (redraw) {
            DispMsg("   ",1);
            for (y=-1; y<2; y++) {
                for (x=-1; x<2; x++) {
                    color=1;
                    if (myy+y>=0) {
                        if (myx+x>=0) {
                            if (!testbit(labyrinth[myy+y],15-(myx+x))) color=0;
                        }
                    }
                    if (color) plot(x+1,y+1);
                }
            }
        }
        if (redraw) {
            redraw=0;
            while (ReadButtons()); // Wait for release
        }
        ReadButtons();
        if (buttons==1 && myy>0) {myy--; redraw=1;}
        if (buttons==3 && myx>0) {myx--; redraw=1;}
        if (buttons==2 && myy<15) {myy++; redraw=1;}
        if (buttons==4 && myx<15) {myx++; redraw=1;}
        if ((flashcnt++)&0x01) disp[1]^=0x40;
    }


    return score;
}




