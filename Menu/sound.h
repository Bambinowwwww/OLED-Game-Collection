#ifndef SOUND_H
#define SOUND_H

#include "utils/notes.c"

#define PIN_BUZZER  10

boolean soundPlaying = false;
int currentIndex = 0;
int currentLength;
int *currentDurations;
int *currentTones;
unsigned long counter = 0;

const int updateEveryMS = 1000 / 100;
const unsigned long timeShift = 5000; 
unsigned long audioTimeStamp = timeShift;

const int FX_SPLASH_LENGTH = 2;
const int FX_SPLASH_DURATIONS[FX_SPLASH_LENGTH] = { 20, 100 };
const int FX_SPLASH_TONES[FX_SPLASH_LENGTH] = { NOTE_A5, NOTE_A6 };

const int FX_MOVE_LENGTH = 1;
const int FX_MOVE_DURATIONS[FX_MOVE_LENGTH] = { 2 };
const int FX_MOVE_TONES[FX_MOVE_LENGTH] = { NOTE_A5 };

const int FX_ROTATE_LENGTH = 4;
const int FX_ROTATE_DURATIONS[FX_ROTATE_LENGTH] = { 15, 15, 15, 15 };
const int FX_ROTATE_TONES[FX_ROTATE_LENGTH] = { NOTE_A5, NOTE_A4, NOTE_A5, NOTE_A4 };

const int FX_LAND_LENGTH = 8;
const int FX_LAND_DURATIONS[FX_LAND_LENGTH] = { 5, 5, 5, 5, 5, 5, 5, 5 };
const int FX_LAND_TONES[FX_LAND_LENGTH] = { NOTE_A3, NOTE_B3, NOTE_C2, NOTE_D3, NOTE_E2, NOTE_F4, NOTE_G3, NOTE_C2 };

const int FX_PAUSE_LENGTH = 7;
const int FX_PAUSE_DURATIONS[FX_PAUSE_LENGTH] = { 50, 50, 50, 50, 50, 50, 50 };
const int FX_PAUSE_TONES[FX_PAUSE_LENGTH] = { NOTE_A5, NOTE_PAUSE, NOTE_A6, NOTE_PAUSE, NOTE_A5, NOTE_PAUSE, NOTE_A6 };

const int FX_CLEAR_LENGTH = 6;
const int FX_CLEAR_DURATIONS[FX_CLEAR_LENGTH] = { 20, 20, 20, 20, 20, 20 };
const int FX_CLEAR_TONES[FX_CLEAR_LENGTH] = { NOTE_A3, NOTE_C3, NOTE_E3, NOTE_A4, NOTE_C4, NOTE_E4 };

const int FX_LEVEL_LENGTH = 5;
const int FX_LEVEL_DURATIONS[FX_LEVEL_LENGTH] = { 10, 10, 10, 10, 10 };
const int FX_LEVEL_TONES[FX_LEVEL_LENGTH] = { NOTE_A5, NOTE_PAUSE, NOTE_A6, NOTE_PAUSE, NOTE_A5  };

const int FX_BTN_LENGTH = 1;
const int FX_BTN_DURATIONS[FX_BTN_LENGTH] = { 2 };
const int FX_BTN_TONES[FX_BTN_LENGTH] = { NOTE_F5 };

const int FX_OVER_LENGTH = 10;
const int FX_OVER_DURATIONS[FX_OVER_LENGTH] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };
const int FX_OVER_TONES[FX_OVER_LENGTH] = { NOTE_A2, NOTE_C2, NOTE_E2, NOTE_A3, NOTE_C3, NOTE_E3, NOTE_A4, NOTE_C4, NOTE_E4, NOTE_A5 };

void soundUpdate() {
  if (soundPlaying) {

      unsigned long elapsed = millis() + timeShift - audioTimeStamp;
      if (elapsed >= currentDurations[currentIndex]) {

        if (currentIndex < currentLength) {
          //noTone(PIN_BUZZER);
          if (currentTones[currentIndex] != NOTE_PAUSE) {
            tone(PIN_BUZZER, currentTones[currentIndex], currentDurations[currentIndex] + 10);
          }
          currentIndex++;
          audioTimeStamp = millis() + timeShift;
        } else {
          soundPlaying = false;
          //noTone(PIN_BUZZER);
        }
        
      }

  }
}

void soundPlay(const int &dur, const int &tones, const int lnght) {
  currentIndex = 0;
  audioTimeStamp = timeShift;
  currentLength = lnght;
  currentDurations = dur;
  currentTones = tones;
  soundPlaying = true;
}

void soundSplash() { soundPlay(FX_SPLASH_DURATIONS, FX_SPLASH_TONES, FX_SPLASH_LENGTH); }

void soundMove() { soundPlay(FX_MOVE_DURATIONS, FX_MOVE_TONES, FX_MOVE_LENGTH); }

void soundRotate() { soundPlay(FX_ROTATE_DURATIONS, FX_ROTATE_TONES, FX_ROTATE_LENGTH); }

void soundLand() { soundPlay(FX_LAND_DURATIONS, FX_LAND_TONES, FX_LAND_LENGTH); }

void soundPause() { soundPlay(FX_PAUSE_DURATIONS, FX_PAUSE_TONES, FX_PAUSE_LENGTH); }

void soundClear() { soundPlay(FX_CLEAR_DURATIONS, FX_CLEAR_TONES, FX_CLEAR_LENGTH); }

void soundLevel() { soundPlay(FX_LEVEL_DURATIONS, FX_LEVEL_TONES, FX_LEVEL_LENGTH); }

void soundButton() { soundPlay(FX_BTN_DURATIONS, FX_BTN_TONES, FX_BTN_LENGTH); }

void soundOver() { soundPlay(FX_OVER_DURATIONS, FX_OVER_TONES, FX_OVER_LENGTH); }

void soundTest() {
  soundRotate();
}


//
//// noTone(pin);
//
//void soundSplash() {
//  tone(10, notes[0], 20);
//}
//
//int melody[] = {
//  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
//};
//
//// note durations: 4 = quarter note, 8 = eighth note, etc.:
//int noteDurations[] = {
//  4, 8, 8, 4, 4, 4, 4, 4
//};
//void test() {
//
//  for (int thisNote = 0; thisNote < 8; thisNote++) {
//
//    // to calculate the note duration, take one second divided by the note type.
//    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//    int noteDuration = 1000 / noteDurations[thisNote];
//    tone(PIN_BUZZER, melody[thisNote], noteDuration);
//
//    // to distinguish the notes, set a minimum time between them.
//    // the note's duration + 30% seems to work well:
//    int pauseBetweenNotes = noteDuration * 1.30;
//    delay(pauseBetweenNotes);
//    // stop the tone playing:
//    noTone(PIN_BUZZER);
//  }
//}



#endif
