#ifndef Animation_H
#define Animation_H
  #include "Arduino.h"
  #include "eyes.h"
  
  class Animation {
    #define LCD_CE      5   //D1
    #define LCD_RESET   4   //D2
    #define LCD_DC      12  //D6
    #define LCD_DIN     13  //D7
    #define LCD_CLK     14  //D5
    #define LCD_C       LOW
    #define LCD_D       HIGH
    
    #define OFFLINE   0
    #define NORMAL    1
    #define ALERT     2
    #define UPDATING  3
    #define UNDEFINED 5
    
    private:
      unsigned int nextBlink = 0;
      byte currentStatus = OFFLINE;
      byte lastStatus = UNDEFINED;
      unsigned long timer;

      void write(byte dc, byte data);
      void showImage(const byte *data);
      void blinkEyes();
      
    public:
      Animation();
      void setup();
      void setAlertMode();
      void setOfflineMode();
      void setNormalMode();
      void setUpdating();
      void run();
  };
#endif
