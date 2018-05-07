#include <stdint.h>

#include "font8x8_basic.h"

typedef uint32_t Pixel;

extern uint32_t lfb_address;
extern uint32_t horizontalResolution;
extern uint32_t verticalResolution;

void clear(uint8_t red, uint8_t green, uint8_t blue);

void drawChar(char character, int line, int column);

void setPixel(int r, int c, uint32_t rgb);

Pixel createPixel(uint8_t red, uint8_t green, uint8_t blue);