#include <Arduino.h>
#include <TFT_eSPI.h>

#include "FS.h"
#include "SPIFFS.h"

#include "serial_init.h"
#include "spiffs_init.h"
#include "wifi_service.h"
#include "time_service.h"
#include "weather_service.h"
#include "battery_service.h"
#include "screen_service.h"
#include "sync_service.h"

#include "watchface.h"

RTC_DATA_ATTR bool cold_boot = true;

using namespace wifi_service;
using namespace time_service;
using namespace weather_service;

TFT_eSPI tft = TFT_eSPI();

void setup() {
  setCpuFrequencyMhz(160);

  serial_init::serial_init();
  spiffs_init::spiffs_init();

  // automatic timed sync:
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER){
        wifi_service::wifi_service_config_init("/wpa.txt");
        if(wifi_service::wifi_service_connect()){
            Serial.println("connected to wifi");
        }
        else{
            Serial.println("could not connect to wifi");
        }

        sync_service::sync_service();

        wifi_service::wifi_service_disconnect();

        esp_sleep_enable_gpio_wakeup();
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_sleep_enable_timer_wakeup(time_service::time_to_sync() * 60 * 1000000);
        
        Serial.println("synced and entering deep sleep...");

        esp_deep_sleep_start();
  }
  
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(4);
  
  screen_service::tft_bl_init();
  time_service::time_service_init();

  if(cold_boot){
      screen_service::tft_splash_screen();
      
      wifi_service::wifi_service_config_init("/wpa.txt");
      if(wifi_service::wifi_service_connect()){
          Serial.println("connected to wifi");
      }
      else{
          Serial.println("could not connect to wifi");
      }

      sync_service::sync_service();

      wifi_service::wifi_service_disconnect();
      
      cold_boot = false;
  }

  Serial.printf("local time: %d:%d\n", time_service::time_hour_local(), 
                                       time_service::time_minute_local());

  watchface::watchface_init();
}

void loop() {}
