#include <WiFi.h>
#include "weather.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct weatherJson data = {100.0f, -100.0f, nullptr};

void  min_max(float tmp, float *min, float *max)
{
  if (tmp < *min) *min = tmp;
  if (tmp > *max) *max = tmp;
  return ;
}

bool parse_weather(JsonDocument& doc, float *min, float *max) {
  float temp;
  if (doc["list"].isNull()) return false;
  for (JsonObject item : doc["list"].as<JsonArray>()) {
      temp = item["main"]["temp_min"];
      min_max(temp, min, max);
  }
  return true;
}

void fetch_weather() {
  // 'static' keeps this out of the stack to prevent crashes
  static DynamicJsonDocument doc(16384);
  HTTPClient http;
  DeserializationError error;

  // MUST include http:// protocol
  String url = "http://api.openweathermap.org/data/2.5/forecast?lat=-34.683331&lon=138.683334&appid=0c09cbd7b4cee723eeb324c9f47e9d25&units=metric";
  
  Serial.println("Fetching weather...");
  http.begin(url); 
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
  //  payload.toCharArray(weather_info, 5000);
    error = deserializeJson(doc, http.getStream());
    if (error) { // Failure Checking
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return ;
    }
    parse_weather(doc, &data.min, &data.max);
    Serial.print("min: ");
    Serial.println(data.min);
    Serial.print("max: ");
    Serial.println(data.max);
  } else {
    Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}