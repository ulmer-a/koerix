#include <types.h>
#include <mm.h>
#include <addr_space.h>
#include <bitmap.h>
#include <pagemap.h>
#include <x86/stivale.h>

static PageMap s_pagemap;

PageMap& PageMap::get()
{
    return s_pagemap;
}

static size_t getSystemPageCount(stivale_struct* stivale_info)
{
    /* Compute the amount of page frames present in system RAM */
    auto mmap = (stivale_mmap_entry*)stivale_info->memory_map_addr;
    auto& last_entry = mmap[stivale_info->memory_map_entries - 1];
    return ((last_entry.base + last_entry.length) >> PAGE_SHIFT) + 1;
}

static size_t getFirstNFreePages(stivale_struct* stivale_info, size_t n)
{
    /* Find first consecutive n free pages */
    auto mmap = (stivale_mmap_entry*)stivale_info->memory_map_addr;
    for (int i = 0; i < stivale_info->memory_map_entries; i++)
    {
        if (mmap[i].type == MEM_USABLE ||
            mmap[i].type == MEM_LOADER_RECLAIMABLE)
        {
            if (mmap[i].length >= (n << PAGE_SHIFT))
            {
                return mmap[i].base >> PAGE_SHIFT;
            }
        }
    }

    return (size_t)-1;
}

void create_page_bitmap(stivale_struct *stivale_info)
{
    /* Compute total RAM size */
    const auto total_pages = getSystemPageCount(stivale_info);

    /* Compute the amount of pages needed to store the page bitmap */
    const size_t bitmap_size = BITMAP_BYTE_SIZE(total_pages);
    size_t bitmap_pages = bitmap_size >> PAGE_SHIFT;
    if (total_pages % PAGE_SIZE != 0)
        bitmap_pages++;

    /* Get a memory location to store the page bitmap */
    const auto pagemap_addr =
        (void*)(getFirstNFreePages(stivale_info, bitmap_pages) << PAGE_SHIFT);

    /* Create a temporary bitmap and clear all usable pages */
    baselib::Bitmap tempBitmap { total_pages, pagemap_addr };
    tempBitmap.setAll();

    auto mmap = (stivale_mmap_entry*)stivale_info->memory_map_addr;
    for (int i = 0; i < stivale_info->memory_map_entries; i++)
    {
        if (mmap[i].type == MEM_USABLE ||
            mmap[i].type == MEM_LOADER_RECLAIMABLE)
        {
            /* For usable memory and bootloader reclaimable memory, the
             * base and length fields are guaranteed to be page aligned. */
            size_t start_page = mmap[i].base >> PAGE_SHIFT;
            size_t page_count = mmap[i].length >> PAGE_SHIFT;

            for (size_t j = 0; j < page_count; j++)
                tempBitmap.clr(start_page + j);
        }
    }

    /* Initialize the PageManager class */
    new (&s_pagemap) PageMap(total_pages, pagemap_addr);
}
