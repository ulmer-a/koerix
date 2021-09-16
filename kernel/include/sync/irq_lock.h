#pragma once

#include <sync/lock.h>

namespace sync {

  class IrqDisableLock : public Lock
  {
    public:
      IrqDisableLock() = default;
      ~IrqDisableLock();

      void lock() override;
      void unlock() override;

    private:
      bool m_locked;
      bool m_irqsWereEnabled;
  };

}
