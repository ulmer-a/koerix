#pragma once

#include <types.h>
#include <bitmap.h>

class PageMap
{
  public:
    PageMap();

    static PageMap& get();
    PageMap(size_t bitmapSize, void* initializedMem);

    size_t alloc();
    void free(size_t ppn);

  private:
    baselib::Bitmap m_bitmap;
};
