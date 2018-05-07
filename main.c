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

    lfb_address = (uint32_t) ((void*) gop->Mode->FrameBufferBase);
    verticalResolution = gop_mode_info->VerticalResolution;
    horizontalResolution = gop_mode_info->HorizontalResolution;

}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    setMode();

    clear(0, 0, 0);
    const char* string = "Hello World!\nSpencer Brown :D";
    kprint(string);

    while(1) {
        ;
    }

    return EFI_SUCCESS;
}
