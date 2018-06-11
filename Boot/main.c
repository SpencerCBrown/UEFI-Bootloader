#include <efi.h>
#include <efilib.h>

#include "utils.h"

#include "elf.h"
void load_kernel()
{
    
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    UINTN memory_map_size = 0;
    GetMemoryMap(&memory_map, &descriptor_size, &memory_map_size, &map_key);

    // TODO call ExitBootServices and handoff control to kernel
    while (1);

    return EFI_SUCCESS;
}
