#pragma once

#include <types.h>

extern size_t _raspi_mmio_base;
#define MMIO(addr) ((volatile uint32_t*)(_raspi_mmio_base + (addr)))

#include <gpio.h>
