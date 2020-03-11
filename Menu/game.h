#ifndef GAME_H
#define GAME_H

#include "gameview.h"
#include "controls.h"

void stateGameStart() {
  setNewGameData();
  gameState = STATE_GAME_PLAYING;
}

void stateGamePause() {
  drawGamePause();
  if (buttonPressedAny()) {
    soundButton();
    gameState = STATE_GAME_PLAYING;
  } 
}

void stateGamePlaying() {
   
    updateGame();
    drawGame();
    if (gameEnded) {
      gameState = STATE_GAME_OVER;
    }
  }


void stateGameOver() {
  drawGameOver();
  if (buttonPressedAny()) {
    soundButton();
    gameState = STATE_HOME;
  } 
}



#endif
