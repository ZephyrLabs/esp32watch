#include "battery_service.h"
#include "stdlib.h"

namespace battery_service{

    int battery_service_left(){
        const uint16_t vref = 1100;
        const uint16_t vbat_max = 4250;
        const uint16_t vbat_min = 3250;
        
        int vbat = (analogRead(34)/4095.0) * 2 * 3.3 * (vref/1000.0)*1000;

        return(int(100*(vbat - vbat_min)/(vbat_max - vbat_min)));
    }

    String battery_icon(){
        int percent = battery_service_left();

        if(percent > 120){
            return String("/charge_icons/charging.bmp");
        }
        if(percent < 10){
            return String("/charge_icons/0.bmp");
        }

        String battery_icon_path = "/charge_icons/";
        String file = String(percent - (percent%10));
        battery_icon_path += file;
        battery_icon_path += ".bmp";

        return battery_icon_path;
    }
}
