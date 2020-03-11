#ifndef GLOBALS_H
#define GLOBALS_H

const unsigned long FPS = 20;
#define GAME_WIDTH 48
#define GAME_HEIGHT 84

// States
#define STATE_SPLASH            0
#define STATE_HOME              1
#define STATE_GAME_START        2
#define STATE_GAME_PLAYING      3
#define STATE_GAME_PAUSE        4
#define STATE_GAME_OVER         5

// State of the game
byte gameState = STATE_SPLASH;

#endif
