#include <platform.h>
#include <raspi.h>
#include <mm.h>

#define AUX_ENABLE      MMIO(0x00215004)
#define AUX_MU_IO       MMIO(0x00215040)
#define AUX_MU_IER      MMIO(0x00215044)
#define AUX_MU_IIR      MMIO(0x00215048)
#define AUX_MU_LCR      MMIO(0x0021504C)
#define AUX_MU_MCR      MMIO(0x00215050)
#define AUX_MU_LSR      MMIO(0x00215054)
#define AUX_MU_MSR      MMIO(0x00215058)
#define AUX_MU_SCRATCH  MMIO(0x0021505C)
#define AUX_MU_CNTL     MMIO(0x00215060)
#define AUX_MU_STAT     MMIO(0x00215064)
#define AUX_MU_BAUD     MMIO(0x00215068)

/* On the Raspberry Pi, we're going to use
 * UART1 for debugging purposes. */

using namespace raspi;

void debug_print_init()
{
  /* expose UART1 on the hardware GPIO pins,
   * disable pullup/pulldown resistors */
  gpio::setPinMode(14, gpio::MODE_ALT5, gpio::PULL_NONE);
  gpio::setPinMode(15, gpio::MODE_ALT5, gpio::PULL_NONE);

  *AUX_ENABLE |= 1;       // enable UART1
  *AUX_MU_CNTL = 0;       // disable TX, RX
  *AUX_MU_LCR = 3;        // line control: set 8N1
  *AUX_MU_MCR = 0;        // modem ctrl: ignored for auto flow-ctrl
  *AUX_MU_IER = 0;        // doesn't do anything ?
  *AUX_MU_IIR = 0xc6;     // disable interrupts
  *AUX_MU_BAUD = 270;     // 115200
  *AUX_MU_CNTL = 2;       // re-enable TX, but no RX (since this is a debug port)
}

void debug_print(const char* str)
{
  while (*str)
  {
    while (!(*AUX_MU_LSR & 0x20));
    *AUX_MU_IO = *str++;
  }
}
