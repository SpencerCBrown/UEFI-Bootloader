#include "kernel.h"

void drawNum(int num, int *line, int *column)
{
    char buf[100]; // no more than 100 digits
    int count;
    for (count = 0; num > 0; ++count) {
        int digit = num % 10;
        char literal_digit = digit + 48; // convert from int to ascii char
        buf[count] = literal_digit;
        num /= 10;
    }

    for (int i = count - 1; i >= 0; --i) {
        drawChar(buf[i], (*line), *column);
        *column = *column + 1;
    }
}

//TODO (for this and reg kprint) add protection for when column is past end of screen?
void kprintf(const char *output, int num)
{
    int current_line = 0;
    int current_column = 0;
    int character_index = 0;
    char c = output[character_index++];
    while (c != '\0') {
        if (c == '\n') {
            ++current_line;
            current_column = 0;
        } else if (c == '#') {
            c = output[character_index++];
            // if c == '\0', then ERROR
            // draw char
            drawNum(num, &current_line, &current_column);
            ++current_column;
        } else {
            // In the future we won't call drawChar directly, but rather write to a buffer that is drawn to screen by the graphics module.
            drawChar(c, current_line, current_column);
            ++current_column;
        }
        if (c != '\0') {
            c = output[character_index++];
        }
    }
}

//TODO: handle new line/carriage return
void kprint(const char *output)
{
    int current_line = 0;
    int current_column = 0;
    int character_index = 0;
    char c = output[character_index++];
    while (c != '\0') {
        if (c == '\n') {
            ++current_line;
            current_column = 0;
        } else {
            // In the future we won't call drawChar directly, but rather write to a buffer that is drawn to screen by the graphics module.
            drawChar(c, current_line, current_column*8);
            ++current_column;
        }
        if (c != '\0') {
            c = output[character_index++];
        }
    }
}
