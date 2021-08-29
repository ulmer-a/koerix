
TARGET_ARCH         = i386
TARGET_PLATFORM     = pc
BINDIR              = build

FLAGS               = -g -O0
OS_FLAGS			= -ffreestanding -nostdinc -nostdlib -fno-stack-protector \
                      -fno-pic -mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-red-zone

CFLAGS := $(CFLAGS) $(FLAGS) $(OS_FLAGS)
CXXFLAGS := $(CFLAGS) $(FLAGS) $(OS_FLAGS)
ASMFLAGS := $(CFLAGS) $(FLAGS) $(OS_FLAGS)


.phony: all clean qemu

all: prepare_bindir libkernel

include arch/$(TARGET_ARCH)/Makefile.inc
include platform/$(TARGET_PLATFORM)/Makefile.inc

#################################################
# GENERAL BUILD RULES                           #
#################################################

INCLUDES = -I arch/$(TARGET_ARCH)/include \
           -I kernel/include \
           -I platform/$(TARGET_PLATFORM)/include

%.c.o: %.c
	@ echo " [ CC  ]  $<"
	@ clang $(INCLUDES) $(CFLAGS) --target=$(TARGET_ARCH)-none-eabi -c $< -o $@

%.cxx.o: %.cpp
	@ echo " [ CXX ]  $<"
	@ clang++ $(INCLUDES) $(CXXFLAGS) --target=$(TARGET_ARCH)-none-eabi -c $< -o $@

%.S.o: %.S
	@ echo " [ ASM ]  $<"
	@ clang $(INCLUDES) $(ASMFLAGS) --target=$(TARGET_ARCH)-none-eabi -c $< -o $@

##########################################
# Kernel 
##########################################
KERNEL_SRC           = $(wildcard kernel/*.c)
KERNEL_OBJ           = $(patsubst %.c, %.c.o, $(KERNEL_SRC))
#KERNEL_OBJ          := $(subst kernel, $(BINDIR)/kernel, $(KERNEL_OBJ))
KERNEL_ELF           = $(BINDIR)/kernel.elf

libkernel: $(ARCH_OBJ) $(KERNEL_OBJ) $(PLAT_OBJ)
	@ echo " [ LD  ]  $(KERNEL_ELF)"
	@ ld -T platform/$(TARGET_PLATFORM)/$(TARGET_ARCH).ld \
		$(PLAT_OBJ) $(KERNEL_OBJ) $(ARCH_OBJ) \
		-o $(KERNEL_ELF)

#################################################
# OTHER PHONY TARGETS                           #
#################################################

prepare_bindir:
	@ mkdir -p $(BINDIR)
clean:
	@ rm -r $(BINDIR)

qemu:
	@ qemu-system-$(TARGET_ARCH) -kernel $(KERNEL_ELF) -debugcon stdio

