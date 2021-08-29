PLAT_C_SRC = $(wildcard platform/$(TARGET_PLATFORM)/*.c)
PLAT_S_SRC = $(wildcard platform/$(TARGET_PLATFORM)/*.S)
PLAT_OBJ   = $(patsubst %.S, %.S.o, $(PLAT_S_SRC)) \
             $(patsubst %.c, %.c.o, $(PLAT_C_SRC))
#PLAT_OBJ  := $(subst platform/$(TARGET_PLATFORM), $(BINDIR)/platform, $(PLAT_OBJ))
