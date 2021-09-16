#include <x86/cpuinfo.h>

namespace cpuid {

  static bool s_initialized = false;
  static CpuFeaturesEcx s_ecxFeatures;
  static CpuFeaturesEdx s_edxFeatures;
  static CpuFeaturesExtEcx s_ecxExtFeatures;
  static CpuFeaturesExtEdx s_edxExtFeatures;

  extern "C" void _cpuid(
      size_t function, uint32_t* ecx, uint32_t* edx);

  void initialize()
  {
    _cpuid(0x00000001, (uint32_t*)&s_ecxFeatures, (uint32_t*)&s_edxFeatures);
    _cpuid(0x80000001, (uint32_t*)&s_ecxExtFeatures, (uint32_t*)&s_edxExtFeatures);
    s_initialized = true;
  }

  CpuFeaturesEcx& getFeatures1()
  {
    if (!s_initialized)
      initialize();
    return s_ecxFeatures;
  }

  CpuFeaturesEdx& getFeatures2()
  {
    if (!s_initialized)
      initialize();
    return s_edxFeatures;
  }

  cpuid::CpuFeaturesExtEdx& getFeatures3()
  {
    if (!s_initialized)
      initialize();
    return s_edxExtFeatures;
  }

}
