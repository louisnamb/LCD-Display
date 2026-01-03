#ifndef MAIN_H
#define MAIN_H

#include <TimeLib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct WeatherData {
  float min;
  float max;
};

struct MapData {
  unsigned long epoch;
//  char *epoch;
};

// Main
bool fetch_json(const char* url, JsonDocument& doc);
// Weather
bool parse_weather(JsonDocument& doc, WeatherData& out);
// Map
bool parse_map(JsonDocument& doc, MapData& time);
#endif
