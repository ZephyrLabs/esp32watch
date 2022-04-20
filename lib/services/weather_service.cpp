#include "weather_service.h"
#include "wifi_service.h"
#include "SPIFFS.h"
#include "FS.h"
#include "HTTPClient.h"
#include "string.h"
#include "ArduinoJson.h"

namespace weather_service{
    HTTPClient http;
    StaticJsonDocument<1024> WeatherJson;

    String town;            
    String country;
    String key;
    String endpoint;

    String payload; 
    String temperature;
    String description;

    RTC_DATA_ATTR char icon[4];
    RTC_DATA_ATTR char temp[5];
    RTC_DATA_ATTR char desc[128];

    char weather_icon_path[25] = "/weather_icons/nul.bmp";

    void weather_service_init(){
        fs::File weather_file = SPIFFS.open("/weather.txt");

        country = weather_file.readStringUntil('\n');
        town = weather_file.readStringUntil('\n');
        key = weather_file.readStringUntil('\n');

        endpoint = "http://api.openweathermap.org/data/2.5/weather?q=" 
                      + town + "," + country + "&units=metric&APPID=";
    }

    void weather_service_fetch(){
        if(WiFi.status() == WL_CONNECTED){
            http.begin(endpoint + key);
            int httpCode = http.GET();

            if (httpCode > 0){
                payload = http.getString();
                //Serial.println(payload);
            }

            else{
                Serial.println("Error on HTTP request");
                
                strcpy(icon, "nul");
                strcpy(temp, "0.00");
                strcpy(desc, "unavailable");

                return;
            }

            http.end();
            
            deserializeJson(WeatherJson, payload.c_str());
            
            String temperature = WeatherJson["main"]["temp"];
            String description = WeatherJson["weather"][0]["description"];
            String icon_id = WeatherJson["weather"][0]["icon"];

            memcpy(icon, icon_id.c_str(), 4);
            memcpy(temp, temperature.c_str(), 5);
            memcpy(desc, description.c_str(), 128);

            Serial.println("Temperature: " + String(temperature) + " C");
            Serial.println("Weather: " + description);
        }

        else{
            Serial.println("Not connected to the network!");
        }
    }

    String weather_icon(){
        sprintf(weather_icon_path, "/weather_icons/%c%c%c.bmp", icon[0], icon[1], icon[2]);
        return String(weather_icon_path);
    }

    String weather_desc(){
        return(String(desc));
    }

    String weather_temp(){
        return(String(temp));
    }
}
