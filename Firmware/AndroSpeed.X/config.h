#ifndef CONFIG_H
#define	CONFIG_H

#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions
#include <xc.h>

#ifdef _PIC16F1829
// PIC16F1829 Configuration Bit Settings
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
#endif


// PIC16LF1828 Configuration Bit Settings
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)




#define _XTAL_FREQ 16000000

#define CCLED   LATAbits.LATA1  // Ports for the CommonCathode drivers
#define CCD1    LATBbits.LATB6
#define CCD2    LATAbits.LATA5
#define CCD3    LATBbits.LATB7

#define SEGA    LATCbits.LATC3  // Ports for the 7-seg Anodes
#define SEGB    LATCbits.LATC4
#define SEGC    LATCbits.LATC6
#define SEGD    LATCbits.LATC0
#define SEGE    LATCbits.LATC1
#define SEGF    LATCbits.LATC2
#define SEGG    LATCbits.LATC7
#define SEGP    LATCbits.LATC5

#define LED1    SEGG            // Ports for the LED Anodes
#define LED2    SEGF
#define LED3    SEGA
#define LED4    SEGB

#define BUTTON1CPS  3       // CPS channels for the buttons
#define BUTTON2CPS  2
#define BUTTON3CPS  11
#define BUTTON4CPS  10

#define BUTTON1TRIS TRISA4  //  TRIS & ANSEL definitions for the buttons
#define BUTTON1ANSEL ANSA4
#define BUTTON2TRIS TRISA2
#define BUTTON2ANSEL ANSA2
#define BUTTON3TRIS TRISB5
#define BUTTON3ANSEL ANSB5
#define BUTTON4TRIS TRISB4
#define BUTTON4ANSEL ANSB4

#define VERSTRING   "R01"

#endif	/* CONFIG_H */

