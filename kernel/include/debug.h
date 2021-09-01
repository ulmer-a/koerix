#pragma once

#include <types.h>

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

        char *m_destPtr;
        Modifier m_currentMode;
        char m_buffer[256];
    };

}

static inline debugging::DebugStream debug()
{
    return debugging::DebugStream();
}