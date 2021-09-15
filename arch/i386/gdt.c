// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

/*
 * UlmerOS i386 Global descriptor table and Task state segment
 * Copyright (C) 2017-2021 Alexander Ulmer
 *
 * The GDT configuration provides four different segment
 * selectors that can be used.
 *
 * Usable segment selector values
 *  0x08 -> Kernel code segment
 *  0x10 -> Kernel data segment
 *  0x1b -> User mode code segment
 *  0x23 -> User mode data segment
 */

#include <types.h>

/* GDT entry count:
 *  1 Null descriptor
 *  2 Kernel segment descriptors
 *  2 32bit User segment descriptors
 *  1 Task state segment (128bit)
 *  --------------------------------------
 *  7 x 64bit total
 */
#define GDT_ENTRIES     7
#define TSS_INDEX       5

/* segment descriptor flags. they can be combined to
 * create different variations of segment descriptors. */
#define PRESENT         BIT(47)             // selector is present
#define CODE            BIT(43) | BIT(44)   // code segment
#define CODE_R          BIT(41)             // code segment readable
#define DATA            BIT(44)             // data segment
#define DATA_W          BIT(41)             // data segment writable
#define LONGMODE        BIT(53)             // long mode / compatibility mode
#define DPL_USER        BIT(45) | BIT(46)   // user mode
#define OPSIZE32        BIT(54)             // set compat. op size to 32bit
#define MAX_LIMIT       0x8f00000000ffff    // maximum segment limit´

typedef struct
{
  uint16_t size;
  uint64_t* addr;
} _PACKED gdt_t;

typedef struct
{
  uint64_t limit1     :16;  // bits 0-15
  uint64_t base1      :24;  // bits 0-23
  uint64_t type       :4;
  uint64_t zero1      :1;
  uint64_t dpl        :2;
  uint64_t present    :1;
  uint64_t limit2     :4;   // bits 16-19
  uint64_t avl        :1;
  uint64_t reserved1  :2;
  uint64_t gran       :1;
  uint64_t base2      :40;   // bits 24-63
  uint64_t reserved2  :8;
  uint64_t zero2      :5;
  uint64_t reserved3  :19;
} _PACKED tssd_t;

typedef struct
{
  uint16_t link;
  uint16_t reserved0;
  uint32_t esp0;
  uint16_t ss0;
  uint16_t reserved1;
  uint32_t esp1;
  uint16_t ss1;
  uint16_t reserved2;
  uint32_t esp2;
  uint16_t ss2;
  uint16_t reserved3;
  uint32_t cr3;
  uint32_t eip;
  uint32_t eflags;
  uint32_t eax;
  uint32_t ecx;
  uint32_t edx;
  uint32_t ebx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t esi;
  uint32_t edi;
  uint16_t es;
  uint16_t reserved4;
  uint16_t cs;
  uint16_t reserved5;
  uint16_t ss;
  uint16_t reserved6;
  uint16_t ds;
  uint16_t reserved7;
  uint16_t fs;
  uint16_t reserved8;
  uint16_t gs;
  uint16_t reserved9;
  uint16_t ldtr;
  uint32_t reserved10;
  uint16_t iopb;
} _PACKED tss_t;

static gdt_t s_gdt_descriptor;
static uint64_t s_gdt[GDT_ENTRIES];
static tss_t s_tss = {};

static void load_gdt()
{
  /* tell the CPU the location of the GDT by writing it's
   * size and location to the hidden GDT register */
  s_gdt_descriptor.size = sizeof(uint64_t) * GDT_ENTRIES;
  s_gdt_descriptor.addr = s_gdt;
  __asm__ volatile("lgdt %0" : : "g"(s_gdt_descriptor));
}

static void setup_tss(tssd_t* td)
{
  uint64_t addr = (uint64_t)&s_tss;

  td->zero1   = 0x00;
  td->zero2   = 0x00;
  td->base1   = addr & 0xffffff;
  td->base2   = addr >> 24;
  td->limit1  = sizeof(tss_t);
  td->limit2  = 0x00;

  td->present = 0x01; // present
  td->avl     = 0x00; // not used
  td->gran    = 0x00; // not scaled
  td->dpl     = 0x03; // kernel
  td->type    = 0x09; // available TSS

  s_tss.iopb = sizeof(tss_t);

  __asm__ volatile(
    "mov $0x3b, %ax;"
    "ltr %ax;"
  );
}

void setup_gdt()
{
  s_gdt[0] = 0;

  /* 32bit kernel segment descriptors */
  s_gdt[1] = PRESENT | MAX_LIMIT | CODE | CODE_R;
  s_gdt[2] = PRESENT | MAX_LIMIT | DATA | DATA_W;

  /* 32bit user segment descriptors*/
  s_gdt[3] = PRESENT | MAX_LIMIT | CODE | CODE_R | DPL_USER;
  s_gdt[4] = PRESENT | MAX_LIMIT | DATA | DATA_W | DPL_USER;

  /* load the global descriptor table */
  load_gdt();

  /* setup and load a task state segment */
  setup_tss((tssd_t*)&s_gdt[TSS_INDEX]);
}

void setKernelStackPtr(size_t stackptr)
{
  s_tss.esp0 = stackptr;
}
