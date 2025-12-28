#include "plugins/ClockPlugin.h"
#include <bitset>

void ClockPlugin::setup()
{
  if(firstTime)
  {
    // loading screen
    Screen.lockScreen();
    Screen.clear();
    Screen.setPixel(4, 7, 1);
    Screen.setPixel(5, 7, 1);
    Screen.setPixel(7, 7, 1);
    Screen.setPixel(8, 7, 1);
    Screen.setPixel(10, 7, 1);
    Screen.setPixel(11, 7, 1);
    Screen.unlockScreen();
  }

  previousMinutes = -1;
  previousHour = -1;
  previousSeconds = -1;

  firstTime = false;
}

void ClockPlugin::loop()
{
  if (getLocalTime(&timeinfo))
  {
    if (previousMinutes != timeinfo.tm_min || previousHour != timeinfo.tm_hour)
    {
      std::vector<int> hours = {(timeinfo.tm_hour - timeinfo.tm_hour % 10) / 10, timeinfo.tm_hour % 10};
      std::vector<int> minutes = {(timeinfo.tm_min - timeinfo.tm_min % 10) / 10, timeinfo.tm_min % 10};

      Screen.lockScreen();

      Screen.clear();
      Screen.drawNumber(2, 2, hours[0]);
      Screen.drawNumber(9, 2, hours[1]);
      Screen.drawNumber(2, 9, minutes[0]);
      Screen.drawNumber(9, 9, minutes[1]);
      
      Screen.unlockScreen();
    }

    previousMinutes = timeinfo.tm_min;
    previousHour = timeinfo.tm_hour;
    previousSeconds = timeinfo.tm_sec;
  }
  else
  {
    Serial.println("Failed to obtain time");
    // loading screen
    Screen.lockScreen();
    Screen.clear();
    Screen.setPixel(4, 7, 1);
    Screen.setPixel(5, 7, 1);
    Screen.setPixel(7, 7, 1);
    Screen.setPixel(8, 7, 1);
    Screen.setPixel(10, 7, 1);
    Screen.setPixel(11, 7, 1);
    Screen.unlockScreen();
  }
  delay(16);
}

const char *ClockPlugin::getName() const
{
  return "Uhr";
}
