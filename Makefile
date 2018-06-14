ARCH	    = $(shell uname -m | sed s,i[3456789]86,ia32,)

# Boot-Loader
BOOTLOADER_OBJS = $(addprefix boot/, main.o utils.o loader.o)
#Kernel
KERNEL_OBJS = $(addprefix system/, kernel.o graphics.o)

OBJS	    = boot/main.o boot/utils.o boot/loader.o
TARGET	  = hello.efi

EFIINC	  = /usr/include/efi
EFIINCS	 = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
LIB	     = /usr/lib64
EFILIB	  = /usr/lib64
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS	 = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS	  = $(EFIINCS) -fno-stack-protector -fpic \
		  -fshort-wchar -mno-red-zone -Wall
ifeq ($(ARCH),x86_64)
  CFLAGS += -DEFI_FUNCTION_WRAPPER
endif

LDFLAGS	 = -nostdlib -znocombreloc -T $(EFI_LDS) -shared \
		  -Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS) 

all: $(TARGET)

clean:
	rm -rf *.so
	rm -rf *.efi
	rm -rf *.o

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic \
		-j .dynsym  -j .rel -j .rela -j .reloc \
		--target=efi-app-$(ARCH) $^ $@

hello.so: $(BOOTLOADER_OBJS)
	ld $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi