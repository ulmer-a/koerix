/*
 *  file taken from: stivale boot protocol with minor modifications
 *  https://github.com/stivale/stivale/blob/3ac220793819e4fd883f8688bfbcecedecccc921/stivale.h
 * 
 *  Copyright 2019, 2020, 2021 mintsuki and contributors.
 * 
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in
 *        the documentation and/or other materials provided with the
 *        distribution.
 * 
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#pragma once
#include <types.h>

// Anchor for non ELF kernels
struct stivale_anchor {
    uint8_t anchor[15];
    uint8_t bits;
    uint64_t phys_load_addr;
    uint64_t phys_bss_start;
    uint64_t phys_bss_end;
    uint64_t phys_stivalehdr;
};

/* --- Header --------------------------------------------------------------- */
/*  Information passed from the kernel to the bootloader                      */

struct stivale_header {
    uint64_t stack;
    uint16_t flags;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint64_t entry_point;
};

/* --- Struct --------------------------------------------------------------- */
/*  Information passed from the bootloader to the kernel                      */

struct stivale_module {
    uint64_t begin;
    uint64_t end;
    char string[128];
    uint64_t next;
};

#define STIVALE_MMAP_USABLE                 1
#define STIVALE_MMAP_RESERVED               2
#define STIVALE_MMAP_ACPI_RECLAIMABLE       3
#define STIVALE_MMAP_ACPI_NVS               4
#define STIVALE_MMAP_BAD_MEMORY             5
#define STIVALE_MMAP_KERNEL_AND_MODULES     10
#define STIVALE_MMAP_BOOTLOADER_RECLAIMABLE 0x1000
#define STIVALE_MMAP_FRAMEBUFFER            0x1002

struct stivale_mmap_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t unused;
};

enum stivale_mem_type {
    MEM_USABLE              = 0x0001,
    MEM_RESERVED            = 0x0002,
    MEM_ACPI_RECLAIMABLE    = 0x0003,
    MEM_ACPI_NVS            = 0x0004,
    MEM_BAD                 = 0x0005,
    MEM_KERNEL_CODE         = 0x1000,
    MEM_LOADER_RECLAIMABLE  = 0x1002
};

#define STIVALE_FBUF_MMODEL_RGB 1

struct stivale_struct {
    uint64_t cmdline;
    uint64_t memory_map_addr;
    uint64_t memory_map_entries;
    uint64_t framebuffer_addr;
    uint16_t framebuffer_pitch;
    uint16_t framebuffer_width;
    uint16_t framebuffer_height;
    uint16_t framebuffer_bpp;
    uint64_t rsdp;
    uint64_t module_count;
    uint64_t modules;
    uint64_t epoch;
    uint64_t flags; // bit 0: 1 if booted with BIOS, 0 if booted with UEFI
                    // bit 1: 1 if extended colour information passed, 0 if not
    uint8_t  fb_memory_model;
    uint8_t  fb_red_mask_size;
    uint8_t  fb_red_mask_shift;
    uint8_t  fb_green_mask_size;
    uint8_t  fb_green_mask_shift;
    uint8_t  fb_blue_mask_size;
    uint8_t  fb_blue_mask_shift;
    uint8_t  reserved;
    uint64_t smbios_entry_32;
    uint64_t smbios_entry_64;
};
