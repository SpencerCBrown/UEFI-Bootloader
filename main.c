#include <efi.h>
#include <efilib.h>

#include "kernel.h"

/*
 * This should handle all bootloader functionality.  e.g. setting boot video mode and getting VRAM LFB using UEFI Boot Services, etc.
*/

#define DESIRED_HREZ            1600
#define DESIRED_VREZ             900
#define DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

void setMode();
void setupMemory();

void setMode()
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

void setupMemory()
{

}

void zero_memory(void *buf, int length)
{
    char *bytes = (char*) buf;
    for (int i = 0; i < length; ++i) {
        bytes[i] = 0;
    }
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    EFI_STATUS status = EFI_SUCCESS;

    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINT32 version = 0;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    UINTN memory_map_size = 0;

    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);

    status = uefi_call_wrapper(BS->GetMemoryMap, 5, &memory_map_size, memory_map, &map_key, &descriptor_size, &version);
    // descriptor size and version should be correct values.  memory_map_size should be needed size.

    if (status == EFI_BUFFER_TOO_SMALL) {
        UINTN encompassing_size = memory_map_size + (2 * descriptor_size);
        void *buffer = NULL;
        status = uefi_call_wrapper(BS->AllocatePool, 3, EfiLoaderData, encompassing_size, &buffer);
        if (status == EFI_SUCCESS) {
            memory_map = (EFI_MEMORY_DESCRIPTOR*) buffer;
            memory_map_size = encompassing_size;

            status = uefi_call_wrapper(BS->GetMemoryMap, 5, &memory_map_size, memory_map, &map_key, &descriptor_size, &version);
            if (status != EFI_SUCCESS) {
                Print(L"Second call to GetMemoryMap failed for some reason.");
            } else {
                Print(L"Second call to GetMemoryMap succeeded.\n");
                char * mem = (char*) memory_map;
                for (int i = 0; i < (memory_map_size / descriptor_size); ++i) {
                    EFI_MEMORY_DESCRIPTOR desc = *( (EFI_MEMORY_DESCRIPTOR*) mem);
                    Print(L"sizeof(EFI_MEMORY_DESCRIPTOR):\t%d", sizeof(desc));
                    Print(L"descriptor_size:\t%d", descriptor_size);
                    // Print(L"Physical Address of i-th memory descriptor:\t%x\n", desc.PhysicalStart);
                    // Print(L"Virtual Address of i-th memory descriptor:\t%x\n", desc.VirtualStart);
                    // Print(L"Memory Type of i-th memory descriptor:\t%d\n", desc.Type);
                    uefi_call_wrapper(BS->Stall, 1, 4000000);

                    mem += descriptor_size;
                }
            }

        } else {
            Print(L"AllocatePool failure.");
        }
    } else if (status == EFI_SUCCESS) { // shouldn't happen.
        Print(L"First call to GetMemoryMap should never succeed... ???");
    } else {
        Print(L"(First) GetMemoryMap usage failure.");
    }


    return EFI_SUCCESS;
}
