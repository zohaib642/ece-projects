/*
 * LED.h
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>

#define LED_PUNCH  0x20000   // PA17
#define LED_KICK   0x10000   // PA16
#define LED_BLOCK  0x8000    // PA15
#define LED_PAUSE  0x0100    // PA8

// initialize your LEDs
void LED_Init(void);

// data specifies which LED to turn on
void LED_On(uint32_t data);

// data specifies which LED to turn off
void LED_Off(uint32_t data);

// data specifies which LED to toggle
void LED_Toggle(uint32_t data);

#endif /* LED_H_ */