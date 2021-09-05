#include <types.h>
#include <mm.h>
#include <string.h>
#include <debug.h>
#include <addr_space.h>
#include <pagemap.h>
#include <x86/stivale.h>

static PageMap s_pagemap;
extern stivale_struct* s_stivale;

PageMap& PageMap::get()
{
    /* Singleton getter for the PageMap */
    return s_pagemap;
}

static size_t getSystemPageCount()
{
    /* Compute the amount of page frames present in system RAM */
    auto mmap = (stivale_mmap_entry*)s_stivale->memory_map_addr;

    for (int i = s_stivale->memory_map_entries - 1; i >= 0; i--)
    {
        if (mmap[i].type != MEM_USABLE && mmap[i].type != MEM_LOADER_RECLAIMABLE)
            continue;
        auto& last_usable_entry = mmap[i];
        return (last_usable_entry.base + last_usable_entry.length) >> PAGE_SHIFT;
    }

    assert(false);
    return 0;
}

static size_t getFirstNFreePages(size_t n)
{
    /* Find first consecutive n free pages */
    auto mmap = (stivale_mmap_entry*)s_stivale->memory_map_addr;
    for (int i = 0; i < s_stivale->memory_map_entries; i++)
    {
        if (mmap[i].type == MEM_USABLE)
        {
            if (mmap[i].length >= (n << PAGE_SHIFT))
            {
                return mmap[i].base >> PAGE_SHIFT;
            }
        }
    }

    return (size_t)-1;
}

static const char* get_mmap_type_str(uint32_t type)
{
    switch (type)
    {
    case MEM_RESERVED: return "reserved";
    case MEM_USABLE: return "usable";
    case MEM_BAD: return "bad memory";
    case MEM_ACPI_NVS:
    case MEM_ACPI_RECLAIMABLE: return "ACPI";
    case MEM_KERNEL: return "kernel/module code, data";
    case MEM_LOADER_RECLAIMABLE: return "bootloader reclaimable";
    case MEM_FRAMEBUFFER: return "framebuffer";
    default: return "unknown";
    }
}

static void print_mmap_entry(stivale_mmap_entry& entry)
{
    debug() << "  " << DEBUG_HEX
            << entry.base << " - " << (entry.base + entry.length - 1)
            << ": " << get_mmap_type_str(entry.type) << "\n";
}

void create_page_bitmap()
{
    /* Compute total RAM size */
    const auto total_pages = getSystemPageCount();
    debug() << "Total RAM size: " << (total_pages >> 8) << "MB ("
            << total_pages << " page frames)\n";

    /* Compute the amount of pages needed to store the page refcounter */
    size_t pagemap_pages = total_pages >> PAGE_SHIFT;
    if (total_pages % PAGE_SIZE != 0)
        pagemap_pages++;
    debug() << "PageMap: ref counter size: ~" << (total_pages >> 10) << "KB\n";

    /* Get a memory location to store the page bitmap */
    const size_t pagemap_start_page = getFirstNFreePages(pagemap_pages);
    const auto pagemap = (uint8_t*)PPN_TO_VIRT(pagemap_start_page);
    debug() << "PageMap: stored at " << (void*)pagemap << "\n";

    /* Set initial refcount for all pages to 0xff (max) */
    memset(pagemap, 0xff, total_pages);

    size_t free_pages = 0;
    auto mmap = (stivale_mmap_entry*)s_stivale->memory_map_addr;
    for (int i = 0; i < s_stivale->memory_map_entries; i++)
    {
        print_mmap_entry(mmap[i]);

        if (mmap[i].type == MEM_USABLE)
        {
            /* For usable memory and bootloader reclaimable memory, the
             * base and length fields are guaranteed to be page aligned. */
            size_t start_page = mmap[i].base >> PAGE_SHIFT;
            size_t page_count = mmap[i].length >> PAGE_SHIFT;

            for (size_t j = 0; j < page_count; j++)
            {
                free_pages += 1;
                pagemap[start_page + j] = 0;
            }
        }
    }

    for (int i = 0; i < pagemap_pages; i++)
    {
        free_pages -= 1;
        pagemap[pagemap_start_page + i] = 0xff;
    }

    /* Initialize the PageManager class */
    new (&s_pagemap) PageMap(total_pages, free_pages, pagemap);

    debug() << "Usable RAM: " << (free_pages >> 8)
            << "/" << (total_pages >> 8) << " MB\n";
}
