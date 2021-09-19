// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>

namespace cpuid {

  struct CpuFeaturesEcx
  {
    uint32_t sse3 : 1;
    uint32_t pclmul : 1;
    uint32_t dtes64 : 1;
    uint32_t monitor : 1;
    uint32_t ds_cpl : 1;
    uint32_t vmx : 1;
    uint32_t smx : 1;
    uint32_t est : 1;
    uint32_t tm2 : 1;
    uint32_t ssse3 : 1;
    uint32_t cid : 1;
    uint32_t fma : 1;
    uint32_t cx16 : 1;
    uint32_t etprd : 1;
    uint32_t pcmd : 1;
    uint32_t pcide : 1;
    uint32_t dca : 1;
    uint32_t sse4_1 : 1;
    uint32_t sse4_2 : 1;
    uint32_t x2apic : 1;
    uint32_t movbe : 1;
    uint32_t popcnt : 1;
    uint32_t aes : 1;
    uint32_t xsave : 1;
    uint32_t oxsave : 1;
    uint32_t avx : 1;
  } _PACKED;

  struct CpuFeaturesEdx
  {
    uint32_t fpu :1;
    uint32_t vme :1;
    uint32_t de :1;
    uint32_t pse :1;
    uint32_t tsc :1;
    uint32_t msr :1;
    uint32_t pae :1;
    uint32_t mce :1;
    uint32_t cx8 :1;
    uint32_t apic :1;
    uint32_t sep :1;
    uint32_t mtrr :1;
    uint32_t pge :1;
    uint32_t mca :1;
    uint32_t cmov :1;
    uint32_t pat :1;
    uint32_t pse36 :1;
    uint32_t psn :1;
    uint32_t clf :1;
    uint32_t dtes :1;
    uint32_t acpi :1;
    uint32_t mmx :1;
    uint32_t fxsr :1;
    uint32_t sse :1;
    uint32_t sse2 :1;
    uint32_t ss :1;
    uint32_t htt :1;
    uint32_t tm1 :1;
    uint32_t ia64 :1;
    uint32_t pbe :1;
  } _PACKED;

  struct CpuFeaturesExtEcx
  {
    uint32_t dont_care;
  } _PACKED;

  struct CpuFeaturesExtEdx
  {
    uint32_t fpu :1;
    uint32_t vme :1;
    uint32_t de :1;
    uint32_t pse :1;
    uint32_t tsc :1;
    uint32_t msr :1;
    uint32_t pae :1;
    uint32_t mce :1;
    uint32_t cmpxchg8b :1;
    uint32_t apic :1;
    uint32_t reserved :1;
    uint32_t syscall :1;
    uint32_t mtrr :1;
    uint32_t pge :1;
    uint32_t mca :1;
    uint32_t cmov :1;
    uint32_t pat :1;
    uint32_t pse36 :1;
    uint32_t reserved2 :2;
    uint32_t nx :1;
  } _PACKED;

  CpuFeaturesEcx& getFeatures1();
  CpuFeaturesEdx& getFeatures2();
  CpuFeaturesExtEdx& getFeatures3();
}

