#ifndef BL_LOADER_H
#define BL_LOADER_H
#include <efi.h>
#include <efilib.h>

#include "elf.h"

void load_kernel(EFI_HANDLE ImageHandle);

void parse_ehdr(void *buf, Elf64_Ehdr *header);
void print_ehdr(void *buf);
void open_kernel_image(EFI_HANDLE ImageHandle, EFI_FILE_PROTOCOL **file);

#endif