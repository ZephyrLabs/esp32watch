#ifndef BATTERY_SERVICE_H
#define BATTERY_SERVICE_H

#include "Arduino.h"

namespace battery_service{
        int battery_service_left();
        String battery_icon();
}

#endif