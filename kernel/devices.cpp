#include <dev/devices.h>
#include <platform.h>
#include <list.h>
#include <mm.h>
#include <string.h>
#include <dev/chardev.h>

namespace dev {

  static ktl::List<dev::Device*> s_devices;

  Device::Device()
  {
    registerDevice(this);
  }

  Device::~Device()
  {
    unregisterDevice(this);
  }

  void setup()
  {
    new (&s_devices) ktl::List<dev::Device*>;
    init_drivers();
  }

  void registerDevice(Device* chardev)
  {
    s_devices.push_back(chardev);
  }

  void unregisterDevice(Device* chardev)
  {
    s_devices.remove(s_devices.find(chardev));
  }

  Device* findDevice(const char* name)
  {
    for (auto it = s_devices.begin(); it != nullptr; it = it->next)
    {
      if (strcmp((*it)->getName(), name) == 0)
        return it->item;
    }
    return nullptr;
  }

}
