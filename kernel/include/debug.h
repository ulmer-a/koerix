#pragma once

#include <types.h>

void _NORETURN panic(const char* msg);

#ifdef DEBUG
#define assert(x) if (!(x)) { \
    debug() << "****\n" \
               "**** in " << __FILE__ << ": " \
            << __func__ << "(), line " << __LINE__ << "\n"; \
    panic("assertion failed"); \
}
#define assert_verify(x) assert(x)
#else
#define assert(x)
#define assert_verify(x) (x)
#endif

#define DEBUG_HEX debugging::DebugStream::HEX
#define DEBUG_DEC debugging::DebugStream::HEX

namespace debugging {

    class DebugStream
    {
      public:
        enum Modifier
        {
            HEX,
            DEC
        };

        DebugStream();
        ~DebugStream();

        DebugStream& operator<<(char c);
        DebugStream& operator<<(const char* str);
        DebugStream& operator<<(size_t i);
        DebugStream& operator<<(int32_t i);
        DebugStream& operator<<(int64_t i);
        DebugStream& operator<<(uint32_t i);
        DebugStream& operator<<(uint64_t i);
        DebugStream& operator<<(void* ptr);
        DebugStream& operator<<(Modifier modif);

      private:
        int getBase();
        void basePrefix();

        char *m_destPtr;
        Modifier m_currentMode;
        char m_buffer[256];
    };

}

static inline debugging::DebugStream debug()
{
    return debugging::DebugStream();
}
