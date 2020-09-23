#define PRESSED HIGH
#define NOT_PRESSED LOW

const unsigned long shortPress = 100;
const unsigned long longPress = 5000;  //5 sec
const unsigned long reallyLongPress = 10000;  //10 sec
 
typedef struct Buttons {
    const byte pin = 15; //D8
    const int debounce = 10;
 
    unsigned long counter=0;
    bool prevState = NOT_PRESSED;
    bool currentState;
} Button;
 
// create a Button variable type
Button button;

void setupButton() {
  pinMode(button.pin, INPUT);  
}

void handleShortPress() {
  Serial.println("short");
}
 
void handleLongPress() {
  Serial.println("long");
}

void handleReallyLongPress() {
  Serial.println("really long");
}

byte checkButton() {
  byte ret = 0;
  unsigned long currentMillis = millis();
  unsigned long pressedTime = currentMillis - button.counter;
  
  // check the button
  button.currentState = digitalRead(button.pin);

  //button is held down more than 5 sec and less then 10 sec
  if (button.currentState == PRESSED && pressedTime >= longPress && pressedTime < reallyLongPress) {
    ret = 20;
  }
  //button is held down more than 10 sec
  if (button.currentState == PRESSED && pressedTime >= reallyLongPress) {
    ret = 30;
  }
  
  // has it changed?
  if (button.currentState != button.prevState) {
      delay(button.debounce);
      
      // update status in case of bounce
      button.currentState = digitalRead(button.pin);
      if (button.currentState == PRESSED) {
          // a new press event occured record when button went down
          button.counter = millis();
      }

      if (button.currentState == NOT_PRESSED) {
          // but no longer pressed, how long was it down?
          if ((pressedTime >= shortPress) && !(pressedTime >= longPress) && !(pressedTime >= reallyLongPress)) {
              // short press detected. 
              handleShortPress();
              ret = 1;
          }
          if (pressedTime >= longPress && pressedTime < reallyLongPress) {
              // the long press was detected
              handleLongPress();
              ret = 2;
          }
          if (pressedTime >= reallyLongPress) {
              // the really long press was detected
              handleReallyLongPress();
              ret = 3;
          }
      }
      // used to detect when state changes
      button.prevState = button.currentState;
  }
  return ret;
}
