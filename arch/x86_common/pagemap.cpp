#include <types.h>
#include <addr_space.h>
#include <x86/stivale.h>

static size_t getSystemPageCount(stivale_struct* stivale_info)
{
    /* Compute the amount of page frames present in system RAM */
    auto mmap = (stivale_mmap_entry*)stivale_info->memory_map_addr;
    auto& last_entry = mmap[stivale_info->memory_map_entries - 1];
    return ((last_entry.base + last_entry.length) >> PAGE_SHIFT) + 1;
}

void create_page_bitmap(stivale_struct *stivale_info)
{

}
