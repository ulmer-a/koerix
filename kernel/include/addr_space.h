// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <mutex.h>
#include <offsets.h>

#include <arch/addr_space.h>

class AddrSpace
{
  public:
    enum MapFlags
    {
      MAP_USER    = BIT(0),
      MAP_WRITE   = BIT(1),
      MAP_NOEXEC  = BIT(2),
      MAP_SHARED = BIT(3)
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
    ADDR_SPACE_IMPL

    size_t m_topLevelPageTable;
    Mutex m_lock;
};
