#include "main.h"

bool fetch_json(const char* url, JsonDocument& doc) {
  HTTPClient http;
  http.begin(url);

  int code = http.GET();
  if (code != HTTP_CODE_OK) {
    http.end();
    return false;
  }
  DeserializationError err = deserializeJson(doc, http.getStream());
  http.end();

  return !err;
}

bool parse_weather(JsonDocument& doc, WeatherData& out) {
  out.min = 100.0f;
  out.max = -100.0f;
  JsonArray list = doc["list"];

  if (list.isNull()) return false;

  for (JsonObject item : list) {
    float tmp = item["main"]["temp_min"];
    if (tmp < out.min) out.min = tmp;
    if (tmp > out.max) out.max = tmp;
  }
  return true;
}