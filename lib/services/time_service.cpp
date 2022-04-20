#include "time_service.h"
#include "ESPxxTime.h"
#include "SPIFFS.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "WiFi.h"

namespace time_service{
    ESPxxTime rtc;
    int local_hour_offset = 0;
    int local_minute_offset = 0;
    
    RTC_DATA_ATTR int last_time_sync_hour = 0;
    RTC_DATA_ATTR int last_time_sync_minute = 0;
    RTC_DATA_ATTR int last_time_sync_second = 0;
    RTC_DATA_ATTR int last_time_sync_day = 1;
    RTC_DATA_ATTR int last_time_sync_month = 0;
    RTC_DATA_ATTR int last_time_sync_year = 1900;

    void time_service_init(){
        fs::File time_file = SPIFFS.open("/time.txt");

        String hour = time_file.readStringUntil('\n');
        String minute = time_file.readStringUntil('\n');

        local_hour_offset = (int)hour.toInt();
        local_minute_offset = (int)minute.toInt();
    }

    bool time_service_ntp_sync(){
        Serial.printf("last sync: %d:%d:%d %d/%d/%d\n", 
            last_time_sync_hour,
            last_time_sync_minute,
            last_time_sync_second,
            last_time_sync_day,
            last_time_sync_month,
            last_time_sync_year
        );

        if(WiFi.status() == WL_CONNECTED){
            float timezone = 0;
            byte daysavetime = 0;

            configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
            struct tm tmstruct;

            vTaskDelay(2000/portTICK_RATE_MS);
            tmstruct.tm_year = 0;
            
            getLocalTime(&tmstruct, 5000);

            last_time_sync_hour = tmstruct.tm_hour;
            last_time_sync_minute = tmstruct.tm_min;
            last_time_sync_second = tmstruct.tm_sec;
            last_time_sync_day = tmstruct.tm_mday;
            last_time_sync_month = tmstruct.tm_mon + 1;
            last_time_sync_year = tmstruct.tm_year + 1900;

            rtc.setTime(tmstruct.tm_sec, tmstruct.tm_min, tmstruct.tm_hour, tmstruct.tm_mday, (tmstruct.tm_mon)+1, (tmstruct.tm_year)+1900);
            Serial.printf("utc time: %d:%d:%d %d/%d/%d\n",tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, (tmstruct.tm_mon)+1, (tmstruct.tm_year)+1900);
            
            return(true);
        }

        return(false);
    }

    int time_hour(){
        return (rtc.getHour(true));
    }

    int time_minute(){
        return (rtc.getMinute());
    }

    int time_second(){
        return (rtc.getSecond());
    }

    int day_of_week(){
        return (rtc.getDayofWeek());
    }

    int time_minute_local(){
        int local_time_minute = rtc.getMinute();

        local_time_minute += local_minute_offset;

        if(local_time_minute > 59){
            local_time_minute -= 59;
        }

        else if(local_time_minute < 0){
            local_time_minute += 59;
        }

        return local_time_minute;
    }

    int time_hour_local(){
        int local_time_hour = rtc.getHour(true);

        local_time_hour += local_hour_offset;

        if(rtc.getMinute() + local_minute_offset > 59){
            local_time_hour+=1;
        }

        else if(rtc.getMinute() + local_minute_offset < 0){
            local_time_hour-=1;
        }

        if(local_time_hour > 23){
            local_time_hour -= 23;
        }
        else if(local_time_hour < 0){
            local_time_hour += 23;
        }

        return local_time_hour;
    }

    String globe_icon_path(){
        String path = String("/globe_icons/");

        int icon_code = time_hour() - 12;

        if(icon_code <= 0){
            icon_code += 24;
        }

        path += (String(icon_code) + String(".bmp"));
        
        return path;
    }

    int time_to_sync(){
        int next_sync_point = 0;

        if(time_minute() > 0 && time_minute() < 30){
            next_sync_point = (30 - time_minute());
        }

        if(time_minute() > 30 && time_minute() < 60){
            next_sync_point = (60 - time_minute());
        }

        return next_sync_point;
    }
}
