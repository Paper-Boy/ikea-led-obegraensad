#pragma once

#include "PluginManager.h"

class DatePlugin : public Plugin
{
private:
  bool firstTime = true;

  struct tm timeinfo;

  int previousDay;
  int previousMonth;

  font currentFont = fonts[0];

  // just for testing
  unsigned long lastUpdate = 0;
  unsigned int counter = 0;

  std::vector<std::vector<char>> monthLetters = {
      {'J', 'A', 'N'},
      {'F', 'E', 'B'},
      {'M', 'A', 'R'},
      {'A', 'P', 'R'},
      {'M', 'A', 'I'},
      {'J', 'U', 'N'},
      {'J', 'U', 'L'},
      {'A', 'U', 'G'},
      {'S', 'E', 'P'},
      {'O', 'K', 'T'},
      {'N', 'O', 'V'},
      {'D', 'E', 'Z'}
  };

public:
  void setup() override;
  void loop() override;
  const char *getName() const override;
};