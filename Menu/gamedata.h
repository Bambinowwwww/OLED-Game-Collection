#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <EEPROM.h>
#include "gametetrominos.h"
#include "controls.h"
#include "gameview.h"
#include "sound.h"

#define ROWS_VISIBLE 20
#define ROWS 22
#define COLS 10

int EEPROMaddress = 0;
long score = 0;
long highscore = 0;
int lines = 0;
int level = 0;
boolean boardMap[ROWS][COLS] = {{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}}; // row 0/0 == bottom left(row 0 bottom / top visible 19)

const uint8_t fallSpeedPerLevel[] = { 18, 16, 15, 13, 11, 9, 7, 6, 5, 4, 3, 3, 2, 2, 2, 1 }; // frame skip before fall 1 block(fast fall is every frame)
const uint8_t fallLockDelay = 11; // in frames / resets on successuful rotation or shifting
uint8_t actionFrameCount = 0;
boolean fallFast = false;
boolean fallFastEnabled = true;

int8_t sideMovementFrameCount = 0;
const int8_t sideMovementFrameStart = -2;
const int8_t sideMovementFrameInterval = 2;

boolean boardRowsToRemove[ROWS] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
const int8_t boardRowsRemoveInterval = 10;
int8_t boardRowsRemoveBlinkFrameCount = 0;
const int8_t boardRowsRemoveBlinkInterval = 1;
boolean dontDrawFilledRows = false; // for blink animation blah...

int8_t tetCol = 4; // position
int8_t tetRow = 17;// position
int8_t tetRows = 3; // array size
int8_t tetCols = 3;
uint8_t tetDataSize = tetRows * tetCols;
uint8_t tetRotation = 0;
Tetromino tetCurrent;
Tetromino tetNext;
boolean *tetData;
int8_t r = 0; // za loops
int8_t c = 0;
int8_t mr = 0; // mino row
int8_t mc = 0; // mino col

boolean removeRowsAnimation = false;
boolean endFillAnimation = false; // game end fill animation
boolean gameEnded = false;

int bagNextIndex = 8;
Tetromino bag[] = {T_I,  T_J,  T_L,  T_O,  T_S,  T_T,  T_Z };


////////////////////////////////////
// SCORE
////////////////////////////////////

