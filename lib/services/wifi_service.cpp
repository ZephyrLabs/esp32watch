#include "wifi_service.h"
#include "Arduino.h"
#include "WiFi.h"
#include "SPIFFS.h"
#include "FS.h"
#include "stdlib.h"
#include "string.h"

namespace wifi_service{
    char wpa_ssid[32];
    char wpa_passphrase[32];

    bool wifi_service_config_init(const char* path){

        memset(wpa_ssid, 0, 32);
        memset(wpa_passphrase, 0, 32);

        if(!SPIFFS.exists(path)){
            strcpy(wpa_ssid, "null");
            strcpy(wpa_passphrase, "null");
            
            return(false);
        }
        else{
            fs::File wifi_config_file = SPIFFS.open(path, "r");

            strcpy(wpa_ssid, wifi_config_file.readStringUntil('\n').c_str());
            strcpy(wpa_passphrase, wifi_config_file.readStringUntil('\n').c_str());
        }

        return(true);
    }

    void wifi_service_set_credentials(char* ssid, char* passphrase){
        memset(wpa_ssid, 0, 32);
        memset(wpa_passphrase, 0, 32);
        
        strcpy(wpa_ssid, ssid);
        strcpy(wpa_passphrase, passphrase);
    }

    bool wifi_service_connect(){
        /* enable the modem */
        //adc_power_on();
        WiFi.disconnect(false);
        WiFi.mode(WIFI_STA);

        WiFi.begin(wpa_ssid, wpa_passphrase);
        unsigned long conn_start = millis();
        while(WiFi.status() != WL_CONNECTED) {
            if((millis() - conn_start) < 10000)continue;
            else{
                return(false);
            }
        }
        return(true);
    }

    bool wifi_service_disconnect(){
        /* disable the modem */
        //adc_power_off();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return(true);
    }

    int wifi_service_strength(){
        return WiFi.RSSI();
    }

    String wifi_service_icon(){
        int wifi_strength = wifi_service_strength();
        
        String wifi_icon_path = "/radio_icons/unavailable.bmp";

        if(wifi_strength > -80) wifi_icon_path = "/radio_icons/poor.bmp";
        if(wifi_strength > -70) wifi_icon_path = "/radio_icons/weak.bmp";
        if(wifi_strength > -60) wifi_icon_path = "/radio_icons/moderate.bmp";
        if(wifi_strength > -50) wifi_icon_path = "/radio_icons/strong.bmp";

        if(wifi_strength == 0) wifi_icon_path = "/radio_icons/unavailable.bmp";

        return wifi_icon_path;
    }

    wifi_status wifi_service_status(){
        switch (WiFi.status()){
            case WL_CONNECTED:
                return wifi_status::connected;

            case WL_CONNECT_FAILED:
                return wifi_status::connection_fail;
            
            case WL_CONNECTION_LOST:
                return wifi_status::connection_lost;
            
            case WL_DISCONNECTED:
                return wifi_status::disconnected;

            case WL_IDLE_STATUS:
                return wifi_status::idle;
            
            case WL_NO_SSID_AVAIL:
                return wifi_status::unavailable;
            
            case WL_SCAN_COMPLETED:
                return wifi_status::scanned;

            default:
                return wifi_status::other;
        }
    }
}