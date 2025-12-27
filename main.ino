#include <WiFi.h>
#include "weather.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *ssid_Router = "DN8245V-2DB5";
const char *password_Router = "MAS972ARLM";

void Wifi_setup() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid_Router, password_Router);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED && counter < 20) {
    delay(500);
    Serial.print(".");
    counter++;
  }
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed.");
  }
}

void setup() {
  Serial.begin(115200);
  while(!Serial); 
  for(int i = 0; i < 10; i++) {
    Serial.println(">>> TESTING SERIAL CONNECTION <<<");
    delay(500);
  }
  Wifi_setup();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    fetch_weather();
  }
  delay(30000); // Wait 30 seconds between updates
}