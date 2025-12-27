#ifndef WEATHER_H
#define WEATHER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct  weatherJson {
  float min;
  float max;
  char  *description;
};

void  min_max(float tmp, float *min, float *max);
bool parse_weather(JsonDocument& doc, float *min, float *max);
void fetch_weather();

#endif