
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
#include "lib.h"
 
typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM mainGameLoop[] = {
  stateSplash,
  stateHome,
  stateGameStart,
  stateGamePlaying,
  stateGameOver
};

const unsigned long eachFrameMillis = 1000 / FPS; // FPS
unsigned long nextFrameStart = 0;

boolean nextFrame();
void setupT() { 
  Serial.begin(9600);

  delay(10);
  randomSeed(analogRead(4)); //3
  
  setupDisplay();
  setupControls();
  loadHighscore();
  
}

void loopT() {
  if (nextFrame()) {
    updateControls();
    ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
    // audio
    display.display();
    display.clearDisplay();
    //fpsAndRamAtInterval(5);
  }
  soundUpdate();
}

boolean nextFrame() {
  unsigned long now = millis();
  if (now < nextFrameStart) {
    return false;
  }
  nextFrameStart = millis() + eachFrameMillis;
  return true;  
}

uint16_t getFreeRam() {
  //from http://www.controllerprojects.com/2011/05/23/determining-sram-usage-on-arduino/
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

static inline void fpsAndRamAtInterval(const int seconds){
  static unsigned long lastMillis;
  static unsigned long frameCount;
  static unsigned int framesPerSecond;
  unsigned long now = millis();
  frameCount ++;
  if (now - lastMillis >= seconds * 1000) {
    framesPerSecond = frameCount / seconds;
    Serial.println("------------");
    Serial.print("RAM Usage: ");
    Serial.print(2048 - getFreeRam());
    Serial.print(" / 2048");
    Serial.println();
    Serial.print("FPS: ");
    Serial.println(framesPerSecond);
    frameCount = 0;
    lastMillis = now;
  }
}

void StartTetris(){
  setupT();
  loopT();
  u8g2.setFont(u8g2_font_6x10_tf);
}
