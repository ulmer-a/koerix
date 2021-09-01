#include <types.h>

class PageMap
{
  public:
    PageMap();

    static PageMap& get();

    size_t alloc();
    void free(size_t ppn);
};
