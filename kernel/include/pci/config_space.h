#pragma once

#include <types.h>

namespace pci {

  uint32_t read32(size_t offset,
    size_t bus, size_t slot, size_t func);

  void write32(size_t offset, uint32_t value,
    size_t bus, size_t slot, size_t func);

}
