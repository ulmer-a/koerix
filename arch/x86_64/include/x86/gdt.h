// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

/*
 * Koerix OS x86 Global Descriptor Table and Task state segment
 * Copyright (C) 2017-2021 Alexander Ulmer
 *
 * The GDT configuration provides four (six) different segment
 * descriptors that can be used. With compatibility mode
 * user segment selectors, the 64 bit operating system can also
 * run 32bit x86 programs in compatibility mode.
 *
 * GDT entry count:
 *  1 Null descriptor
 *  2 Kernel segment descriptors
 *  2 User segment descriptors
 *  2 32 bit User segment descriptors (x86_64 only)
 *  1 Task state segment 128 bit
 *  ----------------------------------------------
 *  7 (9) x 64bit total
 *
 * Usable segment selector values:
 *  0x08 -> Kernel code segment
 *  0x10 -> Kernel data segment
 *  0x1b -> User code segment
 *  0x23 -> User data segment
 *  0x2b -> Compatibility mode user code segment (x86_64 only)
 *  0x33 -> Compatibility mode user data segment (x86_64 only)
 */

#include <types.h>

/* basic segment descriptor flags. they can be combined to
 * create different variations of segment descriptors. */
#define PRESENT         BIT(47)                       // selector is present
#define DPL_USER        BIT(45) | BIT(46)             // user mode
#define CODE            BIT(41) | BIT(43) | BIT(44)   // code segment
#define DATA            BIT(41) | BIT(44)             // data segment
#define MAX_LIMIT       0x8f00000000ffff              // maximum segment limit
#define LONGMODE        BIT(53)             // for 64 bit code/data
#define OPSIZE32        BIT(54)             // for 32 bit code/data

#ifdef i386
  #define GDT_ENTRIES     7
  #define TSS_INDEX       5
  #define MODE OPSIZE32
#else
  #define GDT_ENTRIES     9
  #define TSS_INDEX       7
  #define MODE LONGMODE
#endif

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

#ifdef i386
  typedef struct
  {
    uint32_t prev_tss;   // for hardware multitasking
    uint32_t esp0;       // kernel SP
    uint32_t ss0;        // kernel SS
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
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
    uint32_t es;         // kernel ES
    uint32_t cs;         // kernel CS
    uint32_t ss;         // kernel SS
    uint32_t ds;         // kernel DS
    uint32_t fs;         // kernel FS
    uint32_t gs;         // kernel GS
    uint32_t ldt;        // unused: no LDT
    uint16_t trap;
    uint16_t iopb_offset;
  } _PACKED tss_t;
#else
  typedef struct
  {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist1;
    uint64_t ist2;
    uint64_t ist3;
    uint64_t ist4;
    uint64_t ist5;
    uint64_t ist6;
    uint64_t ist7;
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
  } _PACKED tss_t;
#endif

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

static void _INIT setup_tss(tssd_t* td)
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

  s_tss.iopb_offset = sizeof(tss_t);

  __asm__ volatile(
  #ifdef i386
    "mov $0x2b, %%ax;"
  #else
    "mov $0x3b, %%ax;"
  #endif
    "ltr %%ax;"
    ::: "ax"
  );
}

void _INIT setup_gdt()
{
  s_gdt[0] = 0;

  /* kernel segment descriptors */
  s_gdt[1] = PRESENT | MODE | MAX_LIMIT | CODE;
  s_gdt[2] = PRESENT | MODE | MAX_LIMIT | DATA;

  /* user segment descriptors*/
  s_gdt[3] = PRESENT | MODE | MAX_LIMIT | CODE | DPL_USER;
  s_gdt[4] = PRESENT | MODE | MAX_LIMIT | DATA | DPL_USER;

#ifndef i386
  /* 32bit compatibility mode user segment descriptors */
  s_gdt[5] = PRESENT | OPSIZE32 | MAX_LIMIT | CODE | DPL_USER;
  s_gdt[6] = PRESENT | OPSIZE32 | MAX_LIMIT | DATA | DPL_USER;
#endif

  /* load the global descriptor table */
  load_gdt();

  /* setup and load a task state segment */
  setup_tss((tssd_t*)&s_gdt[TSS_INDEX]);
}

void setKernelStackPtr(size_t stackptr)
{
#ifdef i386
  s_tss.esp0 = stackptr;
#else
  s_tss.rsp0 = stackptr;
#endif
}
