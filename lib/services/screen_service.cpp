#include <Arduino.h>
#include <FreeRTOS.h>
#include <TFT_eSPI.h>
#include "screen_service.h"
#include "bmp.h"

#define TFT_BL_PIN 4

namespace screen_service{
    TFT_eSPI tft = TFT_eSPI();

    RTC_DATA_ATTR int bl_duty = 10;
    bool tft_wake_status = true;

    void tft_bl_init(){
        ledcSetup(0, 5000, 8);
        ledcAttachPin(TFT_BL_PIN, 0);
        ledcWrite(0, bl_duty);
    }

    void tft_splash_screen(){
        bmp::drawBmp("/splash.bmp", 0, 0);
    }

    int tft_bl_get(){
        return bl_duty;
    }

    void tft_bl_set(int duty){
        bl_duty = duty;
        ledcWrite(0, bl_duty + 5);
    }

    void tft_sleep(){
        ledcWrite(0, 0);
        tft.writecommand(0x10);
        tft_wake_status = false;
    }

    void tft_wake(){
        if(!tft_wake_status){
            tft.writecommand(0x11);
            vTaskDelay(120/portTICK_RATE_MS);
            ledcWrite(0, bl_duty);
            tft_wake_status = true;
        }
    }
}
