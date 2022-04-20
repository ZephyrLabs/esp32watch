#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include "Arduino.h"

enum class wifi_status{
    connected,
    connection_fail,
    connection_lost,
    disconnected,
    scanned,
    unavailable,
    idle, 
    other
};

namespace wifi_service{
    bool wifi_service_config_init(const char* path);
    void wifi_service_set_credentials(char* ssid, char* wpa_passphrase);
    bool wifi_service_connect();
    bool wifi_service_disconnect();
    int wifi_service_strength();
    String wifi_service_icon();
    wifi_status wifi_service_status();
}

#endif
