#include "plugins/ClockPlugin.h"
#include <bitset>

void ClockPlugin::setup()
{
  // loading screen
  Screen.setPixel(4, 7, 1);
  Screen.setPixel(5, 7, 1);
  Screen.setPixel(7, 7, 1);
  Screen.setPixel(8, 7, 1);
  Screen.setPixel(10, 7, 1);
  Screen.setPixel(11, 7, 1);

  previousMinutes = -1;
  previousHour = -1;
  previousSeconds = -1;
}

void ClockPlugin::loop()
{
  if (getLocalTime(&timeinfo))
  {
    if (/*previousSeconds != timeinfo.tm_sec ||*/ previousMinutes != timeinfo.tm_min || previousHour != timeinfo.tm_hour)
    {
      std::vector<int> hours = {(timeinfo.tm_hour - timeinfo.tm_hour % 10) / 10, timeinfo.tm_hour % 10};
      std::vector<int> minutes = {(timeinfo.tm_min - timeinfo.tm_min % 10) / 10, timeinfo.tm_min % 10};
      //int seconds = timeinfo.tm_sec;

      Screen.lockScreen();

      Screen.clear();
      Screen.drawNumber(2, 2, hours[0]);
      Screen.drawNumber(9, 2, hours[1]);
      Screen.drawNumber(2, 9, minutes[0]);
      Screen.drawNumber(9, 9, minutes[1]);

      /*if(seconds > 45)
      {
        seconds -= 45;
        Screen.setPixel(0, 15-seconds, 1, 128);
      }
      else if(seconds > 30)
      {
        seconds -= 30;
        Screen.setPixel(15-seconds, ROWS-1, 1, 128);
      }
      else if(seconds > 15)
      {
        seconds -= 15;
        Screen.setPixel(COLS-1, seconds, 1, 128);
      }
      else
      {
        Screen.setPixel(seconds, 0, 1, 128);
      }*/
      
      Screen.unlockScreen();
    }

    previousMinutes = timeinfo.tm_min;
    previousHour = timeinfo.tm_hour;
    previousSeconds = timeinfo.tm_sec;
  }
  delay(16);
}

const char *ClockPlugin::getName() const
{
  return "Uhr";
}
