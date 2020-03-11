#pragma once

#include <Arduino.h>
#include <U8g2lib.h>

#include <avr/pgmspace.h>
#include "globals.h"
#include "display.h"
#include "bitmaps.h"
#include "splash.h"
#include "home.h"
#include "gameview.h"
#include "gamedata.h"
#include "gametetrominos.h"
#include "game.h"
#include "controls.h"
#include "sound.h"

#ifdef ARDUINO_AVR_MICRO
  #define PRO_MICRO
#endif
#ifdef ARDUINO_SAMD_ZERO
  #define SPARKFUN_SAMD_21
#endif

#define BOARD_74HC138_TO_DIRECT_WIRE
//#define BOARD_74HC138

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#ifdef SPARKFUN_SAMD_21
#define DP_CLK 13
#define DP_DAT 11
#define DP_CS 10
#define DP_DC 9
#define DP_RST 8
#endif

#ifdef PRO_MICRO
#define DP_CLK 15
#define DP_DAT 16
#define DP_CS 10
#define DP_DC 9
#define DP_RST 8
#endif

U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/8);
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#ifdef BOARD_74HC138

void ControlPinsSetup(){
    pinMode(4,OUTPUT); 
    pinMode(5,OUTPUT); 
    pinMode(6,OUTPUT); 
    pinMode(7,INPUT);
}

bool keyDetection(int KeyNumber, int ReadPin){//0 Up, 1 Left, 2 Right, 3 Down, 4 Y, 5 X, 6 B, 7 A
  bool state = false;
  digitalWrite(4,bitRead(KeyNumber,0));
  digitalWrite(5,bitRead(KeyNumber,1));
  digitalWrite(6,bitRead(KeyNumber,2));
  for(int i = 0; digitalRead(7) == 0; i ++){
    if(i >= 1){
      state = true;
      break;
    }
  }
  return state;
}

bool keyDetectionSingle(int KeyNumber, int ReadPin){//0 Up, 1 Left, 2 Right, 3 Down, 4 Y, 5 X, 6 B, 7 A
  bool state = false;
  for(int i = 0; keyDetection(KeyNumber,ReadPin) == true; i ++){
    if(i >= 1){
      state = true;
    }
  }
  return state;
}
#endif

#ifdef BOARD_74HC138_TO_DIRECT_WIRE

void ControlPinsSetup(){
    pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
}

bool keyDetection(int KeyNumber, int FOR_CONSISTENCY){//0 Up, 1 Left, 2 Right, 3 Down, 4 Y, 5 X, 6 B, 7 A
  short int LoopUp[] = {4,5,7,6,A3,A2,A1,A0};
  return !digitalRead(LoopUp[KeyNumber]);
}

bool keyDetectionSingle(int KeyNumber, int ReadPin){//0 Up, 1 Left, 2 Right, 3 Down, 4 Y, 5 X, 6 B, 7 A
  bool state = false;
  for(int i = 0; keyDetection(KeyNumber,ReadPin) == true; i ++){
    if(i >= 1){
      state = true;
    }
  }
  return state;
}
#endif
