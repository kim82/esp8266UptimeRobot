#include "button.h"
#include "wifi.h"
#include "animation.h"
#include "firmware.h"
#include <arduino-timer.h>

#define LED1_PIN      2   //D4
#define LED2_PIN      16  //D0
boolean ledStatus;

auto timer = timer_create_default();

Animation animation;
Wifi      wifi;

void setup() {
  Serial.begin(115200);
  Firmware::printVersion();
  
  //setup leds
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  animation.setup();
  animation.run();
  ledOn();
  
  wifi.setup();

  animation.setNormalMode();
  ledOff();
  setupButton();

  timer.every(1000 * 60 * wifi.settings.interval, checkUptimeRobot_timerHdl);  //every x minutes
  checkUptimeRobot_timerHdl(0);
}
bool checkUptimeRobot_timerHdl(void *) {
  wifi.checkUptimeRobot();
}

void ledBlink() {
  ledStatus = !ledStatus;
  digitalWrite(LED1_PIN, ledStatus); 
  digitalWrite(LED2_PIN, !ledStatus); 
  delay(100);
}

void ledOn() {
  digitalWrite(LED1_PIN, HIGH); 
  digitalWrite(LED2_PIN, HIGH); 
}

void ledOff() {
  digitalWrite(LED1_PIN, LOW); 
  digitalWrite(LED2_PIN, LOW); 
}

void loop() {
  timer.tick();

  boolean beginReset = false;
  boolean beginUpdate = false;
  switch(checkButton()) {
    case 1: //button clicked
      wifi.totalDownCount = 0;
      break;
    case 2: //button released after 5 sec or more
      wifi.resetAll();
      break;
    case 3:
      ledOn();
      Firmware::checkAndUpdate();
      break;
      
    case 20: //button held down more than 5 sec
      beginReset = true;
      ledOn();
      animation.setOfflineMode();
      wifi.totalDownCount = 0;
      break;

    case 30:
      beginUpdate = true;
      break;
  }
  
  if (beginUpdate) {
    ledBlink();
    animation.setUpdating();
  }
  else if (!beginReset) {
    if (wifi.totalDownCount > 0) {
      ledBlink();
      animation.setAlertMode();
    }
    else {
      ledOff();
      animation.setNormalMode();
    }
  }
  animation.run();
}
