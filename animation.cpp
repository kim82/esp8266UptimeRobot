#include "animation.h"

Animation::Animation() {
}

void Animation::setup() {
  pinMode(LCD_CE,    OUTPUT);
  pinMode(LCD_RESET, OUTPUT);
  pinMode(LCD_DC,    OUTPUT);
  pinMode(LCD_DIN,   OUTPUT);
  pinMode(LCD_CLK,   OUTPUT);
  
  digitalWrite(LCD_RESET, LOW);
  digitalWrite(LCD_RESET, HIGH);
  
  write(LCD_C, 0x21 );  // LCD Extended Commands.
  write(LCD_C, 0xB9 );  // Set LCD Vop (Contrast). 
  write(LCD_C, 0x04 );  // Set Temp coefficent.
  write(LCD_C, 0x14 );  // LCD bias mode 1:48.
  write(LCD_C, 0x20 );  // LCD Basic Commands
  write(LCD_C, 0x0C );  // LCD in normal mode.
}

void Animation::write(byte dc, byte data) {
  digitalWrite(LCD_DC, dc);
  digitalWrite(LCD_CE, LOW);
  shiftOut(LCD_DIN, LCD_CLK, MSBFIRST, data);
  digitalWrite(LCD_CE, HIGH);
}

void Animation::showImage(const byte *data) {
  int index=0;
  for (index = 0; index < 504; index++){
    write(LCD_D, data[index]);
  }
}

void Animation::blinkEyes() {
  nextBlink = random(5, 10);  //next blink is random (5 to 10 seconds)
  showImage(closedEyes);
  delay(150);
  showImage(normalEyes);
}

void Animation::setNormalMode() {
  currentStatus = NORMAL;
}
void Animation::setOfflineMode() {
  currentStatus = OFFLINE;
}
void Animation::setAlertMode() {
  currentStatus = ALERT;
}

void Animation::run() {
  switch (currentStatus) {
    case ALERT:
      if (lastStatus != ALERT) {
        showImage(confusedEyes);  //refresh the image
      }
      nextBlink = 1;  //refresh immediately after
      break;
      
    case OFFLINE:
      if (lastStatus != OFFLINE) {
        showImage(closedEyes);  //refresh the image
      }
      
      nextBlink = 1;  //refresh immediately after
      break;

    case NORMAL:
      unsigned long timePassed = millis() - timer;
      if (timePassed >= nextBlink*1000) {
        timer = millis();
        blinkEyes();
      }
      break;
  }
  lastStatus = currentStatus;
}
