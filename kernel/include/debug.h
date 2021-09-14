// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

void _NORETURN panic(const char* msg);

#ifdef DEBUG
#define assert(x) if (!(x)) { \
    debug(KERNEL) << "****\n" \
               "**** in " << __FILE__ << ": " \
            << __func__ << "(), line " << __LINE__ << "\n"; \
    panic("assertion failed"); \
}
#define assert_verify(x) assert(x)
#else
#define assert(x)
#define assert_verify(x) (x)
#endif

#define DEBUG_HEX dbg::DebugStream::HEX
#define DEBUG_DEC dbg::DebugStream::HEX

#define OUTPUT_ENABLE BIT(10)
#define PAGEFAULT     1 | OUTPUT_ENABLE
#define SYSCALL       2 | OUTPUT_ENABLE
#define PROCESS       3 | OUTPUT_ENABLE
#define TASK          4 | OUTPUT_ENABLE
#define VSPACE        5 | OUTPUT_ENABLE
#define FPU           6 | OUTPUT_ENABLE
#define KERNEL        7 | OUTPUT_ENABLE
#define BOOT          8 | OUTPUT_ENABLE
#define EXCEPT        9 | OUTPUT_ENABLE
#define MEMORY       10 | OUTPUT_ENABLE
#define LOADER       11 | OUTPUT_ENABLE
#define TIMER        12 | OUTPUT_ENABLE
#define DEVICES      13 | OUTPUT_ENABLE

namespace dbg {

    class DebugStream
    {
      public:
        enum Modifier
        {
          HEX,
          DEC
        };

        DebugStream(int loglevel);
        ~DebugStream();

        DebugStream& operator<<(char c);
        DebugStream& operator<<(const char* str);
#ifndef i386
        DebugStream& operator<<(size_t i);
#endif
        DebugStream& operator<<(int32_t i);
        DebugStream& operator<<(int64_t i);
        DebugStream& operator<<(uint32_t i);
        DebugStream& operator<<(uint64_t i);
        DebugStream& operator<<(void* ptr);
        DebugStream& operator<<(Modifier modif);

      private:
        const static size_t BUFFER_SIZE = 256;

        int getBase();
        void basePrefix();

        char *m_destPtr;
        Modifier m_currentMode;
        char m_buffer[BUFFER_SIZE];

        int m_loglevel;
    };

}

static inline dbg::DebugStream debug(int loglevel)
{
  return dbg::DebugStream(loglevel);
}
