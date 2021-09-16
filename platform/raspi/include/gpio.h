#pragma once

#include <types.h>

namespace raspi {
  namespace gpio {

    enum PinMode
    {
      MODE_INPUT   = 0b000,
      MODE_OUTPUT  = 0b001,
      MODE_ALT0    = 0b100,
      MODE_ALT1    = 0b101,
      MODE_ALT2    = 0b110,
      MODE_ALT3    = 0b111,
      MODE_ALT4    = 0b011,
      MODE_ALT5    = 0b010
    };

    enum PullMode
    {
      PULL_NONE    = 0,
      PULL_UP      = 1,
      PULL_DOWN    = 2
    };

    enum TriggerMode
    {
      TRIGGER_HIGH  = 1,
      TRIGGER_LOW   = 2,
      TRIGGER_RISE  = 3,
      TRIGGER_FALL  = 4
    };

    void setPinMode(size_t pin, PinMode pinMode, PullMode pullMode);
    void setPullMode(size_t pin, PullMode pullMode);
    void setTriggerMode(size_t pin, TriggerMode triggerMode);
    void setPin(size_t pin, bool enabled);

  }
}
