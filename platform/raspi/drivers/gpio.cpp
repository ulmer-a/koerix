#include <gpio.h>
#include <mm.h>
#include <raspi.h>

#define GPFSEL0         MMIO(0x00200000)
#define GPFSEL1         MMIO(0x00200004)
#define GPFSEL2         MMIO(0x00200008)
#define GPFSEL3         MMIO(0x0020000C)
#define GPFSEL4         MMIO(0x00200010)
#define GPFSEL5         MMIO(0x00200014)
#define GPSET0          MMIO(0x0020001C)
#define GPSET1          MMIO(0x00200020)
#define GPCLR0          MMIO(0x00200028)
#define GPCLR1          MMIO(0x0020002c)
#define GPLEV0          MMIO(0x00200034)
#define GPLEV1          MMIO(0x00200038)
#define GPEDS0          MMIO(0x00200040)
#define GPEDS1          MMIO(0x00200044)
#define GPREN0          MMIO(0x0020004c)
#define GPREN1          MMIO(0x00200050)
#define GPFEN0          MMIO(0x00200058)
#define GPFEN1          MMIO(0x0020005c)
#define GPHEN0          MMIO(0x00200064)
#define GPHEN1          MMIO(0x00200068)
#define GPLEN0          MMIO(0x00200070)
#define GPLEN1          MMIO(0x00200074)
#define GPPUD           MMIO(0x00200094)
#define GPPUDCLK0       MMIO(0x00200098)
#define GPPUDCLK1       MMIO(0x0020009C)

static void waitCycles(size_t cycles)
{
  while (cycles--)
    __asm__ volatile ("nop;");
}

namespace raspi {
  namespace gpio {

    void setPinMode(size_t pin, PinMode pinMode, PullMode pullMode)
    {
      volatile uint32_t *GPFREG;
      if (pin < 10)
        GPFREG = GPFSEL0;
      else if (pin < 20)
        GPFREG = GPFSEL1;
      else if (pin < 30)
        GPFREG = GPFSEL2;
      else if (pin < 40)
        GPFREG = GPFSEL3;
      else if (pin < 50)
        GPFREG = GPFSEL4;
      else if (pin < 54)
        GPFREG = GPFSEL5;
      else return;

      size_t gpfIndex = pin % 10;
      size_t modeValue = pinMode & 0x7;

      uint32_t r = *GPFREG;
      r &= ~(0x7 << (gpfIndex * 3));
      r |= (modeValue << (gpfIndex * 3));
      *GPFREG = r;

      setPullMode(pin, pullMode);
    }

    void setPullMode(size_t pin, PullMode pullMode)
    {
      volatile uint32_t *CLKREG;
      if (pin < 32)
        CLKREG = GPPUDCLK0;
      else if (pin < 54)
        CLKREG = GPPUDCLK1;

      auto bit = BIT(pin % 32);

      *GPPUD = pullMode;
      waitCycles(150);
      *CLKREG = bit;
      waitCycles(150);
      *CLKREG = 0;
      *GPPUD = 0;
    }

    void setTriggerMode(size_t pin, TriggerMode triggerMode)
    {
      volatile uint32_t *ENREG;

      switch (triggerMode)
      {
      case TRIGGER_HIGH: ENREG  = GPHEN0; break;
      case TRIGGER_LOW:  ENREG  = GPHEN0; break;
      case TRIGGER_RISE: ENREG  = GPHEN0; break;
      case TRIGGER_FALL: ENREG  = GPHEN0; break;
      }

      if (pin >= 32)
        ENREG++;

      *ENREG |= (1 << (pin % 32));
    }

    void setPin(size_t pin, bool enabled)
    {
      volatile uint32_t* GPSREG;
      if (pin < 32 && enabled)
        GPSREG = GPSET0;
      else if (pin < 54 && enabled)
        GPSREG = GPSET1;
      else if (pin < 32)
        GPSREG = GPCLR0;
      else if (pin < 54)
        GPSREG = GPCLR1;
      else
        assert(false);

      size_t pin_index = pin % 32;
      *GPSREG |= (1 << pin_index);
    }

  }
}
