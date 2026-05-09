/*
 * LED.c
 *
 *  Created on: Nov 5, 2023
 *
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "LED.h"
// LaunchPad.h defines all the indices into the PINCM table

#define PA8INDEX   54
#define PA15INDEX  36
#define PA16INDEX  37
#define PA17INDEX  38

// initialize your LEDs
void LED_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000081;
  IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00000081;
  IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00000081;

  GPIOA->DOE31_0 |= (LED_PUNCH|LED_KICK|LED_BLOCK|LED_PAUSE);
  GPIOA->DOUTCLR31_0 = (LED_PUNCH|LED_KICK|LED_BLOCK|LED_PAUSE);
    
}
// data specifies which LED to turn on
void LED_On(uint32_t data){
    // write this
    // use DOUTSET31_0 register so it does not interfere with other GPIO
  GPIOA->DOUTSET31_0 = data;
}

// data specifies which LED to turn off
void LED_Off(uint32_t data){
    // write this
    // use DOUTCLR31_0 register so it does not interfere with other GPIO
  GPIOA->DOUTCLR31_0 = data;
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    // write this
    // use DOUTTGL31_0 register so it does not interfere with other GPIO
  GPIOA->DOUTTGL31_0 = data;
}