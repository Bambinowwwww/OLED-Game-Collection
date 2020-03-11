#ifndef SPLASH_H
#define SPLASH_H

#include "sound.h"


boolean splashSoundPlayed = false;
uint8_t splashShowFramesCount = 0;

void stateSplash() {
  if (!splashSoundPlayed && splashShowFramesCount > 1) {
    splashSoundPlayed = true;
    soundSplash();
  }
  if (splashShowFramesCount <= 22) {
    display.drawBitmap(9, 30, SPLASH, SPLASH_WIDTH, SPLASH_HEIGHT, 1, 0);
  }
  if (splashShowFramesCount >= 30) {
    gameState = STATE_HOME;
  } else {
    splashShowFramesCount++;
  }
};

// Old sliding logo splash
//static int splashX = 9;
//int splashY = -SPLASH_HEIGHT;
//static int splashTargetY = 30;
//
//void stateSplash() {
//  
//  if (splashY == splashTargetY) {
//    splashY += 1;
//    delay(1000);
//    //display.clearDisplay();
//    //display.display();
//    //delay(200);
//    gameState = STATE_HOME;
//  } else {
//    //display.fillRect(0, 0, 48, 84, BLACK);
//    display.drawBitmap(splashX, splashY, SPLASH, SPLASH_WIDTH, SPLASH_HEIGHT, 1, 0);
//    splashY += 1;
//  }
//};

#endif
