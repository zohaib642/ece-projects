// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
//
// Last Modified: January 12, 2026

#define ST7735_TEAL 0xB7FF
#define ST7735_BG ST7735_WHITE
#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "../inc/Arabic.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
extern const unsigned short sun[];

// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

Arabic_t ArabicAlphabet[]={
alif,ayh,baa,daad,daal,dhaa,dhaal,faa,ghayh,haa,ha,jeem,kaaf,khaa,laam,meem,noon,qaaf,raa,saad,seen,sheen,ta,thaa,twe,waaw,yaa,zaa,space,dot,null
};
Arabic_t Hello[]={alif,baa,ha,raa,meem,null}; // hello
Arabic_t WeAreHonoredByYourPresence[]={alif,noon,waaw,ta,faa,raa,sheen,null}; // we are honored by your presence

int main0(void){ // main 0, demonstrate Arabic output
  Clock_Init80MHz(0);
  LaunchPad_Init();
  ST7735_InitR(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_WHITE);
  Arabic_SetCursor(0,15);
  Arabic_OutString(Hello);
  Arabic_SetCursor(0,31);
  Arabic_OutString(WeAreHonoredByYourPresence);
  Arabic_SetCursor(0,63);
  Arabic_OutString(ArabicAlphabet);
  while(1){
  }
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {CTRL_SLIDEPOT, CTRL_JOYSTICK} Control_t;
Control_t myControl=CTRL_SLIDEPOT;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

/********************* game constants **************************/
#define BTN_PUNCH 0x01
#define BTN_KICK  0x02
#define BTN_BLOCK 0x04
#define BTN_PAUSE 0x08

#define PLAYER_IDLE_W    20
#define PLAYER_IDLE_H    28
#define PLAYER_ATTACK_W  20
#define PLAYER_ATTACK_H  21
#define ENEMY_IDLE_W     20
#define ENEMY_IDLE_H     19
#define ENEMY_ATTACK_W   20
#define ENEMY_ATTACK_H   17
#define FX_W     10
#define FX_H     11

#define PLAYER_W         PLAYER_IDLE_W
#define PLAYER_H         PLAYER_IDLE_H
#define ENEMY_W          ENEMY_IDLE_W
#define ENEMY_H          ENEMY_IDLE_H

#define GROUND_Y           150
#define PLAYER_MIN_X       8
#define PLAYER_MAX_X       (128-PLAYER_IDLE_W)
#define ENEMY_MIN_X        0
#define ENEMY_MAX_X        (128-ENEMY_IDLE_W)
#define MAX_HP             10
#define PLAYER_PUNCH_TIME  5
#define PLAYER_KICK_TIME   8
#define ENEMY_ATTACK_TIME  7
#define ATTACK_RANGE       26

/********************* local sprites **************************/
const unsigned short PlayerIdle[] = {
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB5B6, 0x8452, 0xCE59,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF5D, 0xC659, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xF79E, 0x0000, 0x0083, 0xC639, 0xFFFF, 0xFFFF, 0xFFFF, 0xDEBA, 0x738D, 0x1926, 0x4A8B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0x28E2, 0x5268, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4185, 0x3123, 0x632C, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xAD33, 0x49A5, 0xB595, 0xFFFF, 0xFFFF, 0xFFFF, 0x6B2B,
 0x41A5, 0xAD34, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4185, 0x62A9,
 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF, 0x4185, 0x83CE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xF7BE, 0x51E6, 0x7BAD, 0xEF7D, 0xFFFF, 0xFFFF, 0x51E5, 0x62A9, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC617, 0x6288, 0x83EF, 0xF79E, 0xFFFF, 0xE73C, 0x62A9, 0xB575, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x6288, 0x6B2B, 0xCE58, 0xFFFF,
 0xEF3C, 0x6B0B, 0x6B0B, 0xE71C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE71B,
 0x72EA, 0x7B8D, 0xD6BA, 0xC638, 0x734C, 0x736C, 0x62CA, 0xD699, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0x6AEA, 0x83AD, 0x83CF, 0x7BAE, 0x62A9, 0x732B, 0x7B4B, 0x7B8D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5A47, 0x8C30, 0x7BAE, 0x5A68, 0x734B, 0x7B8D, 0x62CA, 0xBDD6, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5A68, 0x8C30, 0x736D, 0x5A68, 0x730A,
 0x732B, 0x83EF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0x62C9,
 0x7B6C, 0x7B4B, 0x7B6C, 0x62A9, 0x62CA, 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x6BB0, 0x31EA, 0x4A49, 0x4A49, 0x4A69, 0x2986, 0xCE9A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x2145, 0x0881, 0x18C2, 0x10A2, 0x10A2, 0x3A07, 0xFFFF, 0xA555, 0x2965, 0xB5D6,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x2985, 0x10C3, 0x10C2, 0x10C2, 0x08A2, 0x31C6,
 0x7412, 0x0907, 0x0020, 0x3A28, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5B0B, 0x1148,
 0x1946, 0x10C2, 0x10C2, 0x10A1, 0x0083, 0x11CC, 0x10A2, 0x2964, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xBE18, 0x0928, 0x19ED, 0x1106, 0x10C2, 0x10C2, 0x2124, 0x1926, 0x2145, 0x2124, 0xD6BA, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD6DA, 0x08A2, 0x1946, 0x10E4, 0x0881, 0x2166, 0x3A4C, 0x2943, 0xB5F9, 0xF7DF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0020, 0x18A1, 0x1081, 0x29A7, 0x1947,
 0x29A9, 0x9CF3, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xAD55, 0x632C, 0x3A2C, 0x4AEF, 0x8CF7, 0xC659, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0x8D16, 0x9D97, 0xADB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,

};

const unsigned short PlayerAttack[] = {
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF79E, 0x9CF4, 0xCE79, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF5D,
 0xC618, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4A48, 0x0000, 0xB5D7, 0xFFFF,
 0xFFFF, 0xFFFF, 0x83EF, 0x2146, 0x6BB0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xBDD7, 0x41A5, 0xDEFB, 0xFFFF, 0xFFFF, 0xFFDF, 0x3102, 0x62CA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x49C6, 0x83AD, 0xFFFF, 0xFFFF, 0xFFFF, 0x6B0B, 0x6B0B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0x49A5, 0xAD34, 0xFFFF, 0xFFFF, 0xE6FB, 0x5207,
 0xE71C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDEBA, 0x6288,
 0xAD34, 0xFFFF, 0xFFFF, 0x5A48, 0xAD33, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0x6288, 0x7BAE, 0xFFFF, 0xFFDE, 0x6B0B, 0x6B0B, 0xE73C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF3C, 0x732B, 0x7BCF, 0xCE38, 0x83AD, 0x732B, 0x6B0B, 0xF77D, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xAD34, 0x83CE, 0x7B8E, 0x62C9, 0x730B, 0x732C,
 0x8C0F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x9471, 0x8C30,
 0x6B2C, 0x5A88, 0x736C, 0x6B0B, 0xF77D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x732C, 0x7B8D, 0x7B4C, 0x732B, 0x5A68, 0xCE58, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7BF, 0x2167, 0x3A09, 0x3A09, 0x31A6, 0x7BCF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0x0000, 0x10A2, 0x10C2, 0x0040, 0x94F3, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF79E, 0x0083, 0x10C2,
 0x10C2, 0x10C2, 0x1924, 0xD6BA, 0xF7BE, 0xD6BA, 0xDEFB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFDF, 0x0129, 0x1169, 0x10C2, 0x1903, 0x2146, 0x2123, 0x2A0B, 0x00E7, 0x0861, 0x5AEB, 0xD6BA, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0x0062, 0x1946, 0x10A2, 0x2166, 0x3A4C, 0x0840, 0x00C6, 0x7433, 0xDEDB, 0x4A69,
 0x6350, 0xF79F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4A69, 0x0860, 0x10A2, 0x31EA, 0x322C, 0x5B0D,
 0xCE99, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDEDB,
 0xBE18, 0x530E, 0x9558, 0xA5B8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE75D, 0xE75D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

const unsigned short EnemyIdle[] = {
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE73C, 0x8C6D, 0xB5B4,
 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xA4EF, 0x6B09, 0x6AED, 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0x7B89, 0xA514, 0x49EA, 0xDEDA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7BAE, 0x62A8, 0x734C, 0x8C0F, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF77D, 0x3165, 0x5229, 0x730B,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xF7BE, 0x49E7, 0x4A07, 0xCE38, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x836D, 0x4A27, 0x4A49, 0xCE59, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x6ACA, 0x49E6, 0x7B8D, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xE71C, 0x5A27,
 0xA4D2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x5A8A, 0x18A3, 0x7BD0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xB595, 0x8C71, 0x9CD2, 0xCE18, 0xDEDB, 0xB594, 0x7388, 0x4A05, 0x6B0B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xF79E, 0x6B2C, 0x1043, 0x3966, 0x8BCF, 0xA450, 0x946C, 0xA54C, 0x83A9, 0x62C9, 0xF79E, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0xEF3D, 0x9432, 0xC5F7, 0xF79E, 0xAD33, 0x5A87, 0x83CB,
 0xB551, 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0x9CB0, 0x6B07, 0x5AA8, 0x7B8E, 0xF79E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD678, 0x83ED, 0x840A, 0x4A29, 0x20A7, 0x8C30, 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDE, 0xFFFF, 0xDEDA, 0x7B6D, 0xCE39, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

const unsigned short EnemyAttack[] = {
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7B8E, 0x83CD,
 0xF77D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0x8C31, 0x4A07, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x9CD2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF79E, 0x2923, 0xAD33, 0xFFFF, 0xFFFF, 0xFFFF, 0x9CB4, 0x8BF1, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5AAA, 0x6B0C, 0xFFBF, 0xFFFF, 0xFFFF, 0xE71C, 0x5AA6,
 0xEF3C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x41A5, 0x630C, 0xD6BA,
 0xFFFF, 0xF7BE, 0x4A28, 0x8C2F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xD699, 0x49E7, 0x5269, 0xAD34, 0x62CB, 0x5ACA, 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xBD95, 0x41E6, 0x5227, 0x6248, 0xA4D2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xAD33, 0x62A8, 0x8BAD, 0xFFDF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x736F,
 0x2925, 0x9CB1, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDEB9, 0xEF3C, 0xD679,
 0xC5F6, 0xDEFB, 0xB595, 0x3144, 0x20E3, 0x7BCF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xB532, 0x39A9, 0x3986, 0x39A6, 0x5A8A, 0x7B2C, 0x732C, 0x5A86, 0xBDF5, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF79E, 0x520A, 0x5A6E, 0x5A8D, 0x526B, 0x6AEA, 0x730B, 0x9CAF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0x83F1, 0x732C, 0x7368, 0x41E5,
 0xDEDA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xBDB3, 0x7BA9, 0xF79E, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

const unsigned short HitFX[] = {
 // row 10 (bottom) - empty
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 9 - diagonal tips
 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 8 - inner diagonals
 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 7 - vertical spike pixel
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 6 - lower arm
 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF,
 // row 5 - center horizontal
 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF,
 // row 4 - upper arm
 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0x0000, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF,
 // row 3 - vertical spike pixel
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 2 - inner diagonals
 0xFFFF, 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 1 - diagonal tips
 0xFFFF, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0x0000, 0xFFFF, 0xFFFF, 0xFFFF,
 // row 0 (top) - empty
 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};

/********************* game state **************************/
typedef struct{
  int32_t x;
  int32_t y;
  int32_t hp;
  uint32_t attackTimer;
  uint32_t kickAttack;
  uint32_t hitDone;
  uint32_t facingRight;
  uint32_t blocking;
  uint32_t jumpTimer;
} Fighter_t;

volatile Fighter_t Player;
volatile Fighter_t Enemy;

volatile uint32_t ADCdata=0;
volatile uint32_t JoyX=0, JoyY=0;
volatile uint32_t Buttons=0;
volatile uint32_t LastButtons=0;
volatile uint32_t Score=0;
volatile uint32_t FrameFlag=0;
volatile uint32_t Paused=0;
volatile uint32_t GameOver=0;
volatile uint32_t PlayerWon=0;
volatile uint32_t EffectTimer=0;
volatile int32_t EffectX=0;
volatile int32_t EffectY=0;
volatile uint32_t EnemyThink=0;
volatile int32_t PrevPlayerX=20;
volatile int32_t PrevPlayerY=GROUND_Y;
volatile int32_t PrevEnemyX=98;
volatile int32_t PrevEnemyY=GROUND_Y;
volatile int32_t EnemyDir=-1;
volatile uint32_t ArenaDrawn=0;
volatile uint32_t PauseOverlayVisible=0;
volatile uint32_t MusicFramesLeft=0;
volatile uint32_t MusicIndex=0;
volatile uint32_t MusicMuteFrames=0;
volatile int32_t PrevPlayerHP=-1;
volatile int32_t PrevEnemyHP=-1;
#define LED_HIT_ENEMY LED_BLOCK   // PA15
#define LED_HIT_PLAYER LED_PUNCH  // PA17
volatile uint32_t GreenLEDTimer = 0;
volatile uint32_t RedLEDTimer = 0;

const int32_t JumpTable[8]={0,6,11,14,14,11,6,0};

const char Title_English[]="STREET FIGHTER";
const char Title_Spanish[]="PELEA CALLEJERA";
const char Select_English[]="Punch=English";
const char Select_Spanish[]="Block=Espa\xA4ol";
const char Start_English[]="Press a button";
const char Start_Spanish[]="Oprima boton";
const char Win_English[]="YOU WIN";
const char Win_Spanish[]="TU GANAS";
const char Lose_English[]="YOU LOSE";
const char Lose_Spanish[]="TU PIERDES";
const char Pause_English[]="PAUSED";
const char Pause_Spanish[]="PAUSA";
const char Score_English[]="Score";
const char Score_Spanish[]="Puntos";
const char HP_English[]="HP";
const char HP_Spanish[]="HP";
const char Fight_English[]="Punch Block Pause";
const char Fight_Spanish[]="Golpe Bloquea Pausa";
const char CtrlTitle_English[]="Choose Control";
const char CtrlTitle_Spanish[]="Elige Control";
const char CtrlSlide_English[]="Punch=Slidepot";
const char CtrlSlide_Spanish[]="Golpe=Deslizador";
const char CtrlJoy_English[]="Block=Joystick";
const char CtrlJoy_Spanish[]="Bloquea=Joystick";

void Game_Reset(void){
  Player.x = 20;
  Player.y = GROUND_Y;
  Player.hp = MAX_HP;
  Player.attackTimer = 0;
  Player.kickAttack = 0;
  Player.hitDone = 0;
  Player.facingRight = 1;
  Player.blocking = 0;
  Player.jumpTimer = 0;

  Enemy.x = 98;
  Enemy.y = GROUND_Y;
  Enemy.hp = MAX_HP;
  Enemy.attackTimer = 0;
  Enemy.kickAttack = 0;
  Enemy.hitDone = 0;
  Enemy.facingRight = 0;
  Enemy.blocking = 0;
  Enemy.jumpTimer = 0;

  Score = 0;
  FrameFlag = 0;
  Paused = 0;
  GameOver = 0;
  PlayerWon = 0;
  EffectTimer = 0;
  EffectX = 0;
  EffectY = 0;
  EnemyThink = 20;
  PrevPlayerX = Player.x;
  PrevPlayerY = Player.y;
  PrevEnemyX = Enemy.x;
  PrevEnemyY = Enemy.y;
  EnemyDir = -1;
  ArenaDrawn = 0;
  PauseOverlayVisible = 0;
  MusicFramesLeft = 0;
  MusicIndex = 0;
  MusicMuteFrames = 0;
  PrevPlayerHP = -1;
  PrevEnemyHP = -1;
  GreenLEDTimer = 0;
  RedLEDTimer = 0;
  LED_Off(LED_HIT_ENEMY | LED_HIT_PLAYER);
  Buttons = 0;
  LastButtons = 0;
}

void DrawHealthBar(int32_t x, int32_t y, int32_t hp, uint16_t color){
  int32_t width = hp*5;
  if(width < 0){
    width = 0;
  }
  if(width > 50){
    width = 50;
  }
  ST7735_FillRect(x, y, 50, 6, ST7735_DARKGREY);
  ST7735_FillRect(x, y, width, 6, color);
}
void InitArenaBackground(void){
  ST7735_FillScreen(ST7735_BG);
  ST7735_DrawBitmap(0, 150, (uint16_t*)sun, 128, 118);
  ST7735_DrawFastHLine(0, 20, 128, ST7735_BLACK);
  ST7735_FillRect(0, 151, 128, 9, ST7735_BLACK);
  ST7735_SetCursor(2,0);
  if(myLanguage == English){
    ST7735_OutString((char *)HP_English);
  }else{
    ST7735_OutString((char *)HP_Spanish);
  }
  DrawHealthBar(4, 6, Player.hp, ST7735_GREEN);
  DrawHealthBar(74, 6, Enemy.hp, ST7735_RED);
  PrevPlayerHP = Player.hp;
  PrevEnemyHP = Enemy.hp;
  ArenaDrawn = 1;
}

// sun is 128x118, drawn at y_bottom=150, so it covers rows 33..150
#define SUN_TOP 33
void RestoreBackgroundRect(int32_t x, int32_t y, int32_t w, int32_t h){
  if(y < 21){ int32_t clip = 21 - y; y = 21; h -= clip; }
  if(x < 0){ w += x; x = 0; }
  if((x + w) > 128){ w = 128 - x; }
  if((y + h) > 151){ h = 151 - y; }
  if((w <= 0) || (h <= 0)) return;
  for(int32_t row = y; row < y + h; row++){
    if(row < SUN_TOP){
      ST7735_FillRect(x, row, w, 1, ST7735_BG);
    }else{
      ST7735_DrawBitmap(x, row, (uint16_t*)&sun[(150 - row) * 128 + x], w, 1);
    }
  }
}

void EraseSpriteBox(int32_t x, int32_t y, int32_t w, int32_t h){
  int32_t top = y - h;
  RestoreBackgroundRect(x, top, w, h);
}


volatile uint32_t PrevPlayerAttacking=0;
volatile uint32_t PrevEnemyAttacking=0;
volatile uint32_t PrevPlayerBlocking=0;
volatile uint32_t PrevEnemyBlocking=0;
volatile uint32_t PrevEffectTimer=0;
volatile int32_t PrevEffectX=0;
volatile int32_t PrevEffectY=0;

void DrawArena(void){
  uint32_t playerAttacking = (Player.attackTimer != 0);
  uint32_t enemyAttacking = (Enemy.attackTimer != 0);
  uint32_t playerMoved, enemyMoved;

  if(ArenaDrawn == 0){
    InitArenaBackground();
    PrevPlayerAttacking = !playerAttacking; // force redraw on first frame
    PrevEnemyAttacking  = !enemyAttacking;
    PrevPlayerBlocking  = 2;
    PrevEnemyBlocking   = 2;
  }

  if(PauseOverlayVisible){
    RestoreBackgroundRect(32, 60, 64, 20);
    PauseOverlayVisible = 0;
    PrevPlayerAttacking = !playerAttacking;
    PrevEnemyAttacking  = !enemyAttacking;
    PrevPlayerBlocking  = 2;
    PrevEnemyBlocking   = 2;
  }

  playerMoved = (Player.x != PrevPlayerX) || (Player.y != PrevPlayerY)
              || (playerAttacking != PrevPlayerAttacking);
  enemyMoved  = (Enemy.x != PrevEnemyX) || (Enemy.y != PrevEnemyY)
              || (enemyAttacking != PrevEnemyAttacking);

  // erase old FX when it expires
  if(PrevEffectTimer && (EffectTimer == 0)){
    EraseSpriteBox(PrevEffectX, PrevEffectY, FX_W, FX_H);
  }

  if((Player.hp != PrevPlayerHP) || (Enemy.hp != PrevEnemyHP)){
    DrawHealthBar(4, 6, Player.hp, ST7735_GREEN);
    DrawHealthBar(74, 6, Enemy.hp, ST7735_RED);
    PrevPlayerHP = Player.hp;
    PrevEnemyHP = Enemy.hp;
  }

  // update player sprite: draw new first, then erase only the vacated strip
  if(playerMoved){
    if((Player.y == PrevPlayerY) && (playerAttacking == PrevPlayerAttacking)){
      if(playerAttacking){
        ST7735_DrawBitmap(Player.x, Player.y, (uint16_t *)PlayerAttack, PLAYER_ATTACK_W, PLAYER_ATTACK_H);
      }else{
        ST7735_DrawBitmap(Player.x, Player.y, (uint16_t *)PlayerIdle, PLAYER_IDLE_W, PLAYER_IDLE_H);
      }
      if(Player.x > PrevPlayerX){
        EraseSpriteBox(PrevPlayerX, PrevPlayerY, Player.x - PrevPlayerX, PLAYER_IDLE_H);
      }else if(Player.x < PrevPlayerX){
        EraseSpriteBox(Player.x + PLAYER_IDLE_W, PrevPlayerY, PrevPlayerX - Player.x, PLAYER_IDLE_H);
      }
    }else{
      EraseSpriteBox(PrevPlayerX, PrevPlayerY, PLAYER_IDLE_W, PLAYER_IDLE_H);
      if(playerAttacking){
        ST7735_DrawBitmap(Player.x, Player.y, (uint16_t *)PlayerAttack, PLAYER_ATTACK_W, PLAYER_ATTACK_H);
      }else{
        ST7735_DrawBitmap(Player.x, Player.y, (uint16_t *)PlayerIdle, PLAYER_IDLE_W, PLAYER_IDLE_H);
      }
    }
  }

  // update enemy sprite: draw new first, then erase only the vacated strip
  if(enemyMoved){
    if((Enemy.y == PrevEnemyY) && (enemyAttacking == PrevEnemyAttacking)){
      if(enemyAttacking){
        ST7735_DrawBitmap(Enemy.x, Enemy.y, (uint16_t *)EnemyAttack, ENEMY_ATTACK_W, ENEMY_ATTACK_H);
      }else{
        ST7735_DrawBitmap(Enemy.x, Enemy.y, (uint16_t *)EnemyIdle, ENEMY_IDLE_W, ENEMY_IDLE_H);
      }
      if(Enemy.x > PrevEnemyX){
        EraseSpriteBox(PrevEnemyX, PrevEnemyY, Enemy.x - PrevEnemyX, ENEMY_IDLE_H);
      }else if(Enemy.x < PrevEnemyX){
        EraseSpriteBox(Enemy.x + ENEMY_IDLE_W, PrevEnemyY, PrevEnemyX - Enemy.x, ENEMY_IDLE_H);
      }
    }else{
      EraseSpriteBox(PrevEnemyX, PrevEnemyY, ENEMY_IDLE_W, ENEMY_IDLE_H);
      if(enemyAttacking){
        ST7735_DrawBitmap(Enemy.x, Enemy.y, (uint16_t *)EnemyAttack, ENEMY_ATTACK_W, ENEMY_ATTACK_H);
      }else{
        ST7735_DrawBitmap(Enemy.x, Enemy.y, (uint16_t *)EnemyIdle, ENEMY_IDLE_W, ENEMY_IDLE_H);
      }
    }
  }

  // draw FX on the frames it's active
  if(EffectTimer){
    ST7735_DrawBitmap(EffectX, EffectY, (uint16_t *)HitFX, FX_W, FX_H);
  }

  PrevPlayerX = Player.x;
  PrevPlayerY = Player.y;
  PrevEnemyX = Enemy.x;
  PrevEnemyY = Enemy.y;
  PrevPlayerAttacking = playerAttacking;
  PrevEnemyAttacking  = enemyAttacking;
  PrevPlayerBlocking  = Player.blocking;
  PrevEnemyBlocking   = Enemy.blocking;
  PrevEffectTimer = EffectTimer;
  PrevEffectX = EffectX;
  PrevEffectY = EffectY;
}

void DrawPauseScreen(void){
  DrawArena();
  ST7735_FillRect(32, 60, 64, 20, ST7735_BG);
  PauseOverlayVisible = 1;
  ST7735_SetCursor(8,8);
  if(myLanguage == English){
    ST7735_OutString((char *)Pause_English);
  }else{
    ST7735_OutString((char *)Pause_Spanish);
  }
}

void DrawGameOverScreen(void){
  ST7735_FillScreen(ST7735_BG);
  ArenaDrawn = 0;
  PauseOverlayVisible = 0;
  ST7735_SetCursor(6,4);
  if(PlayerWon){
    if(myLanguage == English){
      ST7735_OutString((char *)Win_English);
    }else{
      ST7735_OutString((char *)Win_Spanish);
    }
  }else{
    if(myLanguage == English){
      ST7735_OutString((char *)Lose_English);
    }else{
      ST7735_OutString((char *)Lose_Spanish);
    }
  }
  ST7735_SetCursor(6,7);
  if(myLanguage == English){
    ST7735_OutString((char *)Score_English);
  }else{
    ST7735_OutString((char *)Score_Spanish);
  }
  ST7735_OutChar(':');
  ST7735_OutUDec(Score);
  ST7735_SetCursor(3,11);
  if(myLanguage == English){
    ST7735_OutString("Pause=restart");
  }else{
    ST7735_OutString("Pausa=reinicio");
  }
}

void ChooseLanguage(void){
  uint32_t now;
  ST7735_FillScreen(ST7735_BG);
  ArenaDrawn = 0;
  PauseOverlayVisible = 0;
  ST7735_SetCursor(3,2);
  ST7735_OutString((char *)Title_English);
  ST7735_SetCursor(3,5);
  ST7735_OutString((char *)Select_English);
  ST7735_SetCursor(3,7);
  ST7735_OutString((char *)Select_Spanish);
  ST7735_SetCursor(3,10);
  ST7735_OutString((char *)Start_English);

  while(1){
    now = Switch_In();
    if(now & BTN_PUNCH){
      myLanguage = English;
      break;
    }
    if(now & BTN_BLOCK){
      myLanguage = Spanish;
      break;
    }
  }
  Clock_Delay1ms(300);
}

void ChooseControl(void){
  uint32_t now;
  ST7735_FillScreen(ST7735_BG);
  ArenaDrawn = 0;
  PauseOverlayVisible = 0;
  ST7735_SetCursor(3,2);
  if(myLanguage == English){
    ST7735_OutString((char *)CtrlTitle_English);
    ST7735_SetCursor(3,5);
    ST7735_OutString((char *)CtrlSlide_English);
    ST7735_SetCursor(3,7);
    ST7735_OutString((char *)CtrlJoy_English);
  }else{
    ST7735_OutString((char *)CtrlTitle_Spanish);
    ST7735_SetCursor(3,5);
    ST7735_OutString((char *)CtrlSlide_Spanish);
    ST7735_SetCursor(3,7);
    ST7735_OutString((char *)CtrlJoy_Spanish);
  }
  while(1){
    now = Switch_In();
    if(now & BTN_PUNCH){
      myControl = CTRL_SLIDEPOT;
      break;
    }
    if(now & BTN_BLOCK){
      myControl = CTRL_JOYSTICK;
      break;
    }
  }
  Clock_Delay1ms(300);
}

void ShowInstructions(void){
  ST7735_FillScreen(ST7735_BG);
  ArenaDrawn = 0;
  PauseOverlayVisible = 0;
  ST7735_SetCursor(3,2);
  if(myLanguage == English){
    ST7735_OutString((char *)Title_English);
    ST7735_SetCursor(3,5);
    ST7735_OutString((char *)Fight_English);
    ST7735_SetCursor(3,8);
    ST7735_OutString("Slidepot moves");
  }else{
    ST7735_OutString((char *)Title_Spanish);
    ST7735_SetCursor(3,5);
    ST7735_OutString((char *)Fight_Spanish);
    ST7735_SetCursor(3,8);
    ST7735_OutString("Slidepot mueve");
  }
  Clock_Delay1ms(1500);
}

typedef enum{MUSIC_REST, MUSIC_C, MUSIC_D, MUSIC_E, MUSIC_F, MUSIC_G} MusicNote_t;

typedef struct{
  uint8_t note;
  uint8_t dur;
} SongStep_t;

#define MUSIC_UNIT_FRAMES 8
#define MUSIC_SFX_HOLD_FRAMES 10

void TriggerSFX(uint32_t effect){
  MusicMuteFrames = MUSIC_SFX_HOLD_FRAMES;
  switch(effect){
    case 0:
      Sound_Shoot();
      break;
    case 1:
      Sound_Fastinvader1();
      break;
    case 2:
      Sound_Fastinvader2();
      break;
    case 3:
      Sound_Highpitch();
      break;
    case 4:
      Sound_Killed();
      break;
    case 5:
      Sound_Explosion();
      break;
    default:
      break;
  }
}

void StartPlayerAttack(uint32_t kick){
  if(Player.attackTimer == 0){
    Player.attackTimer = kick ? PLAYER_KICK_TIME : PLAYER_PUNCH_TIME;
    Player.kickAttack = kick;
    Player.hitDone = 0;
    TriggerSFX(0);
  }
}

void StartEnemyAttack(void){
  if(Enemy.attackTimer == 0){
    Enemy.attackTimer = ENEMY_ATTACK_TIME;
    Enemy.kickAttack = Random(2);
    Enemy.hitDone = 0;
    if(Enemy.kickAttack){
      TriggerSFX(1);
    }else{
      TriggerSFX(2);
    }
  }
}

void UpdateJump(volatile Fighter_t *f){
  if(f->jumpTimer){
    f->y = GROUND_Y - JumpTable[8 - f->jumpTimer];
    f->jumpTimer--;
  }else{
    f->y = GROUND_Y;
  }
}

void DoCollisions(void){
  int32_t dist = Enemy.x - Player.x;
  if(dist < 0){
    dist = -dist;
  }

  if(Player.attackTimer && (Player.hitDone == 0) && (dist <= ATTACK_RANGE)){
    if(Enemy.blocking){
      EffectTimer = 2;
      EffectX = Enemy.x - 2;
      EffectY = Enemy.y - 2;
      TriggerSFX(3);
    }else{
      if(Player.kickAttack){
        Enemy.hp -= 2;
      }else{
        Enemy.hp -= 1;
      }
      if(Enemy.hp < 0){
        Enemy.hp = 0;
      }
      Score++;
      EffectTimer = 4;
      EffectX = Enemy.x - 2;
      EffectY = Enemy.y - 2;
      TriggerSFX(0); // shoot on hit
    }GreenLEDTimer = 6;
      LED_On(LED_HIT_ENEMY);
    Player.hitDone = 1;
  }

  if(Enemy.attackTimer && (Enemy.hitDone == 0) && (dist <= ATTACK_RANGE)){
    if(Player.blocking){
      EffectTimer = 2;
      EffectX = Player.x + PLAYER_IDLE_W;
      EffectY = Player.y - 2;
      TriggerSFX(3);
    }else{
      if(Enemy.kickAttack){
        Player.hp -= 2;
      }else{
        Player.hp -= 1;
      }
      if(Player.hp < 0){
        Player.hp = 0;
      }
      EffectTimer = 4;
      EffectX = Player.x + PLAYER_IDLE_W;
      EffectY = Player.y - 2;
      TriggerSFX(0); // shoot on hit
    }
    Enemy.hitDone = 1;
    RedLEDTimer = 6;
      LED_On(LED_HIT_PLAYER);
    }

  if(Enemy.hp <= 0){
    GameOver = 1;
    PlayerWon = 1;
    TriggerSFX(4); // killed at game over
  }
  if(Player.hp <= 0){
    GameOver = 1;
    PlayerWon = 0;
    TriggerSFX(4); // killed at game over
  }
}

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    if(myControl == CTRL_JOYSTICK){
      JoyStick_InPB((uint32_t*)&JoyX, (uint32_t*)&JoyY);
    }else{
      ADCdata = ADCin();
    }
    Buttons = Switch_In();

    if((Buttons & BTN_PAUSE) && ((LastButtons & BTN_PAUSE) == 0)){
    if(GameOver){
      Game_Reset();
    }else{
      Paused ^= 1;
      ArenaDrawn = 0;
      MusicMuteFrames = MUSIC_SFX_HOLD_FRAMES;
    }
  }

    if((Paused == 0) && (GameOver == 0)){
      if((Buttons & BTN_BLOCK) && ((LastButtons & BTN_BLOCK) == 0)){
        TriggerSFX(5); // explosion on block press
      }
      Player.blocking = (Buttons & BTN_BLOCK) ? 1 : 0;
      Enemy.blocking = 0;

      if((Buttons & BTN_PUNCH) && ((LastButtons & BTN_PUNCH) == 0)){
        StartPlayerAttack(0);
      }
      if(myControl == CTRL_SLIDEPOT){
        if(ADCdata > 4050){
          if(Player.jumpTimer == 0){
            Player.jumpTimer = 8;
          }
        }
        Player.x = PLAYER_MIN_X + ((ADCdata*(PLAYER_MAX_X-PLAYER_MIN_X))/4095);
      }else{
        if(JoyStick_ButtonPB() == 0){
          if(Player.jumpTimer == 0){
            Player.jumpTimer = 8;
          }
        }
        if(JoyX > 3000){
          Player.x -= 4;
        }else if(JoyX < 1000){
          Player.x += 4;
        }
      }
      if(Player.x < PLAYER_MIN_X){
        Player.x = PLAYER_MIN_X;
      }
      if(Player.x > PLAYER_MAX_X){
        Player.x = PLAYER_MAX_X;
      }

      UpdateJump(&Player);
      UpdateJump(&Enemy);

      if(Player.attackTimer){
        Player.attackTimer--;
      }
      if(Enemy.attackTimer){
        Enemy.attackTimer--;
      }
      if(EffectTimer){
        EffectTimer--;
      }

      {
        int32_t dist = Enemy.x - Player.x;
        if(dist < 0){
          dist = -dist;
        }
        if(EnemyThink){
          EnemyThink--;
        }else{
          EnemyThink = 3 + Random(5);
          Enemy.blocking = 0;
          if((Enemy.attackTimer == 0) && (dist <= ATTACK_RANGE + 6)){
            if(Random(3) != 0){
              StartEnemyAttack();
            }else{
              Enemy.blocking = 1;
            }
          }
        }

        if(Enemy.attackTimer == 0){
          if(dist <= ATTACK_RANGE + 2){
            if(Enemy.x > Player.x){
              EnemyDir = 1;
            }else{
              EnemyDir = -1;
            }
          }
          Enemy.x += EnemyDir;
          if(Enemy.x <= ENEMY_MIN_X){
            Enemy.x = ENEMY_MIN_X;
            EnemyDir = 1;
          }
          if(Enemy.x >= ENEMY_MAX_X){
            Enemy.x = ENEMY_MAX_X;
            EnemyDir = -1;
          }
        }
      }

      DoCollisions();
    }

    LastButtons = Buttons;
    FrameFlag = 1;
    if(GreenLEDTimer){
      GreenLEDTimer--;
      LED_On(LED_HIT_ENEMY);
    }else{
      LED_Off(LED_HIT_ENEMY);
    }
    if(RedLEDTimer){
      RedLEDTimer--;
      LED_On(LED_HIT_PLAYER);
    }else{
      LED_Off(LED_HIT_PLAYER);
    }
  }
}

uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BG);
  ST7735_DrawBitmap(6, 150, (uint16_t *)PlayerIdle, PLAYER_IDLE_W, PLAYER_IDLE_H);
  ST7735_DrawBitmap(34, 150, (uint16_t *)PlayerAttack, PLAYER_ATTACK_W, PLAYER_ATTACK_H);
  ST7735_DrawBitmap(72, 150, (uint16_t *)EnemyIdle, ENEMY_IDLE_W, ENEMY_IDLE_H);
  ST7735_DrawBitmap(98, 150, (uint16_t *)EnemyAttack, ENEMY_ATTACK_W, ENEMY_ATTACK_H);
  ST7735_DrawBitmap(60, 100, (uint16_t *)HitFX, FX_W, FX_H);

  ST7735_SetCursor(1,1);
  ST7735_OutString("Local sprite test");
  ST7735_SetCursor(1,3);
  ST7735_OutString("Player / Enemy / FX");
  while(1){
  }
}

// use main3 to test switches and LEDs

// use main3 to test switches and LEDs
// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    uint32_t now = Switch_In();
    LED_Off(LED_PUNCH|LED_KICK|LED_BLOCK|LED_PAUSE);

    if(now & 0x01){        // PA24
      LED_On(LED_PAUSE);   // PA8
    }
    if(now & 0x02){        // PA25
      LED_On(LED_BLOCK);   // PA15
    }
    if(now & 0x04){        // PA26
      LED_On(LED_KICK);    // PA16
    }
    if(now & 0x08){        // PA27
      LED_On(LED_PUNCH);   // PA17
    }
  }
}
// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
    last = now;
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BG);
  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  ChooseLanguage();
  ChooseControl();
  ShowInstructions();
  Game_Reset();
  InitArenaBackground();
  __enable_irq();

  while(1){
    // wait for semaphore
    while(FrameFlag == 0){}
       // clear semaphore
    FrameFlag = 0;
       // update ST7735R
    if(GameOver){
      DrawGameOverScreen();
    }else if(Paused){
      DrawPauseScreen();
    }else{
      DrawArena();
    }
    // check for end game or level switch
  }
}