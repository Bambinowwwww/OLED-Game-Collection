#ifndef CONTROLS_H
#define CONTROLS_H

#define BTN_COUNT   5
#define BTN_LEFT    0
#define BTN_DOWN    1
#define BTN_RIGHT   2
#define BTN_B       3
#define BTN_A       4

#define BTN_LEFT_PIN    8  
#define BTN_DOWN_PIN    14
#define BTN_RIGHT_PIN   16
#define BTN_B_PIN       9
#define BTN_A_PIN       12

uint8_t btnPins[BTN_COUNT];
uint8_t btnStates[BTN_COUNT];

void setupControls() {
  btnPins[BTN_LEFT] = BTN_LEFT_PIN;
  btnPins[BTN_DOWN] = BTN_DOWN_PIN;
  btnPins[BTN_RIGHT] = BTN_RIGHT_PIN;
  btnPins[BTN_B] = BTN_B_PIN;
  btnPins[BTN_A] = BTN_A_PIN;
}

void updateControls() {
  for (uint8_t thisButton = 0; thisButton < BTN_COUNT; thisButton++) {
        pinMode(btnPins[thisButton], INPUT_PULLUP);
        if (digitalRead(btnPins[thisButton]) == LOW) {
            btnStates[thisButton]++;
        } else {
            if (btnStates[thisButton] == 0)
                continue;
            if (btnStates[thisButton] == 0xFF)//if previously released
                btnStates[thisButton] = 0; //set to idle
            else
                btnStates[thisButton] = 0xFF; //button just released
        }
        pinMode(btnPins[thisButton], INPUT); //disable internal pull up resistors to save power
    }
}

boolean buttonPressed(uint8_t button) {
    if (btnStates[button] == 1) {
        return true;
    } else {
        return false;
    }
}


boolean buttonReleased(uint8_t button) {
    if (btnStates[button] == 0xFF) {
        return true;
    } else {
        return false;
    }
}

boolean buttonHeld(uint8_t button, uint8_t time){
    if(btnStates[button] == (time+1)) {
        return true;
    } else {
        return false;
    }
}

boolean buttonRepeat(uint8_t button, uint8_t period) {
    if (period <= 1) {
        if ((btnStates[button] != 0xFF) && (btnStates[button]))
            return true;
    } else {
        if ((btnStates[button] != 0xFF) && ((btnStates[button] % period) == 1))
            return true;
    }
    return false;
}

uint8_t buttonTimeHeld(uint8_t button){
    if(btnStates[button] != 0xFF) {
        return btnStates[button];
    } else {
        return 0;
    }
}

boolean buttonPressedAny() {
  return (buttonPressed(BTN_LEFT) || buttonPressed(BTN_DOWN) || buttonPressed(BTN_RIGHT) || buttonPressed(BTN_B) || buttonPressed(BTN_A));
}

#endif
