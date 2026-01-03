#include "main.h"

StaticJsonDocument<16384> doc;
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
  if (WiFi.status() == WL_CONNECTED) {

  //  if (fetch_json(
  //    "http://api.openweathermap.org/data/2.5/forecast?lat=-34.683331&lon=138.683334&appid=0c09cbd7b4cee723eeb324c9f47e9d25&units=metric",
  //    doc)) {
  //    WeatherData w;
  //    if (parse_weather(doc, w)) {
  //      Serial.println(w.min);
  //      Serial.println(w.max);
  //    }
  //  }
  /*Fix map code so you can get the map data!!*/
    if (fetch_json("http://realtime.adelaidemetro.com.au/sirism/SiriStopMonitoring.svc/json/SM?MonitoringRef=16281", doc)) {
      MapData time;
      if (parse_map(doc, time)) {
        Serial.print("time: ");
        Serial.println(time.epoch);
      }
    }
  }
  delay(30000);
}