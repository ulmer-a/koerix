#include <arch/asm.h>
#include <debug.h>
#include <offsets.h>

static bool disassembleFrame(size_t& basePtr, size_t& retAddr)
{
  static const size_t codeStart = KERNEL_CODE;
  static const size_t codeEnd = (size_t)-1;

  const auto stackPtr = basePtr;
  basePtr = *(size_t*)stackPtr;
  retAddr = *((size_t*)stackPtr + 1);

  if (retAddr >= codeStart && retAddr <= codeEnd)
    return true;
  return false;
}

void printStackTrace()
{
  debug(KERNEL) << "--- STACK TRACE ---\n";
  size_t retAddr;
  size_t basePtr;

  __asm__ volatile ("mov %%rbp, %0;" : "=r"(basePtr));

  while (disassembleFrame(basePtr, retAddr))
  {
    debug(KERNEL) << (void*)retAddr << "\n";
  }
}


