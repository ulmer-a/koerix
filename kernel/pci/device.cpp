#include <pci/device.h>
#include <pci/config_space.h>

namespace pci {

  PciDevice::PciDevice(size_t bus, size_t slot, size_t func)
    : m_bus(bus)
    , m_slot(slot)
    , m_func(func)
  {
    uint32_t id = read32(0x00, bus, slot, func);
    m_vendorId = id & 0xffff;
    m_deviceId = id >> 16;
  }

}
