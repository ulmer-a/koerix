// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later
//
//  PCI Configuration Space Access
//

#include <pci/config_space.h>
#include <x86/ports.h>

namespace pci {

  static const uint16_t CONFIG_ADDR   = 0xcf8;
  static const uint16_t CONFIG_DATA   = 0xcfc;

  static inline uint32_t calcAddress(size_t offset,
      size_t bus, size_t slot, size_t func)
  {
    uint32_t address = (bus << 16)
        | (slot << 11)
        | (func << 8)
        | (offset & 0xfc)
        | BIT(31);
    return address;
  }

  uint32_t read32(size_t offset, size_t bus, size_t slot, size_t func)
  {
    auto addr = calcAddress(offset, bus, slot, func);
    outl(CONFIG_ADDR, addr);
    return inl(CONFIG_DATA);
  }

  void write32(size_t offset, uint32_t value,
             size_t bus, size_t slot, size_t func)
  {
    auto addr = calcAddress(offset, bus, slot, func);
    outl(CONFIG_ADDR, addr);
    outl(CONFIG_DATA, value);
  }

}
