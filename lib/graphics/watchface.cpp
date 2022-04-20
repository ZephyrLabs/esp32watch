#include "watchface.h"

#include "Arduino.h"
#include "serial_init.h"
#include "spiffs_init.h"
#include "time_service.h"
#include "battery_service.h"
#include "wifi_service.h"
#include "weather_service.h"
#include "screen_service.h"
#include "sync_service.h"

#include "FS.h"
#include "SPIFFS.h"
#include "bmp.h"
#include "TFT_eSPI.h"

#include "menu.h"

using namespace wifi_service;
using namespace weather_service;

#define L_BTN 0
#define R_BTN 35

namespace watchface{
    TFT_eSPI tft = TFT_eSPI();

    bool awake_status = true;
    int menu = 0;
    
    void watchface_menu(){

        long awake = millis();

        bmp::drawBmp("/wall.bmp", 0, 0);
        bmp::drawBmp(weather_service::weather_icon().c_str(), 102, 45, 0x0000);

        int hour_1 = time_service::time_hour_local() / 10;
        int hour_2 = time_service::time_hour_local() % 10;

        int minute_1 = time_service::time_minute_local() / 10;
        int minute_2 = time_service::time_minute_local() % 10;     

        bmp::drawBmp(get_num_glyph(hour_1).c_str(), 4, 20, 0x0000);
        bmp::drawBmp(get_num_glyph(hour_2).c_str(), 40, 20, 0x0000);
        bmp::drawBmp(get_num_glyph(minute_1).c_str(), 4, 80, 0x0000);
        bmp::drawBmp(get_num_glyph(minute_2).c_str(), 40, 80, 0x0000);

        for(;;){
            menu::status_bar();

            if((analogRead(0) == 0) || (analogRead(35) == 0)){
                return;
            }

            if(millis() - awake > 3000){
                vTaskDelay(100/portTICK_RATE_MS);
                return;
            }
        }
       //tft.drawString(String(battery_service::battery_service_left()) + " %" , 54, 200);
    }

    void watchface_init(){
        tft.setRotation(4);
        tft.fillScreen(TFT_BLACK);

        /* GPIO setup */        
        esp_sleep_enable_gpio_wakeup();
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_35, ESP_EXT1_WAKEUP_ALL_LOW);
        esp_sleep_enable_timer_wakeup(time_service::time_to_sync() * 60 * 1000000);

        pinMode(0, INPUT);
        pinMode(35, INPUT);

        /* counters and state management */
        long awake = millis();

        /*initialize the menu system*/
        menu::menu_init();

        watchface_menu();

        for(;;){
            if(analogRead(0) == 0){
                while (analogRead(0) == 0){
                    continue;
                }

                screen_service::tft_wake();
                
                menu++;
                if(menu > 3){
                    menu = 0;
                }

                if(menu == 0)watchface_menu();
                if(menu == 1)menu::menu_1();
                if(menu == 2)menu::menu_2();
                if(menu == 3)menu::menu_3();

                awake = millis(); 
            }

            if(analogRead(35) == 0){
                while (analogRead(35) == 0){
                    continue;
                }

                screen_service::tft_wake();
                
                menu--;
                if(menu < 0){
                    menu = 3;
                }

                if(menu == 0)watchface_menu();
                if(menu == 1)menu::menu_1();
                if(menu == 2)menu::menu_2();
                if(menu == 3)menu::menu_3();

                awake = millis(); 
            }
            
            menu::status_bar();

            if((millis() - awake) > 15000){
                tft.fillScreen(TFT_BLACK);
                vTaskDelay(100/portTICK_RATE_MS);
                screen_service::tft_sleep();            
                awake_status = false;
                Serial.println("entering deep sleep...");
                vTaskDelay(100/portTICK_RATE_MS);
                esp_deep_sleep_start();
            }
            
            if((millis() - awake) > 2000){
                tft.fillScreen(TFT_BLACK);
                vTaskDelay(100/portTICK_RATE_MS);
                screen_service::tft_sleep();            
                awake_status = false;
                vTaskDelay(100/portTICK_PERIOD_MS);
                //esp_light_sleep_start();
            }

            vTaskDelay(100/portTICK_RATE_MS);
        }
    }

    String get_num_glyph(int n){
        String path = "/glyph/" + String(n) + ".bmp";
        return path;
    }
}
