#include "spiffs_init.h"
#include "FS.h"
#include "SPIFFS.h"

namespace spiffs_init{
    void spiffs_init(){
        if (!SPIFFS.begin()) {
            Serial.println("SPIFFS initialisation failed!");
        }
        else{
            Serial.println("SPIFFS mounted");
        }
    }
};