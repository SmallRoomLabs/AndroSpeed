#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>

#include "config.h"
#include "DispTouch.h"
#include "utils.h"


/*
 * RA0  PGD                     RC0  SEG-D
 * RA1  CC-LED / PCG            RC1  SEG-P
 * RA2  BUTTON 2 (CPS2)         RC2  SEG-E
 * RA3  MCLR                    RC3  SEG-A
 * RA4  BUTTON 1 (CPS3)         RC4  SEG-B
 * RA5  CC-2                    RC5  SEG-G
 *                              RC6  SEG-C
 * RB4  BUTTON 3 (CPS10)        RC7  SEG-F
 * RB5  BUTTON 4 (CPS11)
 * RB6  CC-1
 * RB7  CC-3
 */


uint8_t             buttons;    // Bitmap of the button state
volatile uint8_t    leds;       // Bitmap state of the LEDs
volatile uint8_t    disp[3];    // Bitmaps of the 7-segment displays
uint16_t            base[4];    // Base value for an untouched button
uint16_t            limit[4];   // Max value for an untouched button (noise max)
volatile uint16_t   cps;        // The read value from the CPS



//
// Refresh the displays and leds.
// It is called from the Timer 2 interrupt handler
//
void UpdateDisplay(void) {
    static uint8_t cathode=0;
    uint8_t v;

    if (cathode<3) {    // cathode 0..2 is the 7-seg displays
        CCLED=0;        // Turn off all cathodes
        CCD1=0;
        CCD2=0;
        CCD3=0;
        v=disp[cathode];    // Fetch the segments to be lit
        LATC=0x00;          // And set the correspoding bits in the port
        if (v&0x01) SEGA=1;
        if (v&0x02) SEGB=1;
        if (v&0x04) SEGC=1;
        if (v&0x08) SEGD=1;
        if (v&0x10) SEGE=1;
        if (v&0x20) SEGF=1;
        if (v&0x40) SEGG=1;
        if (v&0x80) SEGP=1;

        if (cathode==0) CCD1=1; // Turn on the right Cathode driver
        if (cathode==1) CCD2=1;
        if (cathode==2) CCD3=1;
    } else {
        CCD3=0;             // Do the LEDs, so turn off the last 7-seg display
        LATC=0x00;          // And set the corresponding LEDs
        if (leds&1) LED1=1;
        if (leds&2) LED2=1;
        if (leds&4) LED3=1;
        if (leds&8) LED4=1;
        CCLED=1;            // Turn on the LED Cathode driver
    }

    cathode++;              // Step to next cathode
    cathode&=0x03;
}










/******************************************************************************
*     Stable CSM Captures using Timer 1 Gate in Single-Pulse Toggle Mode
*******************************************************************************
*  This system uses the hardware to both start and stop the capture of a
* reading. In order to flag the system to take a new reading, set the T1GGO
* bit. Once this is done, the next TMR0 overflow will start the reading and
* the one after that will stop the reading. You then have as much time as you
* need to grab the reading and flag the system to take another.
*  NOTE: It is highly recommended you follow this method instead of attempting
* to use something "similar" to this. This method can have problems if the
* interrupts and flags are not managed correctly.
******************************************************************************/
void CPSinit(void) {
    CPSCON0 = 0b10001101;
//              1------- CPSON: CPS module is enabled
//              -0------ CPSRM: CPS module is in the low range. Internal oscillator voltage references are used.
//              ----11-- CPSRNG: Oscillator is in High Range. Charge/Discharge Current is nominally 18 ?A
//              -------1 T0XCS: Timer0 clock source is the capacitive sensing oscillator
    CPSCON1 = 0;

    TMR0CS=1;   // Timer0 Clock Source = Transition on T0CKI pin

    PSA=1;      // Prescaler assigned to WDT
    T0IF        = 0;
    T0IE        = 1;

    T1CON       = 0b01000001;       // TMR1CS:  Fosc
                                    // T1CKPS:  1:1 Prescaler
                                    // T1OSCEN: Dedicated TMR1 oscillator disabled
                                    // TMR1ON:  On
    T1CKPS0=1;
    T1CKPS1=1;

    TMR1IF      = 0;
    TMR1IE      = 1;

    T1GCON      = 0b11110001;       // TMR1GE:  Enabled
                                    // T1GPOL:  Active-High
                                    // T1GTM:   Toggle Mode Enabled
                                    // T1GSPM:  Single-Pulse Mode Enabled
                                    // T1GSS:   Timer0
    TMR1GIF     = 0;
    TMR1GIE     = 1;

//    PEIE        = 1;
}





void CPSstart(uint8_t channel) {
    CPSCON1=channel;

    TMR1ON      = 0;                // Turn off TMR1 before writing to registers
    TMR1H       = 0;
    TMR1L       = 0;
    TMR1ON      = 1;                // Turn TMR1 back on

    TMR0        = 0xFE;             // Force TMR0 close to rollover to save time.
    T1GGO       = 1;                // Flag system to take the next reading
}





//
// Read all four buttons using the CPS module.
// Store the states in the global variable buttons
//
uint8_t ReadButtons(void) {
    uint8_t v;
    v=0;

    cps=0xff;
    CPSstart(BUTTON1CPS);
    while (cps==0xff);
    if (cps>limit[0]) v|=0x01;

    cps=0xff;
    CPSstart(BUTTON2CPS);
    while (cps==0xff);
    if (cps>limit[1]) v|=0x02;

    cps=0xff;
    CPSstart(BUTTON3CPS);
    while (cps==0xff);
    if (cps>limit[2]) v|=0x04;

    cps=0xff;
    CPSstart(BUTTON4CPS);
    while (cps==0xff);
    if (cps>limit[3]) v|=0x08;

    buttons=v;
    return v;
}


//
// Gets base readings for the untouched pads.
// It also seeds the random() function
//
void CalibrateTouch(void) {
    uint8_t i,j;
    uint32_t a;
    uint32_t rnd;

    rnd=0;

    DispMsg("CAL",200);

    for (j=0; j<4; j++) {
        a=0;
        limit[j]=0;
        for (i=0; i<64; i++) {
            if (j==0) CPSstart(BUTTON1CPS);
            else if (j==1) CPSstart(BUTTON2CPS);
            else if (j==2) CPSstart(BUTTON3CPS);
            else CPSstart(BUTTON4CPS);
            __delay_ms(5);
            a+=cps;
            rnd+=cps;   // Build a random value of all readings
            if (cps>limit[j]) limit[j]=cps;
        }
        base[j]=a/64;
        limit[j]+=((limit[j]-base[j])/2); //Add 50% on to of max read value as the value for touched button
    }
    DispMsg("   ",50);
    srand(rnd);
}


