#pragma once

#include <types.h>

namespace pci {

  class PciDevice
  {
    public:
      PciDevice(size_t bus, size_t slot, size_t func);

      inline uint16_t vendorId() { return m_vendorId; }
      inline uint16_t deviceId() { return m_deviceId; }

    private:
      size_t m_bus, m_slot, m_func;
      uint16_t m_vendorId, m_deviceId;
  };

}
