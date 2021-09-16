#include <fpu.h>
#include <arch/fpu.h>

namespace fpu {

  Context::Context()
  {
    // TODO
  }

  Context* Context::clone()
  {
    // TODO
    return this;
  }

  void Context::save()
  {
    // TODO
  }

  void Context::restore() const
  {
    // TODO
  }

}

extern "C" {

bool fpuDetectAndEnable() { return false; }
void fpuEnableTrap() { }
void fpuClearTrap() { }

}
