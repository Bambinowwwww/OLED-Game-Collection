#ifndef HOME_H
#define HOME_H

#include "globals.h"
#include "controls.h"
#include "sound.h"
#include "gameview.h"
#include "gamedata.h"

void stateHome() {
  display.drawBitmap(0, 0, HOME, 48, 84, 1, 0);
  if (highscore > 0) {

    char scoreList[10];
    sprintf(scoreList, "%ld", highscore); //ltoa(val, buffer, 10);
  
    int hsX = 0;
    if (highscore < 9) {
      hsX = 23;
    }else if (highscore < 99) {
      hsX = 21;
    } else if (highscore < 999) {
      hsX = 19;
    } else if (highscore < 9999) {
       hsX = 17;
    } else if (highscore < 99999) {
      hsX = 15;
    } else if (highscore < 999999) {
      hsX = 13;
    } else {
      hsX = 11;
    }
    drawNumber(scoreList, sizeof(scoreList), hsX, 27); 
  }
  if (buttonPressedAny()) {
    //soundTest();
    soundButton();
    gameState = STATE_GAME_START;
  } 
}


#endif
