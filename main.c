#include <efi.h>
#include <efilib.h>

#define DESIRED_HREZ            1600
#define DESIRED_VREZ             900
#define DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

void PreBootHalt( EFI_SIMPLE_TEXT_OUT_PROTOCOL* conerr, UINT16* msg );
void setPixel(int r, int c, uint32_t rgb, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop, uint32_t vres, uint32_t hres);

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
   InitializeLib(ImageHandle, SystemTable);
   UINTN handle_count = 0;
   EFI_HANDLE* handle_buffer;
   EFI_STATUS status;
   EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
   UINTN mode_num;
   EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
   UINTN size_of_info;
   //status = uefi_call_wrapper(BS->LocateHandleBuffer, ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &handle_count, handle_buffer);
   //status = uefi_call_wrapper(BS->LocateHandleBuffer, ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &handle_count, &handle_buffer);
   //status = BS->LocateHandleBuffer(EFI_LOCATE_SEARCH_TYPE.ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &handle_count, &handle_buffer);
   //status = LibLocateHandle(ByProtocol, &gEfiGraphicsOutputProtocolGuid, NULL, &handle_count, &handle_buffer);
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
        if (gop_mode_info->HorizontalResolution > 300 &&
              gop_mode_info->VerticalResolution > 300 &&
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

    uint32_t verticalResolution = gop_mode_info->VerticalResolution;
    uint32_t horizontalResolution = gop_mode_info->HorizontalResolution;

    uint32_t pxl = 0x000032FF;
    uint32_t pxlR = 0x00FF3200;

    for (int i = 0; i < verticalResolution; ++i) {
        for (int o = 0; o < horizontalResolution; ++o) {
            setPixel(i, o, pxl, gop, verticalResolution, horizontalResolution);
        }
    }

    uint32_t character[16*4] = {
        0b00000000000011111111000000000000,0b00000000000011111111000000000000,0b00000000000011111111000000000000,0b00000000000011111111000000000000,
        0b00000000111111111111111100000000,0b00000000111111111111111100000000,0b00000000111111111111111100000000,0b00000000111111111111111100000000,
        0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,
        0b00001111111111111111111111110000,0b00001111111111111111111111110000,0b00001111111111111111111111110000,0b00001111111111111111111111110000,
        0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,
        0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,0b00001111111100000000111111110000,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
        0,0,0,0,
    };
    for (int i = 0; i < 16*4; ++i) {
        for (int k = 0; k < 8*4; ++k) {
            int flag = (character[i] >> (32 - k)) & 1;
            if (flag) {
                setPixel(i, k, 0x00FFFFFF, gop, verticalResolution, horizontalResolution);
            }
        }
    }

    while (1) {
        ;
    }

    return EFI_SUCCESS;
}

void setPixel(int r, int c, uint32_t rgb, EFI_GRAPHICS_OUTPUT_PROTOCOL* gop, uint32_t vres, uint32_t hres)
{
    r *= 4;
    c *= 4;
    void* baseAddr = (void*) gop->Mode->FrameBufferBase;
    int32_t *addr = baseAddr + c + r * hres;
    *addr = rgb | 0xff000000;
}
