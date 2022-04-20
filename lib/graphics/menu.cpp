#include "Arduino.h"
#include "serial_init.h"
#include "spiffs_init.h"
#include "time_service.h"
#include "battery_service.h"
#include "wifi_service.h"
#include "weather_service.h"
#include "screen_service.h"
#include "sync_service.h"

#include "menu.h"

#include "FS.h"
#include "SPIFFS.h"
#include "bmp.h"
#include "TFT_eSPI.h"

namespace menu{
    TFT_eSPI tft = TFT_eSPI();

    void menu_init(){
        tft.setRotation(4);
    }

    void status_bar(){
        bmp::drawBmp(battery_service::battery_icon().c_str(), 99, 0, 0x0000);
        bmp::drawBmp(wifi_service::wifi_service_icon().c_str(), 72, 0, 0x0000);
    }

    void menu_1(){
        bmp::drawBmp("/wall.bmp", 0, 0);
        vTaskDelay(25/portTICK_RATE_MS);

        int awake = millis();

        status_bar();

        tft.drawString(weather_service::weather_temp() + " C", 5, 110);
        tft.drawString(weather_service::weather_desc(), 5, 120);

        tft.fillCircle(26, 70, 16, 0x0000);
        bmp::drawBmp(weather_service::weather_icon().c_str(), 10, 54, 0x0000);


        tft.fillCircle(100, 70, 32, 0x0000);
        bmp::drawBmp(time_service::globe_icon_path().c_str(), 70, 40, 0x0000);

        for(;;){
            String hour = String(time_service::time_hour());
            String minute = String(time_service::time_minute());
            String second = String(time_service::time_second());

            if(time_service::time_hour() <= 9){
                hour = "0" + String(time_service::time_hour());   
            }
            if(time_service::time_minute() <= 9){
                minute = "0" + String(time_service::time_minute());   
            }
            if(time_service::time_second() <= 9){
                second = "0" + String(time_service::time_second());   
            }

            tft.drawString("utc time: " + hour + ":" + minute + ":" + second, 5, 140);

            hour = String(time_service::time_hour_local());
            minute = String(time_service::time_minute_local());

            if(time_service::time_hour_local() <= 9){
                hour = "0" + String(time_service::time_hour_local());   
            }
            if(time_service::time_minute_local() <= 9){
                minute = "0" + String(time_service::time_minute_local());   
            }

            tft.drawString("local time: " + hour + ":" + minute + ":" + second, 5, 150);

            tft.drawString("next sync in: " + String(time_service::time_to_sync()) + " mins", 5, 160);

            if(analogRead(35) == 0){
                return;
            }
            
            if(millis() - awake > 3000){
                vTaskDelay(100/portTICK_RATE_MS);
                return;
            }
            
            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }

    void menu_2(){
        bmp::drawBmp("/wall.bmp", 0, 0);
        
        int bl = screen_service::tft_bl_get();
        int percent = bl / 2;

        long awake = millis();

        tft.drawRoundRect(50, 70, 35, 104, 1, 0xFFFF);
        tft.fillRect(51, 71, 33, 102, 0x0000);
        tft.fillRect(52, 72 + 100 - percent, 31, percent, 0xFFFF);
        vTaskDelay(25/portTICK_RATE_MS);

        tft.fillRect(50, 187, 35, 14, 0x0000);
        tft.drawString(String(percent) + " %", 55, 190);

        while(analogRead(0) == 0){
            continue;
        }
        
        vTaskDelay(100/portTICK_PERIOD_MS);

        for(;;){
            if(analogRead(0) == 0){
                if(percent > 0){
                    percent -= 1;

                    tft.fillRect(52, 72 + 99 - percent, 31, 1, 0x0000);
                    vTaskDelay(25/portTICK_RATE_MS);

                    tft.fillRect(50, 187, 35, 14, 0x0000);
                    tft.drawString(String(percent) + " %", 55, 190);

                    awake = millis();
                }
            }

            if(analogRead(35) == 0){
                if(percent < 100){
                    percent += 1;

                    tft.fillRect(52, 72 + 100 - percent, 31, 1, 0xFFFF);
                    vTaskDelay(25/portTICK_RATE_MS);

                    tft.fillRect(50, 187, 35, 14, 0x0000);
                    tft.drawString(String(percent) + " %", 55, 190);

                    awake = millis();
                }
            }

            bl = percent * 2;
            screen_service::tft_bl_set(bl);

            if(millis() - awake > 3000){
                vTaskDelay(100/portTICK_RATE_MS);
                return;
            }

            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }

    void menu_3(){
        bmp::drawBmp("/wall.bmp", 0, 0);
        status_bar();

        tft.drawString("Press the right", 10, 60);
        tft.drawString("button to sync", 10, 70);
        tft.drawString("with the network", 10, 80);
        tft.fillRect(10, 100, 115, 7, 0x0000);

        long awake = millis();
    
        for(;;){
            if(analogRead(35) == 0){
                tft.fillRect(10, 100, 115, 7, 0x0000);
                tft.drawString("syncing...", 10, 100);
                
                wifi_service::wifi_service_config_init("/wpa.txt");
                if(wifi_service::wifi_service_connect()){
                    Serial.println("connected to wifi");
                }
                else{
                    Serial.println("could not connect to wifi");
                    tft.drawString("could not connect!", 10, 100);
                    return;
                }
                
                status_bar();

                if(sync_service::sync_service()){
                    tft.drawString("syncing successful!", 10, 100);
                }
                else{
                    tft.drawString("syncing unsuccessful!", 10, 100);
                }

                wifi_service::wifi_service_disconnect();
                
                awake = millis();
            }

            if((analogRead(0) == 0)){
                return;
            }
             
            if(millis() - awake > 3000){
                vTaskDelay(100/portTICK_RATE_MS);
                return;
            }

            vTaskDelay(100/portTICK_PERIOD_MS);
        }
    }
}