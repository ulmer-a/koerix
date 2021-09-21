#include <types.h>
#include <debug.h>
#include <pci/device.h>
#include <pci/config_space.h>

namespace pci {

  static bool deviceIsPresent(size_t bus, size_t slot, size_t func)
  {
    uint16_t vendorId = read32(0x00, bus, slot, func) & 0xffff;
    return vendorId != 0xffff;
  }

  void busEnumeration()
  {
    for (int bus = 0; bus < 256; bus++)
    {
      for (int slot = 0; slot < 32; slot++)
      {
        for (int func = 0; func < 8; func++)
        {
          if (!deviceIsPresent(bus, slot, func))
            continue;

          PciDevice dev(bus, slot, func);

          debug(DEVICES) << DEBUG_HEX << "PCI: "
                         << dev.vendorId() << ":"
                         << dev.deviceId() << "\n";
        }
      }
    }
  }

}
