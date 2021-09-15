// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#define PPN_TO_VIRT(x)  ((void*)(((x) << PAGE_SHIFT) + IDENT_OFFSET))

#define ADDR_SPACE_IMPL \
  void updateKernelMappings(); \
  void invalidate(size_t virt); \
  void resolve(size_t virt, Mapping& mapping); \
  bool checkForPresentEntries(GenericPagingTable* table);

#define m_pml4 m_topLevelPageTable
