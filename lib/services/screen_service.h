#ifndef SCREEN_SERVICE_H
#define SCREEN_SERVICE_H

namespace screen_service{
        void tft_bl_init();
        void tft_splash_screen();
        int tft_bl_get();
        void tft_bl_set(int duty);
        void tft_sleep();
        void tft_wake();
}

#endif