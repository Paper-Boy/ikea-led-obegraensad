#pragma once

#include "PluginManager.h"

class SnowPlugin : public Plugin
{
private:
  static const uint8_t NUM_FLAKES = 25;
  static const uint8_t X_MAX = 16;
  static const uint8_t Y_MAX = 16;
  static const uint8_t LED_TYPE_OFF = 0;
  static const uint8_t LED_TYPE_ON = 1;

  int lastFlakeMillis = 0;

  struct Flake
  {
    unsigned char x;
    unsigned char y;
    int brightness;
    bool visible;
    int lastMoveMillis = 0;
  };

  Flake flakes[SnowPlugin::NUM_FLAKES];

public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};
