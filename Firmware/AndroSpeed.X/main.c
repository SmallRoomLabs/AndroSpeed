#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions
#include <stdio.h>

#include <xc.h>
#include "config.h"
#include "DispTouch.h"
#include "utils.h"
#include "game1.h"
#include "game2.h"
#include "game3.h"
#include "game4.h"

volatile uint16_t tick;

//
// Initialize oscillator and the ports
//
void SetupBoard() {
    OSCCON=0b01111010;              // PLL off, 16MHz, IntOsc

    ANSELA=0b00000000;              // Most pins are digital
    ANSELB=0b00000000;  
    ANSELC=0b00000000;  
    BUTTON1ANSEL=1;                 // Except the buttons
    BUTTON2ANSEL=1;
    BUTTON3ANSEL=1;
    BUTTON4ANSEL=1;

    TRISA=0x00;                     // Most pins are Outputs
    TRISB=0x00;
    TRISC=0x00;
    BUTTON1TRIS=1;                  // Except the buttons
    BUTTON2TRIS=1;
    BUTTON3TRIS=1;
    BUTTON4TRIS=1;

    LATA=0x00;                      // Set all pins low
    LATB=0x00;
    LATC=0x00;
}


//
// Handle Timer 0, 1 and 2 overflows
//
void interrupt TimerOverflow() {
    if (TMR1GIE && TMR1GIF) {       // Timer 1 signals that the CPS reading is done
        TMR1GIF = 0;
        TMR0=0;
        cps=(TMR1H << 8)+TMR1L;
        cps=cps<<4;
    }

    if (T0IE && T0IF) {             // Timer 0 is also for CPS
        T0IF = 0;
    }

    if(TMR2IF==1) {                 // Timer2 is for refreshing the displays & leds
        TMR2IF=0;
        tick++;
        UpdateDisplay();
    }
}




void main(void) {
    uint8_t     i;
    uint16_t    cnt;
    uint8_t     lastButtons;
    uint16_t    score;

    SetupBoard();
    CPSinit();

    // Timer2 is for Display Refresh
    tick=0;
    T2CON=0b00000111;       // 1:1 postscaler, timer on, 1:64 prescaler
    TMR2IE=1;               // Enable timer 2 interrupts
    TMR2IF=1;               // Clear pending timer 2 interrupt

    GIE=1;                  // Globally enable interrupts
    PEIE=1;
    ei();

    DispMsg("   ",50);      // Show current firmware release
    DispMsg(VERSTRING,30);
    DispMsg("   ",50);

    CalibrateTouch();

    score=0;
    cnt=0;
    for (;;) {
        ReadButtons();
        if (cnt==0) {                               // Waiting for game selection
            DispMsg("   ",20);
            DispMsg(" ? ",2);
        } else if (cnt<12) {                        // Button pressed (but not too long)
            switch (buttons) {
                case 1:DispMsg("HIT",12); break;
                case 2:DispMsg("SPD",12); break;
                case 4:DispMsg("REP",12); break;
                case 8:DispMsg("AST",12); break;
            }
            if ((buttons==0) && (lastButtons>0)) {  // Button just released
                switch (lastButtons) {
                    case 1:score=Game1(); break;
                    case 2:score=Game2(); break;
                    case 4:score=Game3(); break;
                    case 8:score=Game4(); break;
                }
                lastButtons=0;
                cnt=0;
                leds=0;
                for (i=0; i<12; i++) {              // Flash the final score for a while
                    DispMsg("   ",30);
                    DispValue(score);
                    Delay10mS(30);
                }
                DispMsg("   ",100);
                continue;
            }
            lastButtons=buttons;
        } else {
            DispMsg("   ",12);
        }
        cnt++;
        if (buttons==0) cnt=0;
    }

}




