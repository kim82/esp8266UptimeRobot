#include "firmware.h"

void Firmware::printVersion() {
  Serial.print("Current firmware version: ");
  Serial.println(FW_VERSION);
}

boolean Firmware::checkAndUpdate() {
  String fwURL = FWUrlBase;
  String fwVersionURL = fwURL;
  fwVersionURL.concat(FWVersionFile);

  Serial.println("Checking for firmware updates.");
  Serial.print("Firmware version URL: ");
  Serial.println(fwVersionURL);

  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  
  HTTPClient httpClient;
  httpClient.setUserAgent(UserAgent);
  httpClient.begin( client, fwVersionURL);
  int httpCode = httpClient.GET();
  
  if( httpCode == 200 ) {
    String newFWVersion = httpClient.getString();

    Firmware::printVersion();
    Serial.print("Available firmware version: ");
    Serial.println(newFWVersion);

    int newVersion = newFWVersion.toInt();
    if( newVersion > FW_VERSION ) {
      Serial.println("Preparing to update");

      String fwImageURL = fwURL;
      fwImageURL.concat(FWFile);
      fwImageURL.concat(newFWVersion);
      fwImageURL.concat(FWFileType);
      
      Serial.print("UPDATING: ");
      Serial.println(fwImageURL);
      
      t_httpUpdate_return ret = ESPhttpUpdate.update(client, fwImageURL);
      
      switch(ret) {
        case HTTP_UPDATE_FAILED:
          Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
          break;

        case HTTP_UPDATE_NO_UPDATES:
          Serial.println("HTTP_UPDATE_NO_UPDATES");
          break;

        case HTTP_UPDATE_OK:
          return true;
      }
    }
    else {
      Serial.println("Already on latest version");
    } 
  }
  else {
    Serial.println("Firmware check failed");
  }

  return false;
}
