#ifndef Wifi_H
#define Wifi_H
  #include "Arduino.h"
  #include <EEPROM.h>
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
  
  class Wifi {
    #define EEPROM_VERSION 1304
    
    private:
      void readFromEeprom();
      void writeToEeprom(const char *api_key, const char *interval);
      void failedConnection();
    public:
      typedef struct
      {
        int  ver = EEPROM_VERSION;
        char api_key[64]  = "";
        int interval = 5;
      } WMSettings;
    
      WMSettings settings;
      int totalDownCount;
      int connectionFailCount = 0;
      
      Wifi();
      void setup();
      bool checkUptimeRobot();
      void resetAll();
  };
#endif
