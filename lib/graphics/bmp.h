#ifndef BMP_H
#define BMP_H

#include <FS.h>
#include <SPIFFS.h>

namespace bmp{
    uint16_t read16(fs::File &f);
    uint32_t read32(fs::File &f);
    void drawBmp(const char *filename, int16_t x, int16_t y);
    void drawBmp(const char *filename, int16_t x, int16_t y, int16_t transparent);
}

#endif