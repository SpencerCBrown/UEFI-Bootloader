#include <efi.h>
#include <efilib.h>

#include "utils.h"

#include "elf.h"

void open_kernel_image(EFI_HANDLE ImageHandle, EFI_FILE_PROTOCOL **file)
{
    EFI_STATUS status = 0;
    EFI_HANDLE device_handle;
    EFI_LOADED_IMAGE *loaded_image;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *filesystem_protocol;
    EFI_FILE_PROTOCOL *filesystem;
    EFI_GUID loaded_image_protocol = LOADED_IMAGE_PROTOCOL;
    EFI_GUID simple_fs_protocol = SIMPLE_FILE_SYSTEM_PROTOCOL;

    status = uefi_call_wrapper(BS->HandleProtocol, 3, ImageHandle, &loaded_image_protocol, (void **) &loaded_image);
    if (status != EFI_SUCCESS) {
        Print(L"load_kernel: Error in HandleProtocol for LOADED_IMAGE_PROTOCOL.\n");
    }

    device_handle = loaded_image->DeviceHandle;

    status = uefi_call_wrapper(BS->HandleProtocol, 3, device_handle, &simple_fs_protocol, &filesystem_protocol);
    if (status != EFI_SUCCESS) {
        Print(L"load_kernel: Error in HandleProtocol for Simple Filesystem Protocol.\n");
    }

    status = uefi_call_wrapper(filesystem_protocol->OpenVolume, 2, filesystem_protocol, &filesystem);
    if (status != EFI_SUCCESS) {
        Print(L"load_kernel: Error in OpenVolume.\n");
    }

    Print(L"REACHED FILE PROTOCOL::Open call.\n");

    status = uefi_call_wrapper(filesystem->Open, 5, filesystem, file, L"simple.txt", EFI_FILE_MODE_READ, 0);
    if (status != EFI_SUCCESS) {
        Print(L"load_kernel: Error opening file.\n");
    }

    return;
}

void load_kernel(EFI_HANDLE ImageHandle)
{
    EFI_STATUS status;
    EFI_FILE_PROTOCOL *file;
    open_kernel_image(ImageHandle, &file);

    char buffer[16];
    UINTN size = 16;
    uefi_call_wrapper(file->Read, 3, file, &size, buffer);
    if (status != EFI_SUCCESS) {
        Print(L"load_kernel: ERROR reading file.\t%d\n", status);
    }

    for (int i = 0; i < size; ++i) {
        Print(L"data %d:\t%d\n", i, buffer[i]);
    }
}

EFI_STATUS
EFIAPI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);

    load_kernel(ImageHandle);

    while (1) {
        ;
    }

    EFI_MEMORY_DESCRIPTOR *memory_map = NULL;
    UINTN map_key = 0;
    UINTN descriptor_size = 0;
    UINTN memory_map_size = 0;
    GetMemoryMap(&memory_map, &descriptor_size, &memory_map_size, &map_key);

    // TODO call ExitBootServices and handoff control to kernel
    while (1);

    return EFI_SUCCESS;
}
