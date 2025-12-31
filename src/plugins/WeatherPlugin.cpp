#include "plugins/WeatherPlugin.h"

#define UPDATE_INTERVAL_MINUTES 10

// https://github.com/chubin/wttr.in/blob/master/share/translations/en.txt
#ifdef ESP8266
WiFiClient wiFiClient;
#endif

void WeatherPlugin::setup()
{
    currentStatus = LOADING;
    
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
    
    if(lastTemperature != -1 && lastWeatherCode != -1)
    {
        this->drawWeatherDWD();
    }
    else
    {
        this->update();
    }
    
    currentStatus = NONE;
}

void WeatherPlugin::loop()
{
    if((lastTemperature != -1 || lastWeatherCode != -1) && millis() < this->lastUpdate + (1000 * 20)) // Update every 20 seconds, when no weather data is stored
    {
        Serial.println("updating weather");
        this->update();
        this->lastUpdate = millis();
    }
    else if (millis() >= this->lastUpdate + (1000 * 60 * UPDATE_INTERVAL_MINUTES)) // update every 10 minutes
    {
        Serial.println("updating weather");
        this->update();
        this->lastUpdate = millis();
    }
}

bool WeatherPlugin::readWeatherDataDWD()
{
    if(getLocalTime(&timeinfo) == false)
    {
        Serial.println("Failed to obtain time");
        return false;
    }
    int index = timeinfo.tm_hour;

#ifdef ESP32
    http.begin(weatherAPIString + stationID);
#endif
#ifdef ESP8266
    http.begin(wiFiClient, weatherAPIString + stationID);
#endif
    http.setTimeout(2000);
    int code = http.GET();

    if (code == HTTP_CODE_OK)
    {
        DynamicJsonDocument doc(20480);

        DeserializationError err;

        String payload = http.getString();
        err = deserializeJson(doc, payload);

        http.end();

        if (err)
        {
            Serial.print(F("deserializeJson() error: "));
            Serial.println(err.f_str());
        }

        // Validate JSON structure and array bounds before accessing
        if (!doc.containsKey(stationID) || !doc[stationID].containsKey("forecast1") ||
            !doc[stationID]["forecast1"].containsKey("temperature") || !doc[stationID]["forecast1"].containsKey("icon"))
        {
            Serial.println("Missing expected keys in JSON response");
            return false;
        }

        JsonArray temps = doc[stationID]["forecast1"]["temperature"].as<JsonArray>();
        JsonArray icons = doc[stationID]["forecast1"]["icon"].as<JsonArray>();
        if (index < 0 || index >= (int)temps.size() || index >= (int)icons.size())
        {
            Serial.println("JSON arrays shorter than expected or index out of range");
            return false;
        }

        lastTemperature = round(temps[index].as<float>() / 10.0f);
        lastWeatherCode = icons[index].as<int>();

        return true;
    }
    else
    {
        Serial.println("HTTP request failed with code: " + String(code));
        http.end();
        return false;
    }
}

void WeatherPlugin::drawWeatherDWD()
{
    if(lastTemperature == -1 || lastWeatherCode == -1) 
    {
        Serial.println("No weather data to draw");
        return;
    }

    int temperature = lastTemperature;
    int weatherCode = lastWeatherCode;

    // Fallback values if no icon can be found
    int weatherIcon = 7;
    int iconY = 1;
    int tempY = 10;

    if (std::find(thunderCodes.begin(), thunderCodes.end(), weatherCode) != thunderCodes.end())
    {
        weatherIcon = 1;
    }
    else if (std::find(rainCodes.begin(), rainCodes.end(), weatherCode) != rainCodes.end())
    {
        weatherIcon = 4;
    }
    else if (std::find(snowCodes.begin(), snowCodes.end(), weatherCode) != snowCodes.end())
    {
        weatherIcon = 5;
    }
    else if (std::find(fogCodes.begin(), fogCodes.end(), weatherCode) != fogCodes.end())
    {
        weatherIcon = 6;
        iconY = 2;
    }
    else if (std::find(clearCodes.begin(), clearCodes.end(), weatherCode) != clearCodes.end())
    {
        weatherIcon = 2;
        iconY = 1;
        tempY = 9;
    }
    else if (std::find(cloudyCodes.begin(), cloudyCodes.end(), weatherCode) != cloudyCodes.end())
    {
        weatherIcon = 0;
        iconY = 2;
        tempY = 9;
    }
    else if (std::find(partlyCloudyCodes.begin(), partlyCloudyCodes.end(), weatherCode) != partlyCloudyCodes.end())
    {
        weatherIcon = 3;
        iconY = 2;
    }

    Screen.lockScreen();

    Screen.clear();
    Screen.drawWeather(0, iconY, weatherIcon);

    if (temperature >= 10)
    {
        Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4);
        Screen.drawNumbers(1, tempY, {(temperature - temperature % 10) / 10, temperature % 10});
    }
    else if (temperature <= -10)
    {
        Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
        Screen.drawCharacter(11, tempY, Screen.readBytes(degreeSymbol), 4);
        temperature *= -1;
        Screen.drawNumbers(3, tempY, {(temperature - temperature % 10) / 10, temperature % 10});
    }
    else if (temperature >= 0)
    {
        Screen.drawCharacter(7, tempY, Screen.readBytes(degreeSymbol), 4);
        Screen.drawNumbers(4, tempY, {temperature});
    }
    else
    {
        Screen.drawCharacter(0, tempY, Screen.readBytes(minusSymbol), 4);
        Screen.drawCharacter(9, tempY, Screen.readBytes(degreeSymbol), 4);
        Screen.drawNumbers(3, tempY, {-temperature});
    }

    Screen.unlockScreen();
}

void WeatherPlugin::update()
{
    if(!readWeatherDataDWD())
    {
        Serial.println("Failed to read weather data");
        return;
    }

    drawWeatherDWD();
}

const char *WeatherPlugin::getName() const
{
    return "Wetter (" WEATHER_LOCATION ")";
}

