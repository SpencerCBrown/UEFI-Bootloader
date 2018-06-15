#include <efi.h>
#include <efilib.h>

#include "loader.h"
#include "utils.h"

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    load_kernel(ImageHandle);

    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    UINTN memory_map_size = 0;
    GetMemoryMap(&memory_map, &descriptor_size, &memory_map_size, &map_key);

    printMemoryMap(memory_map, memory_map_size, descriptor_size);

    // TODO call ExitBootServices and handoff control to kernel
    while (1);

    return EFI_SUCCESS;
}