void EEPROMWritelong(int address, long value) {
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
      
long EEPROMReadlong(int address) {
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void saveHighscore() {
  EEPROMWritelong(EEPROMaddress, highscore);
}


void loadHighscore() {
  highscore = EEPROMReadlong(EEPROMaddress);
  if (highscore < 0) {
    highscore = 0;
    saveHighscore();
  }
}

void scoreAddDropPoints() {
  score += 1;
}

void scoreAddClearRowsPoints(uint8_t clearedLines) {
  switch(clearedLines) {
    case 1: score += 40 * (level + 1); break;
    case 2: score += 100 * (level + 1); break;
    case 3: score += 300 * (level + 1); break;
    case 4: score += 1200 * (level + 1); break;
  }
}



////////////////////////////////////
// BOARD
////////////////////////////////////

void clearBoard() {
  for (uint8_t row=0; row < ROWS; row++){
    for (uint8_t col=0; col < COLS; col++){
        boardMap[row][col] = false;
    }
  }
//  for (uint8_t col=0; col < COLS; col++){
//      if (col < 4 || col > 5) {
//          boardMap[0][col] = true;
//      }
//    }
}

boolean haveRowsToRemove() {
  boolean have = false;
  for (uint8_t row=0; row < ROWS; row++){
    mc = 0; // mino count
    for (uint8_t col=0; col < COLS; col++){
      if (boardMap[row][col]) mc++;
    }
    if (mc == COLS) {
      boardRowsToRemove[row] = true;
      have = true;
    } else {
      boardRowsToRemove[row] = false;
    }
  }
  return have;
}

uint8_t boardRemoveFilledRows() {
  uint8_t removeRows = 0;
  uint8_t row2;
  for (uint8_t row=0; row < ROWS; row++){
    mc = 0; // mino count
    for (uint8_t col=0; col < COLS; col++){
      if (boardMap[row][col]) mc++;
    }
    if (mc == COLS) {
      removeRows++;
      for (uint8_t row2=row; row2 < ROWS; row2++){
        for (uint8_t col=0; col < COLS; col++){
          if (row2 != ROWS-1) {
            boardMap[row2][col] = boardMap[row2+1][col];
          } else {
            boardMap[row2][col] = false;
          }
        }
      }
      row--;
    }
  }
  return removeRows;
}

void boardFillRow(uint8_t fillRow) {
  for (uint8_t col=0; col < COLS; col++){
    boardMap[fillRow][col] = true;
  }
}

////////////////////////////////////
// GENERATE TETROMINO
////////////////////////////////////

void shuffleBag() {
  int last = 0;
  Tetromino temp = bag[last];
  for (int i=0; i<7; i++) {
    int index = random(7);
    bag[last] = bag[index];
    last = index;
  }
  bag[last] = temp;
  //for(int j = 0; j < 7; j++) { Serial.print(bag[j]); Serial.print(" "); } Serial.println("");
}

void nextTetromino() {
  tetCurrent = bag[bagNextIndex];
  bagNextIndex++;
  if (bagNextIndex == 7) {
    bagNextIndex = 0;
    shuffleBag();
  }
  tetNext = bag[bagNextIndex];
  //tetCurrent = T_I; // hack
  switch(tetCurrent) {
      case T_I: tetRows = 4; tetCols = 4; tetData = t_i; break;
      case T_J: tetRows = 3; tetCols = 3; tetData = t_j; break;
      case T_L: tetRows = 3; tetCols = 3; tetData = t_l; break;
      case T_O: tetRows = 3; tetCols = 4; tetData = t_o; break;
      case T_S: tetRows = 3; tetCols = 3; tetData = t_s; break;
      case T_T: tetRows = 3; tetCols = 3; tetData = t_t; break;
      case T_Z: tetRows = 3; tetCols = 3; tetData = t_z;  break;
  }
  tetCol = 3; 
  tetRow = 17; 
  tetRotation = 0;
  tetDataSize = tetRows * tetCols;
}

////////////////////////////////////
// MOVE/ROTATE/LAND TETROMINO
////////////////////////////////////

bool tetColliding(int8_t tr, int8_t tc, uint8_t ttr) { // row/col/rotation
  for (r=0; r<tetRows; r++) {
      for (c=0; c<tetCols; c++) {
        if (tetData[c + tetCols * r + tetDataSize * ttr]) {
          mr = tr + r; // mino row
          mc = tc + c; // mino col
          if (boardMap[mr][mc] || mr < 0 || mc < 0 || mc >= COLS) {
            return true;
          }
        }
      }
    }
    return false;
}


bool tetCanMoveLeft() { return !tetColliding(tetRow, tetCol-1, tetRotation); }

bool tetCanMoveRight() { return !tetColliding(tetRow, tetCol+1, tetRotation); }

bool tetCanMoveDown() { return !tetColliding(tetRow-1, tetCol, tetRotation); }

bool tetMoveLeft() {
  if (tetCanMoveLeft()) {
    tetCol--;
    return true;
  }
  return false;
}

bool tetMoveRight() {
  if (tetCanMoveRight()) {
    tetCol++;
    return true;
  }
  return false;
}

bool tetMoveDown() {
  if (tetCanMoveDown()) {
    tetRow--;
    return true;
  }
  return false;
}

bool tetRotate(uint8_t tetNewRotation, uint8_t tetWallKickIndex) {
  if (tetCurrent == T_I) {

    for (uint8_t i = 0; i < 5; i++) {
      if (!tetColliding(tetRow+wallKickDataI[tetWallKickIndex][i][1], tetCol+wallKickDataI[tetWallKickIndex][i][0], tetNewRotation)) {
        tetCol = tetCol+wallKickDataI[tetWallKickIndex][i][0];
        tetRow = tetRow+wallKickDataI[tetWallKickIndex][i][1];
        tetRotation = tetNewRotation;
        return true;
      }
    }
    
  } else {
    
    for (uint8_t i = 0; i < 5; i++) {
      if (!tetColliding(tetRow+wallKickData[tetWallKickIndex][i][1], tetCol+wallKickData[tetWallKickIndex][i][0], tetNewRotation)) {
        tetCol = tetCol+wallKickData[tetWallKickIndex][i][0];
        tetRow = tetRow+wallKickData[tetWallKickIndex][i][1];
        tetRotation = tetNewRotation;
        return true;
      }
    }
  }
  return false;
}

bool tetRotateRight() {

  if (tetCurrent == T_O) {
    return true;
  } else {
    uint8_t tetNewRotation = (tetRotation == 3) ? 0 : tetRotation + 1;
    switch(tetRotation) {
      case 0: return tetRotate(tetNewRotation, 0);  break;
      case 1: return tetRotate(tetNewRotation, 2);  break;
      case 2: return tetRotate(tetNewRotation, 4);  break;
      case 3: return tetRotate(tetNewRotation, 6);  break;
    }
  }
  return false;
}

bool tetRotateLeft() {
  
  if (tetCurrent == T_O) {
    return true;
  } else {
    uint8_t tetNewRotation = (tetRotation == 0) ? 3 : tetRotation - 1;
    switch(tetRotation) {
      case 0: return tetRotate(tetNewRotation, 7);  break;
      case 1: return tetRotate(tetNewRotation, 1);  break;
      case 2: return tetRotate(tetNewRotation, 3);  break;
      case 3: return tetRotate(tetNewRotation, 5);  break;
    }
  }
  return false;
}

void tetLand() {
  for (r=0; r<tetRows; r++) {
     for (c=0; c<tetCols; c++) {
       if (tetData[c + tetCols * r + tetDataSize * tetRotation]) {
          mr = tetRow + r; // mino row
          mc = tetCol + c; // mino col
          boardMap[mr][mc] = true;
       }
     }
   }
   tetCurrent = T_NONE;
}

void updateTetromino() {
  
  // Move Left
  if (buttonRepeat(BTN_LEFT, 0) && !buttonRepeat(BTN_RIGHT, 0)) {
    if (buttonPressed(BTN_LEFT)) {
      sideMovementFrameCount = sideMovementFrameStart;
      if (tetMoveLeft()) {
          soundMove();
      }
    } else {
      sideMovementFrameCount++;
      if (sideMovementFrameCount >= sideMovementFrameInterval) {
        sideMovementFrameCount = 0;
        if (tetMoveLeft()) {
          soundMove();
        }
      }
    }
  }

  // Move Right
  if (buttonRepeat(BTN_RIGHT, 0) && !buttonRepeat(BTN_LEFT, 0)) {
     if (buttonPressed(BTN_RIGHT)) {
      sideMovementFrameCount = sideMovementFrameStart;
      if (tetMoveRight()) {
          soundMove();
      };
    } else {
      sideMovementFrameCount++;
      if (sideMovementFrameCount >= sideMovementFrameInterval) {
        sideMovementFrameCount = 0;
        if (tetMoveRight()) {
          soundMove();
        }
      }
    }
  }
  
  // Move Down
  fallFast = (fallFastEnabled && !buttonRepeat(BTN_LEFT, 0) && !buttonRepeat(BTN_RIGHT, 0)) ? buttonRepeat(BTN_DOWN, 0) : false;
  if (!tetCanMoveDown()) {
    actionFrameCount++;
    if (actionFrameCount >= fallLockDelay || fallFast) {
        tetLand();
        fallFastEnabled = !fallFast;
        actionFrameCount = 0;
        soundLand();
    }
  } else  if (fallFast || actionFrameCount >= fallSpeedPerLevel[min(level, 15)]) {
    if (tetMoveDown()) {
      if (fallFast) {
        tetMoveDown(); // fall faster then one cell per frame
        scoreAddDropPoints();
        scoreAddDropPoints();
      }
      actionFrameCount = 0;
    } 
  } else {
    actionFrameCount++;
  }

  // Rotate
  if (buttonPressed(BTN_A) && !buttonRepeat(BTN_B, 0)) {
    if (tetRotateRight()) {
        soundRotate();
    };
  } else if (buttonPressed(BTN_B) && !buttonRepeat(BTN_A, 0)) {
    if (tetRotateLeft()) {
        soundRotate();
    };
  }
}


////////////////////////////////////
// ANIMATIONS DATA
////////////////////////////////////


boolean updateRemoveRowsAnimationData() {

  if (actionFrameCount == boardRowsRemoveInterval) {
      dontDrawFilledRows = false;
      return true;
    } else {
      actionFrameCount++;
    }
    if (boardRowsRemoveBlinkFrameCount == boardRowsRemoveBlinkInterval) {
      boardRowsRemoveBlinkFrameCount = 0;
      dontDrawFilledRows = !dontDrawFilledRows;
    } else {
      boardRowsRemoveBlinkFrameCount++;
    }
    return false;
}

boolean updateFillRowsRowsAnimationData() {

  boardFillRow(actionFrameCount);
  actionFrameCount++;
  if (actionFrameCount == ROWS_VISIBLE) {
    return true;
  }
  return false;
}


////////////////////////////////////
// MAIN LOGIC
////////////////////////////////////

void setNewGameData() { // random_shuffle(&a[0], &a[10]);
  score = 0;
  lines = 0;
  level = 0;
  bagNextIndex = 0;
  actionFrameCount = 0;
  endFillAnimation = false;
  removeRowsAnimation = false;
  gameEnded = false;
  fallFastEnabled = true;
  shuffleBag();
  nextTetromino();
  clearBoard();
}

void updateGame() {

  // Enable fall fast on key release after tetromino land
  if (!fallFastEnabled && buttonReleased(BTN_DOWN)) {
    fallFastEnabled = true;
  }
  
  if (tetCurrent != T_NONE && !endFillAnimation) {
    updateTetromino();
    if (tetCurrent == T_NONE && haveRowsToRemove()) {
      removeRowsAnimation = true;
      actionFrameCount = 0;
      boardRowsRemoveBlinkFrameCount = 0;
      dontDrawFilledRows = false;
      soundClear();
    }

  } else if (removeRowsAnimation) {

     if (updateRemoveRowsAnimationData()) { // ended
      removeRowsAnimation = false;
      uint8_t removedRows = boardRemoveFilledRows();
      scoreAddClearRowsPoints(removedRows);
      lines += removedRows;
      uint8_t newLevel = lines / 10;
      actionFrameCount = 0;
      if (newLevel != level) {
        level = newLevel;
        soundLevel();
      }
     }
    
  } else if (endFillAnimation) {

    if (updateFillRowsRowsAnimationData()) { // ended
      endFillAnimation = false;
      gameEnded = true;
      if (score > highscore) {
        highscore = score;
        saveHighscore();
      }
    }

  } else {

    nextTetromino();
    if (tetColliding(tetRow, tetCol, tetRotation)) {
      endFillAnimation = true;
      actionFrameCount = 0;
      soundOver();
    }
  }
  
}

#endif
