#include "utils.h"

#define DESIRED_HREZ            1600
#define DESIRED_VREZ             900
#define DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

extern uint32_t* lfb_address;
extern uint32_t verticalResolution;
extern uint32_t horizontalResolution;

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
        Print(L"Memory Type of i-th memory descriptor:\t%x\n", desc.Type);
        uefi_call_wrapper(BS->Stall, 1, 4000000);

        mem += descriptor_size;
    }
}

void zero_memory(void *buf, int length)
{
    char *bytes = (char*) buf;
    for (int i = 0; i < length; ++i) {
        bytes[i] = 0;
    }
}

void setVideoMode()
{
    UINTN handle_count = 0;
    EFI_HANDLE* handle_buffer;
    EFI_STATUS status;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    UINTN mode_num;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
    UINTN size_of_info;

    status = uefi_call_wrapper(BS->LocateHandleBuffer, 5, ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &handle_count, &handle_buffer);
    if (status != EFI_SUCCESS) { //
        while (1) {
            Print(L"ERROR IN LocateHANDLEBuffer\n");
            Print(L"NUMBER OF BUFFERS: %d", handle_count);
            Print(L"ERROR CODE IS: %d", status);
        }
    }

    status = uefi_call_wrapper(BS->HandleProtocol, 3, *handle_buffer, &gEfiGraphicsOutputProtocolGuid, (VOID **) &gop);
    if (status != EFI_SUCCESS) {
        while (1) {
            Print(L"ERROR In HandleProtocol\n");
            Print(L"ERROR CODE IS: %d", status);
        }
    }

    for (mode_num = 0; (status = uefi_call_wrapper(gop->QueryMode, 4, gop, mode_num, &size_of_info, &gop_mode_info )) == EFI_SUCCESS; mode_num++) {
        Print(L"In Loop");
        if (gop_mode_info->HorizontalResolution >= 800 &&
              gop_mode_info->VerticalResolution >= 600 &&
              gop_mode_info->PixelFormat        == DESIRED_PIXEL_FORMAT)
            break;
    }

    if (gop_mode_info->PixelFormat != DESIRED_PIXEL_FORMAT) {
        while (1) {
            Print(L"CRAP\n");
        }
    }

    if (status != EFI_SUCCESS) {
       while (1) {
           Print(L"ERROR In QueryMode\n");
           Print(L"ERROR CODE IS: %d", status);
       }
   }

   if (uefi_call_wrapper(gop->SetMode, 2, gop, mode_num) != EFI_SUCCESS) {
        while (1) {
            Print(L"ERROR In SetMode\n");
        }
    }

    lfb_address = ((uint32_t*) gop->Mode->FrameBufferBase);
    verticalResolution = gop_mode_info->VerticalResolution;
    horizontalResolution = gop_mode_info->HorizontalResolution;

}

void GetMemoryMap(EFI_MEMORY_DESCRIPTOR **memory_map_ptr, UINTN *descriptor_size, UINTN *memory_map_size, UINTN *map_key)
{
    EFI_STATUS status = EFI_SUCCESS;
    UINT32 version = 0;
    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    *descriptor_size = 0;
    *memory_map_size = 0;
    *map_key = 0;

    status = uefi_call_wrapper(BS->GetMemoryMap, 5, memory_map_size, memory_map, map_key, descriptor_size, &version);

    if (status == EFI_BUFFER_TOO_SMALL) {
        UINTN encompassing_size = *memory_map_size + (2 * (*descriptor_size));
        void *buffer = NULL;
        status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, encompassing_size, &buffer);
        if (status == EFI_SUCCESS) {
            memory_map = (EFI_MEMORY_DESCRIPTOR*) buffer;
            *memory_map_size = encompassing_size;

            status = uefi_call_wrapper(BS->GetMemoryMap, 5, memory_map_size, memory_map, map_key, descriptor_size, &version);
            if (status != EFI_SUCCESS) {
                // Print(L"Second call to GetMemoryMap failed for some reason.");
            } else {
                // Print(L"Second call to GetMemoryMap succeeded.\n");
            }
        } else {
            // Print(L"Failure allocating memory pool");
        }
    } else if (status == EFI_SUCCESS) { // shouldn't happen.
        // Print(L"First call to GetMemoryMap should never succeed... ???");
    } else {
        // Print(L"(First) GetMemoryMap usage failure.");
    }
    *memory_map_ptr = memory_map;
}