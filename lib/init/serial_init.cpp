#include <Arduino.h>
#include "serial_init.h"

namespace serial_init{
        void serial_init(){
                Serial.begin(115200);
        }
};
