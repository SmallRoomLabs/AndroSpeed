#ifndef DISPTOUCH_H
#define	DISPTOUCH_H

#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions
#include <xc.h>

extern uint8_t             buttons;    // Bitmap of the button state
extern volatile uint8_t    leds;       // Bitmap state of the LEDs
extern volatile uint8_t    disp[3];    // Bitmaps of the 7-segment displays
extern uint16_t            base[4];    // Base value for an untouched button
extern uint16_t            limit[4];   // Max value for an untouched button (noise max)
extern volatile uint16_t   cps;         // The read value from the CPS
//extern uint16_t            pressDelta; //


void UpdateDisplay(void);
void CPSinit(void);
void CalibrateTouch(void);
uint8_t ReadButtons(void);


#endif	/* DISPTOUCH_H */

