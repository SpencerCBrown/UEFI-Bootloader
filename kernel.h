#include <stdint.h>

typedef uint32_t Pixel;

extern uint32_t lfb_address;
extern uint32_t horizontalResolution;
extern uint32_t verticalResolution;

void kprint(char *output);

void clear(uint8_t red, uint8_t green, uint8_t blue);

void setPixel(int r, int c, uint32_t rgb);

Pixel createPixel(uint8_t red, uint8_t green, uint8_t blue);
