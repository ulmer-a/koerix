// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

extern void* _ppn_to_virt(size_t ppn);
#define PPN_TO_VIRT(x)  _ppn_to_virt(x)

#define ADDR_SPACE_IMPL \
  void updateKernelMappings(); \
  void invalidate(size_t virt); \
  void resolve(size_t virt, Mapping& mapping); \
  bool checkForPresentEntries(GenericPagingTable* table);

#define m_pdir m_topLevelPageTable
#define m_pdpt m_topLevelPageTable
