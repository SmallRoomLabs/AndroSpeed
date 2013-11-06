#include <stdlib.h>
#include <stdint.h>         // uint8_t definitions
#include <stdbool.h>        // true/false definitions

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "DispTouch.h"


uint8_t const charmap[]={
     0x00, 0x86, 0x22, 0x08, 0x08, 0x08, 0x08, 0x02, /*   ! " # $ % & '  */
     0x39, 0x0f, 0x08, 0x08, 0x04, 0x40, 0x80, 0x52, /* ( ) * + , - . /  */
     0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, /* 0 1 2 3 4 5 6 7  */
     0x7F, 0x6F, 0x04, 0x04, 0x39, 0x48, 0x0f, 0x53, /* 8 9 : ; < = > ?  */
     0x04, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x3d, /* @ A B C D E F G  */
     0x76, 0x10, 0x1E, 0x76, 0x38, 0x37, 0x54, 0x5c, /* H I J K L M N O  */
     0x73, 0x67, 0x50, 0x6d, 0x78, 0x3e, 0x1C, 0x2A, /* P Q R S T U V W  */
     0x76, 0x6e, 0x5b, 0x39, 0x64, 0x0F, 0x01, 0x08  /* X Y Z [ \ ] ^ _  */
};


//
// Displays values from 0 to 999.
// If higher then display three triple-bars.
//
void DispValue(uint32_t value) {
    uint32_t t;

    t=value;
    if (t<1000) {
        disp[0]=charmap[16+(t%10)];
        t=t/10;
        disp[1]=charmap[16+(t%10)];
        t=t/10;
        disp[2]=charmap[16+(t%10)];
    } else {
        disp[0]=0x49;   // Three bars to signal overflow
        disp[1]=0x49;   // Three bars to signal overflow
        disp[2]=0x49;   // Three bars to signal overflow
    }
}



//
// Display 3-character string
//
void DispMsg(const uint8_t msg[], uint8_t dly10ms) {
    disp[2]=charmap[msg[0]-32];
    disp[1]=charmap[msg[1]-32];
    disp[0]=charmap[msg[2]-32];
    Delay10mS(dly10ms);
}



//
// Waits the specified number of 10mS units
//
void Delay10mS(uint8_t cnt) {
    while (cnt--) __delay_ms(10);
}


