/*
 * Switch.c
 *
 *  Created on: January 12, 2026
 *
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PA25INDEX] = 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PA26INDEX] = 0x00050081; // input, pull down
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00050081; // input, pull down
  GPIOA->DOE31_0 &= ~((uint32_t)(1<<24)|(1<<25)|(1<<26)|(1<<27));
}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  uint32_t data = 0;
  if(GPIOA->DIN31_0 & (1<<24)){ // right
    data |= 0x01; // punch
  }
  if(GPIOA->DIN31_0 & (1<<25)){ // bottom
    data |= 0x02; // kick
  }
  if(GPIOA->DIN31_0 & (1<<26)){ // left
    data |= 0x04; // block
  }
  if(GPIOA->DIN31_0 & (1<<27)){ // top
    data |= 0x08; // pause
  }
  return data; // replace this line
}