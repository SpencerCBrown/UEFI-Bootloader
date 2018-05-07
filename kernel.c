#include "kernel.h"

//TODO: handle new line/carriage return
void kprint(const char *output)
{
    int current_line = 0;
    int current_column = 0;
    int character_index = 0;
    char c;
    while ((c = output[character_index++]) != '\0') {
        if (c == '\n') {
            ++current_line;
            current_column = 0;
        } else {
            // In the future we won't call drawChar directly, but rather write to a buffer that is drawn to screen by the graphics module.
            drawChar(c, current_line, current_column*8);
            ++current_column;
        }
    }
}
