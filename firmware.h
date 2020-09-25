#ifndef Firmware_H
#define Firmware_H
  #include "Arduino.h"
  #include <ESP8266HTTPClient.h>
  #include <ESP8266WiFi.h>
  #include <ESP8266httpUpdate.h>

  class Firmware {   
    #define FW_VERSION 3
    
    #define FWUrlBase "https://raw.githubusercontent.com/kim82/esp8266UptimeRobot/master/firmware/"
    #define FWVersionFile "esp8266UptimeRobot.version"
    
    #define FWFile "esp8266UptimeRobot_"
    #define FWFileType ".bin"
        
    #define UserAgent "IOT-Device"
    
    private:
      
    public:
      Firmware();
      static boolean checkAndUpdate();
      static void printVersion();
  };
#endif
