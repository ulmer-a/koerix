ARCH_C_SRC = $(wildcard arch/$(TARGET_ARCH)/*.c)
ARCH_S_SRC = $(wildcard arch/$(TARGET_ARCH)/*.S)
ARCH_OBJ   = $(patsubst %.S, %.S.o, $(ARCH_S_SRC)) \
             $(patsubst %.c, %.c.o, $(ARCH_C_SRC))
#ARCH_OBJ  := $(subst arch/$(TARGET_ARCH), $(BINDIR)/arch, $(ARCH_OBJ))
