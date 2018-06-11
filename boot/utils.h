#ifndef BOOT_UTILS_H
#define BOOT_UTILS_H
#include <efi.h>
#include <efilib.h>

void setVideoMode();
void GetMemoryMap(EFI_MEMORY_DESCRIPTOR **memory_map, UINTN *descriptor_size, UINTN *memory_map_size, UINTN *map_key);

void printMemoryMap(EFI_MEMORY_DESCRIPTOR* map, UINTN map_size, UINTN descriptor_size);

void zero_memory(void *buf, int length);

#endif