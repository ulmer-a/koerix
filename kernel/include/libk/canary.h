#pragma once

#include <types.h>
#include <debug.h>

#define MAGIC 0xdeadbeef

namespace ktl {

  /* Deriving from the Canary class will put a 32bit word into
   * your object, which at strategic locations can be checked
   * for the correct value with verify(). This is especially
   * helpful to detect memory corruptions as well as use of an
   * object that hasn't been constructed yet (e.g. when you use
   * a global object whose constructor has not been called). */

  class Canary
  {
    public:
      Canary() {
        magic = MAGIC;
      }

      ~Canary() {
        magic = 0;
      }

      void verify() const {
          assert(magic == MAGIC);
      }

    private:
      uint32_t magic;
  };

}