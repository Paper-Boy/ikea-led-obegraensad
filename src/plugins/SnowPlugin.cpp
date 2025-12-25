#include "plugins/SnowPlugin.h"

void SnowPlugin::setup()
{
  Screen.clear();
  for (byte i = 0; i < SnowPlugin::NUM_FLAKES; i++)
    for (unsigned char i = 0; i < SnowPlugin::NUM_FLAKES; i++)
    {
      this->flakes[i].x = 0;
      this->flakes[i].y = 0;
      this->flakes[i].visible = false;
    }
}

void SnowPlugin::loop()
{
  Screen.lockScreen();

  // draw the flakes
  for (unsigned char i = 0; i < SnowPlugin::NUM_FLAKES; i++)
  {
    // if the flake is not visible, randomly create a new one at the top
    if (!flakes[i].visible)
    {
      if (this->lastFlakeMillis + 300 > millis() || random(10) > 1)
        continue;

      this->flakes[i].x = random(SnowPlugin::X_MAX);
      this->flakes[i].y = 0;
      this->flakes[i].visible = true;
      this->flakes[i].brightness = random(50, 255);
      this->flakes[i].lastMoveMillis = millis();

      Screen.setPixel(this->flakes[i].x, this->flakes[i].y, SnowPlugin::LED_TYPE_ON, this->flakes[i].brightness);

      this->lastFlakeMillis = millis();
    }
    // else move the flake down and to the sides a bit randomly
    else if(this->flakes[i].lastMoveMillis + 400 < millis())
    {
      // erase current position and move flake down
      Screen.setPixel(this->flakes[i].x, this->flakes[i].y, SnowPlugin::LED_TYPE_OFF, 0);

      if(random(10) > 5)
      {
        this->flakes[i].y++;
        this->flakes[i].lastMoveMillis = millis();
      }

      // if flake leaves the screen, mark it as not visible
      if (this->flakes[i].y >= (SnowPlugin::Y_MAX))
      {
        this->flakes[i].visible = false;
        continue;
      }

      // randomly move flake to the side
      if (random(10) > 5)
      {
        int8_t direction = (random(2) == 0) ? -1 : 1;
        int8_t newX = this->flakes[i].x + direction;
        if (newX >= 0 && newX < SnowPlugin::X_MAX)
          this->flakes[i].x = newX;
      }

      // draw new position
      Screen.setPixel(this->flakes[i].x, this->flakes[i].y, SnowPlugin::LED_TYPE_ON, this->flakes[i].brightness);
    }
  }

  Screen.unlockScreen();

  delay(96);
}

const char *SnowPlugin::getName() const
{
  return "Schnee";
}
