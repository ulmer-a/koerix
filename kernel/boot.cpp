#include <debug.h>
#include <term.h>
#include <dev/devices.h>
#include <fs/file.h>
#include <module.h>
#include <shared_ptr.h>
#include <loader.h>
#include <user_proc.h>

void kernel_init(const char* cmdline)
{
  /* this is the entry point to the architecture-independent
   * kernel boot code. page map, virtual memory and kernel heap
   * as well as scheduling are already up and running at this point. */

  dev::setup();

  auto uart0 = (fs::File*)dev::findDevice("uart0");
  fs::FileDesc serialPort{ *uart0 };
  auto mainTerm = new Terminal(serialPort);
  Terminal::setMainTerm(mainTerm);

  debug() << "welcome on uart!\n";

  void* init_bin = find_module("init.elf");
  assert(init_bin != nullptr);
  auto loader = ktl::shared_ptr<Loader>(
        new Loader(init_bin));
  auto proc = new UserProcess(loader);
}
