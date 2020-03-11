#ifndef MENU_H
#define MENU_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS/CE)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7,6,5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

 /*
1 RST--------- reset 
2 CE------------chip selection 
3 DC-----------data/commands choice 
4 DIN-----------serial data line 
5 CLK------------serial Clock Speed 
6 3.3V------------VCC 
7 LIGHT--------- backlight control terminal 
8 GND-----------power negative
*/

void setupDisplay() {
  display.begin();
  display.setContrast(60);
  display.setRotation(1);
  display.clearDisplay();
}


#endif
