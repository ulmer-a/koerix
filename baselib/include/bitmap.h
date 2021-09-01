#pragma once

#include <types.h>

#define BITMAP_BYTE_SIZE(n) ((n % 8) ? ((n) >> 3) : ((n) >> 3) + 1)

namespace baselib {

    class Bitmap
    {
      public:
        Bitmap() = delete;
        explicit Bitmap(size_t size, void* mem = nullptr);
        ~Bitmap();

        Bitmap(const Bitmap& bm) = delete;

        void set(size_t n);
        void clr(size_t n);

        void setAll();
        void clrAll();

        size_t getSetCount() const;
        size_t getClrCount() const;

        bool get(size_t n) const;

      private:
        bool m_isHeapAllocated;
        size_t m_size;
        size_t m_setCount;
        uint8_t* m_bitmap;
    };

}
