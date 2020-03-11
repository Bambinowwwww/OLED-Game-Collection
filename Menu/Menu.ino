//#include "SpaceTrash_74HC138.h"
//#include "Calculator.h"
#include "Pong.h"
#include "Tetrisw.h"
#include "lib.h"
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//U8G2_PCD8544_84X48_1_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 16, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);

// End of constructor list

short int SelectIndex = 1;
short int LowerRange = 1;
short int UpperRange = 4;
short int Time = 0;

short int ListIndex = 0;
short int InPageIndex = 1;
short int ChoiceLength = 4;
String listOfChoices[] = {"Space Trash","Calculator","Pong","Tetris"};

void setup(void)
{
    ControlPinsSetup();
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
}

void loop(void){
    if(keyDetectionSingle(0,7) && (SelectIndex > LowerRange)){
      SelectIndex--;
      if(InPageIndex > 1){
        InPageIndex--;
      }else if(ListIndex>0){
        ListIndex--;
      }
    }
    if(keyDetectionSingle(3,7) && (SelectIndex < UpperRange)){
      SelectIndex++;
      if(InPageIndex < 3){
        InPageIndex++;
      }else if(ListIndex < ChoiceLength - 2){
        ListIndex++;
      }
    }
    if(keyDetectionSingle(7,7)){
      switch(SelectIndex){
        case 1:
          StartPong();
          break;
        case 2:
           StartTetris();
           break;
     
      }
    }
    
    u8g2.firstPage();
    //u8g2.clearDisplay();
    do{
        u8g2.drawStr(0,9,"Menu");
        u8g2.setCursor(78, 9);
        u8g2.print(SelectIndex);
        u8g2.drawHLine(0, 10, 84);
        u8g2.drawStr(0,10*(InPageIndex + 1),"=");
        //u8g2.drawStr(7,20,"Space Trash");
        //u8g2.drawStr(7,30,"Calculator");
        //u8g2.drawStr(7,40,"Empty");
        char Line1[15];
        char Line2[15]; 
        char Line3[15]; 
        listOfChoices[0+ListIndex].toCharArray(Line1, 15);
        listOfChoices[1+ListIndex].toCharArray(Line2, 15);
        listOfChoices[2+ListIndex].toCharArray(Line3, 15);
        u8g2.drawStr(7,20,Line1);
        u8g2.drawStr(7,30,Line2);
        u8g2.drawStr(7,40,Line3);
    }while (u8g2.nextPage());
    //delay(50);
}
