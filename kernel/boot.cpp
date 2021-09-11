#include <debug.h>
#include <term.h>
#include <dev/devices.h>
#include <fs/vfs.h>
#include <fs/fd.h>
#include <module.h>
#include <shared_ptr.h>
#include <loader.h>
#include <user_proc.h>
#include <pagemap.h>

static void printIssue()
{
  debug() << "Koerix OS kernel\n"
             "Copyright (C) 2017-2021 Alexander Ulmer\n";

  auto& pagemap = PageMap::get();
  debug() << "Memory: " << (pagemap.getUsedMemory() >> 20) << "/"
          << (pagemap.getUsableMemory() >> 20) << " MB used"
             " of total " << (pagemap.getTotalMemory() >> 20) << " MB\n\n";
}

const char* s_cmdline;
bool cmdline_get(const char* name, char* buffer);

void kernel_init(const char* cmdline)
{
  /* this is the entry point to the architecture-independent
   * kernel boot code. page map, virtual memory and kernel heap
   * as well as scheduling are already up and running at this point. */

  char valueBuffer[128];
  s_cmdline = cmdline;
  dev::setup();

  /* check whether we got a command line option instructing the kernel
   * to open a terminal with a specified device */
  ktl::shared_ptr<Terminal> mainTerm;
  if (cmdline_get("console", valueBuffer))
  {
    int error;
    auto fd = fs::open(valueBuffer, error);
    if (!fd.valid()) {
      debug() << "warning: " << valueBuffer << ": "
              << strerror(error) << "\n";
    } else {
      mainTerm = ktl::shared_ptr<Terminal>(new Terminal(fd));
      Terminal::setMainTerm(mainTerm);
    }
  }

  printIssue();

  if (!cmdline_get("init", valueBuffer)) {
    panic("no 'init' cmdline parameter: don't know what to execute");
  }

  void* init_bin = find_module(valueBuffer);
  assert(init_bin != nullptr);
  auto loader = ktl::shared_ptr<Loader>(
        new Loader(init_bin));
  auto proc = new UserProcess(loader, mainTerm);
}
