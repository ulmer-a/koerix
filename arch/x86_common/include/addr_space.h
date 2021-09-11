#pragma once

#include <types.h>
#include <mutex.h>
#include <offsets.h>

#define PPN_TO_PTR(x)   ((void*)((x) << PAGE_SHIFT))
#define PPN_TO_VIRT(x)  ((void*)(((x) << PAGE_SHIFT) + IDENT_OFFSET))

class AddrSpace
{
  public:
    enum MapFlags
    {
        MAP_USER    = BIT(0),
        MAP_WRITE   = BIT(1),
        MAP_NOEXEC  = BIT(2)
    };

    struct Mapping;
    struct GenericPagingTable;

    explicit AddrSpace();
    AddrSpace(const AddrSpace& self) = delete;

    AddrSpace* clone();
    ~AddrSpace();

    static void setup();
    static AddrSpace& kernel();

    void apply();

    void map(size_t virt, size_t phys, int flags);
    void unmap(size_t virt);

    bool triggerCow(size_t virt);

  private:
    void updateKernelMappings();
    void invalidate(size_t virt);
    void resolve(size_t virt, Mapping& mapping);
    bool checkForPresentEntries(GenericPagingTable* table);

    size_t m_topLevelPageTable;
    Mutex m_lock;
};
