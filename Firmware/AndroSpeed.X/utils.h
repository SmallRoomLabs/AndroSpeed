#ifndef UTILS_H
#define	UTILS_H

#include <stdint.h>         // uint8_t definitions

#include <xc.h>
#include "config.h"

#define  testbit(var, bit)   ((var) & (1UL <<(bit)))
#define  setbit(var, bit)    ((var) |= (1UL << (bit)))
#define  clrbit(var, bit)    ((var) &= ~(1UL << (bit)))


extern const uint8_t charmap[];

void DispValue(uint32_t value);
void Delay10mS(uint8_t cnt);
void DispMsg(const uint8_t msg[], uint8_t dly10ms);


#endif	/* UTILS_H */

