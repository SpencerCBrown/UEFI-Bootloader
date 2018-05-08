#ifndef KERNEL_H
#define KERNEL_H

#include "graphics.h"

void drawNum(int num, int *line, int *column);

void kprintf(const char *output, int num);

void kprint(const char *output);

#endif