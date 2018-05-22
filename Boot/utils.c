#include "utils.h"

void printMemoryMap(EFI_MEMORY_DESCRIPTOR* map, UINTN map_size, UINTN descriptor_size)
{
    char *mem = (char*) map;
    Print(L"sizeof(EFI_MEMORY_DESCRIPTOR):\t%d\n", sizeof(EFI_MEMORY_DESCRIPTOR));
    Print(L"descriptor_size:\t%d\n", descriptor_size);
    for (int i = 0; i < (map_size / descriptor_size); ++i) {
        EFI_MEMORY_DESCRIPTOR desc = *( (EFI_MEMORY_DESCRIPTOR*) mem);
        Print(L"\n");
        Print(L"Physical Address of i-th memory descriptor:\t%x\n", desc.PhysicalStart);
        Print(L"Virtual Address of i-th memory descriptor:\t%x\n", desc.VirtualStart);
        Print(L"Memory Type of i-th memory descriptor:\t%d\n", desc.Type);
        uefi_call_wrapper(BS->Stall, 1, 4000000);

        mem += descriptor_size;
    }
}