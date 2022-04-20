#ifndef WATCHFACE_H
#define WATCHFACE_H

#include "Arduino.h"

namespace watchface{
    void watchface_init();
    String get_num_glyph(int n);
}

#endif
