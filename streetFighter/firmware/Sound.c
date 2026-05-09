// Sound.c
// Runs on MSPM0
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

#define BUS_CLOCK 40000000
#define SAMPLE_FREQ 11025

// Globals
static const uint8_t *SoundPt;
static uint32_t SoundCount;
static uint32_t SoundIndex;


void SysTick_IntArm(uint32_t period, uint32_t priority){
    SysTick->CTRL = 0;                // disable during setup
    SysTick->LOAD = period - 1;
    SysTick->VAL  = 0;
    SCB->SHP[11]  = priority << 5;    // set priority (bits 7:5)
    SysTick->CTRL = 0x07;             // enable, core clock, interrupt
}

void Sound_Init(void){
    SoundPt    = 0;
    SoundCount = 0;
    SoundIndex = 0;
    DAC5_Init();
    SysTick_IntArm(BUS_CLOCK / SAMPLE_FREQ, 2);
    // Start with interrupt disabled until Sound_Start is called
    SysTick->CTRL &= ~0x02;           // disable SysTick interrupt
}

void SysTick_Handler(void){
    if(SoundCount == 0){
        SysTick->CTRL &= ~0x02;       // no active sound, disable interrupt
        return;
    }
    DAC5_Out(SoundPt[SoundIndex]);
    SoundIndex++;
    if(SoundIndex >= SoundCount){
        SoundIndex = 0;
        SoundCount = 0;               // one-shot: stop after playing once
        SysTick->CTRL &= ~0x02;
    }
}

void Sound_Start(const uint8_t *pt, uint32_t count){
    SoundPt    = pt;
    SoundCount = count;
    SoundIndex = 0;
    SysTick->CTRL |= 0x02;           // enable SysTick interrupt
}

void Sound_Shoot(void){ //hit
    Sound_Start(shoot, 4080);
}

void Sound_Killed(void){
    Sound_Start(invaderkilled, sizeof(invaderkilled));
}

void Sound_Explosion(void){ //block
    Sound_Start(explosion, sizeof(explosion));
}

void Sound_Fastinvader1(void){

}
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){

}

