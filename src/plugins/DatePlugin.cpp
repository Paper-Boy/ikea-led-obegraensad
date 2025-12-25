#include "plugins/DatePlugin.h"

void DatePlugin::setup()
{
  // loading screen
  Screen.setPixel(4, 7, 1);
  Screen.setPixel(5, 7, 1);
  Screen.setPixel(7, 7, 1);
  Screen.setPixel(8, 7, 1);
  Screen.setPixel(10, 7, 1);
  Screen.setPixel(11, 7, 1);

  previousDay = -1;
  previousMonth = -1;

  // this->lastUpdate = millis();
}

/*
Screen.drawCalendarLetter(0, 1, 'N', 255);
Screen.drawCalendarLetter(5, 6, 'O', 255);
Screen.drawCalendarLetter(10, 11, 'V', 255);  
*/

void DatePlugin::loop()
{
  if (getLocalTime(&timeinfo))
  {
    if (previousDay != timeinfo.tm_mday || previousMonth != timeinfo.tm_mon)
    {
      int monthInteger = timeinfo.tm_mon + 1; // tm_mon is 0-11, we need 1-12

      std::vector<int> day = {(timeinfo.tm_mday - timeinfo.tm_mday % 10) / 10, timeinfo.tm_mday % 10};
      std::vector<int> month = {(monthInteger - monthInteger % 10) / 10, monthInteger % 10};

      if (day[0] == 0)
      {
        day[0] = 10; // blank
      }
      
      if(month[0] == 0)
      {
        month[0] = 10; // blank
      }

      Screen.lockScreen();

      Screen.clear();
      
      Screen.drawNumbers(2, 2, day);
      Screen.drawNumbers(2, 9, month); 
      /*Screen.drawCalendarLetter(0, 1, monthLetters[counter][0], 255);
      Screen.drawCalendarLetter(5, 6, monthLetters[counter][1], 255);
      Screen.drawCalendarLetter(10, 11, monthLetters[counter][2], 255);*/


      Screen.setPixel(12, 6, 1);
      Screen.setPixel(12, 13, 1);

      Screen.unlockScreen();

      /*lastUpdate = millis();
      counter++;
      if(counter >= monthLetters.size())
      {
        counter = 0;
      }*/
    }

    previousDay = timeinfo.tm_mday;
    previousMonth = timeinfo.tm_mon;
  }
  delay(16);
}

const char *DatePlugin::getName() const
{
  return "Datum";
}
