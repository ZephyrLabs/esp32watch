#ifndef TIME_SERVICE_H
#define TIME_SERVICE_H

#include "Arduino.h"
#include "time.h"

namespace time_service{
  void time_service_init();
  bool time_service_ntp_sync();
  void time_fetch_service();
  
  int time_hour();
  int time_minute();
  int time_second();

  int time_hour_local();
  int time_minute_local();
  int day_of_week();

  String globe_icon_path();
  int time_to_sync();

  const char days_of_week[7][10] = {"sunday", 
                                    "monday", 
                                    "tuesday", 
                                    "wednesday", 
                                    "thursday", 
                                    "friday", 
                                    "saturday"
                                  };
}

#endif
