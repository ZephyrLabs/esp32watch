#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include "Arduino.h"

namespace weather_service{
    void weather_service_init();
    void weather_service_fetch();
    String weather_icon();
    String weather_temp();
    String weather_desc();
}

#endif