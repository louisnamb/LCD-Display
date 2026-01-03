#include "main.h"

void  epoch_conversion(unsigned long epoch)
{
  time_t rawtime = epoch; 
  struct tm *ts;
  char buf[80];

  // Convert epoch time to local time structure
  ts = localtime(&rawtime);

  // Format time into a string: "Wed 2010-01-01 12:00:00 GMT" (example output format)
  // The format string can be customized. 
  // %a: Abbreviated weekday name
  // %m: Month (01-12)
  // %d: Day of the month (01-31)
  // %H: Hour in 24h format (00-23)
  // %M: Minute (00-59)
  // %S: Second (00-59)
  // %Z: Time zone name/abbreviation
  strftime(buf, sizeof(buf), "%a %H:%M:%S %Z", ts);
  Serial.printf("Formatted time: %s\n", buf);
    return ;
}

bool parse_map(JsonDocument& doc, MapData& time) {
  time.epoch = 0;
  char  *endptr;
  unsigned long epoch_num = 0;
  char example[11];
  JsonArray deliveries = doc["StopMonitoringDelivery"];
  if (deliveries.isNull() || deliveries.size() == 0) {
    Serial.println("deliveries is null!");
    return false;
  }
  JsonArray visits = deliveries[0]["MonitoredStopVisit"];
  if (visits.isNull() || visits.size() == 0) {
    Serial.println("visits is null!");
    return false;
  }
  JsonObject journey =
    visits[0]["MonitoredVehicleJourney"];
  if (journey.isNull()) {
    Serial.println("journey is null!");
    return false;
  }
  JsonObject call = journey["MonitoredCall"];
  if (call.isNull()) {
    Serial.println("call is null!");
    return false;
  }
  const char* ts = call["AimedArrivalTime"];
  if (!ts) {
    Serial.println("ts is null!");
    return false;
  }
  const char* start = strchr(ts, '(') + 1;
  if (!start) return false;
  strlcpy(example, start, sizeof(example));
  time.epoch = strtoul(example, &endptr, 10);
  epoch_conversion(time.epoch + 37800); // add 37800 to convert to Adelaide timezone
  return true;
}
