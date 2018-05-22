#ifndef KERNEL_H
#define KERNEL_H

#include "graphics.h"

void drawNum(uint64_t num, int *line, int *column);

void kprintf(const char *output, uint64_t num);

void kprint(const char *output);

#endif