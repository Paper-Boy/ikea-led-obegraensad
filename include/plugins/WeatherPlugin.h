#pragma once

#ifdef ESP32
#include <HTTPClient.h>
#endif
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif
#include <ArduinoJson.h>
#include "PluginManager.h"

class WeatherPlugin : public Plugin
{
private:
  unsigned long lastUpdate = 0;
  int lastTemperature = -1;
  int lastWeatherCode = -1;
  struct tm timeinfo;

  StaticJsonDocument<128> filter;

  String weatherAPIString = "https://dwd.api.proxy.bund.dev/v30/stationOverviewExtended?stationIds=";
  String stationID = "H419";

  HTTPClient http;

  std::vector<int> thunderCodes = {-1};
  std::vector<int> cloudyCodes = {4};
  std::vector<int> partlyCloudyCodes = {2, 3};
  std::vector<int> clearCodes = {1};     // 32767 = fallback for "unknown" sky
  std::vector<int> fogCodes = {5};
  std::vector<int> rainCodes = {7, 12, 18, 19};
  std::vector<int> snowCodes = {14};

  bool readWeatherDataDWD();
  void drawWeatherDWD();

public:
  void update();
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
