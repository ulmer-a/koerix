// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

struct Elf64Header
{
  uint32_t magic;           // should be 0x7f ELF
  uint8_t type;             // 1=32bit, 2=64bit
  uint8_t endianness;       // 1=little, 2=big
  uint8_t header_version;   // header version
  uint8_t os_abi;           // 0=SystemV
  uint64_t reserved1;
  uint16_t bintype;         // 1=reloc, 2=exec, 3=shared, 4=core
  uint16_t isa;             // isa_t
  uint32_t elf_version;     // ELF version
  uint64_t entry;           // program entry point
  uint64_t pht_pos;
  uint64_t sht_pos;
  uint32_t flags;
  uint16_t header_size;
  uint16_t pht_entry_size;
  uint16_t pht_entries;
  uint16_t sht_entry_size;
  uint16_t sht_entries;
  uint16_t sht_index_names;
} _PACKED;

struct Elf64ProgHeaderTableEntry
{
  uint32_t type;
  uint32_t flags;
  uint64_t p_offset;
  uint64_t p_vaddr;
  uint64_t undefined;
  uint64_t p_filesz;
  uint64_t p_memsz;
  uint64_t alignment;
} _PACKED;
