#include "kernel.h"

/*
 * Graphics Module Functionality:
 * Assume 32bit pixel format
*/

uint32_t lfb_address;
uint32_t verticalResolution;
uint32_t horizontalResolution;

void kprint(char *output)
{
    // do nothing for now
}

void clear(uint8_t red, uint8_t green, uint8_t blue)
{
    Pixel pixel = createPixel(red, green, blue);
    for (int i = 0; i < verticalResolution; ++i) {
        for (int o = 0; o < horizontalResolution; ++o) {
            setPixel(i, o, pixel);
        }
    }
}

void setPixel(int r, int c, uint32_t rgb)
{
    r *= 4;
    c *= 4;
    uint32_t *addr = lfb_address + c + r * horizontalResolution;
    *addr = rgb | 0xff000000;
}


Pixel createPixel(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t pixel = 0;
    pixel = red;
    pixel <<= 8;
    pixel |= green;
    pixel <<= 8;
    pixel |= blue;
    return pixel;
}
