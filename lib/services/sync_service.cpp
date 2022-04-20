#include "wifi_service.h"
#include "time_service.h"
#include "weather_service.h"

namespace sync_service{
    bool sync_service(){        
        time_service::time_service_ntp_sync();
        weather_service::weather_service_init();
        weather_service::weather_service_fetch();

        return true;
    }
}