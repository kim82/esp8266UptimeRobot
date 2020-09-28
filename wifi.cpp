#include "wifi.h"

Wifi::Wifi() {
}

void Wifi::setup() {
  //read from eeprom
  readFromEeprom();
  char intervalChar[16];
  itoa(settings.interval, intervalChar, 10);  //converts int to char

  //setup wifi manager
  WiFiManager wifiManager;
  WiFiManagerParameter custom_api_key("API Key", "UptimeRobot API Key", settings.api_key, 64);
  WiFiManagerParameter custom_interval("Interval", "Check interval (Minutes)", intervalChar, 4);
  
  wifiManager.addParameter(&custom_api_key);
  wifiManager.addParameter(&custom_interval);
  wifiManager.setTimeout(300);      //timeout in seconds
  wifiManager.autoConnect("UptimeRobotAP");
  
  //saves updated parameters
  writeToEeprom(custom_api_key.getValue(), custom_interval.getValue());

  Serial.print("local ip:");
  Serial.println(WiFi.localIP());
  Serial.print("api:");
  Serial.println(settings.api_key);
  Serial.print("interval:");
  Serial.println(settings.interval);
}

void Wifi::failedConnection() {
  Serial.println("Failed connection");
  if (connectionFailCount < 100)
      connectionFailCount++;
}

bool Wifi::checkUptimeRobot() {
  const int API_TIMEOUT = 15000;  //keep it long if you want to receive headers from client
  Serial.print("Connecting..");
  
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(API_TIMEOUT);
  
  int retries = 6;
  while(!client.connect("api.uptimerobot.com", 443) && (retries-- > 0)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  if(!client.connected()) {
    failedConnection();
    Serial.println("Failed to connect, going back to sleep");
    client.stop();
    return true;
  }
  
  String data = "api_key=" + String(settings.api_key) + "&statuses=9&limit=1";
  client.println("POST /v2/getMonitors HTTP/1.1");
  client.println("Host: api.uptimerobot.com");
  client.println("Cache-Control: no-cache");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(data.length());
  client.println("");
  client.println(data);
  client.println("Connection: close\r\n");
  
  int timeout = 5 * 10; // 5 seconds             
  while(!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!client.available()) {
    failedConnection();
    Serial.println("No response, going back to sleep");
    client.stop();

    return true;
  }
  
  while(client.available()){
    String line = client.readStringUntil('\n');
    int OK = line.indexOf("\"stat\":\"ok\"");
    if (OK > -1) {
      connectionFailCount = 0;
      
      int a = line.indexOf("\"total\":"); //looks for the total:xx entry in the json
      if (a > -1) {
        int b = line.indexOf("}", a);
  
        String total = line.substring(a + 8, b);
        totalDownCount = total.toInt();
        Serial.print("TOTAL DOWN: ");
        Serial.println(totalDownCount);
        
        break;  //stops the reading
      }
    }
    else {
      failedConnection();
    }
  }
  Serial.println("closing connection");
  //delay(1000);
  client.stop();
  Serial.println("UPTIME CHECKED");  

  return true;
}

void Wifi::resetAll() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.restart();
}

void Wifi::readFromEeprom() {
  EEPROM.begin(512);
  EEPROM.get(0, settings);
  EEPROM.end();

  if (settings.ver != EEPROM_VERSION) {
    strcpy(settings.api_key, "");
    settings.interval = 5;
  }
}

void Wifi::writeToEeprom(const char *api_key, const char *interval) {
  if (strlen(api_key) != 0) {
    strcpy(settings.api_key, api_key);
    settings.ver = EEPROM_VERSION;
    settings.interval = atoi(interval);
    
    EEPROM.begin(512);
    EEPROM.put(0, settings);
    EEPROM.commit();
    EEPROM.end();
  }
}
