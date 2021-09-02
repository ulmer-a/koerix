#pragma once

#include <types.h>
#include <debug.h>

#define MAGIC 0xdeadbeef

namespace ktl {

    class SanityCheckObj
    {
      public:
        SanityCheckObj()
            : magic(MAGIC)
        { }

        ~SanityCheckObj() = default;

        void verify() {
            assert(magic == MAGIC);
        }

      private:
        uint32_t magic;
    };

}
