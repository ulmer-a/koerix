// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later
//
// PS/2 Controller driver
//

#include <mm.h>
#include <lib/errno.h>
#include <x86/ports.h>
#include <dev/uart.h>
#include <interrupt.h>
#include <debug.h>
#include <dev/devices.h>
#include <scheduler.h>

enum PS2Registers
{
  DATA_PORT   = 0x60,
  CMD_PORT    = 0x64,
  STATUS_PORT = 0x64
};

struct PS2ConfigByte
{
  uint8_t irqPort1      : 1;
  uint8_t irqPort2      : 1;
  uint8_t systemState   : 1;
  uint8_t zero          : 1;
  uint8_t clkPort1      : 1;
  uint8_t clkPort2      : 1;
  uint8_t translPort1   : 1;
  uint8_t zero2         : 1;
} _PACKED;

struct PS2StatusRegister
{
  uint8_t outBufferFull : 1;
  uint8_t inBufferFull  : 1;
  uint8_t systemState   : 1;
  uint8_t toController  : 1;
  uint8_t specific      : 2;
  uint8_t errTimeout    : 1;
  uint8_t errParity     : 1;
} _PACKED;

static PS2ConfigByte ps2ReadConfig() {
  outb(CMD_PORT, 0x20);
  uint8_t config = inb(DATA_PORT);
  return *(PS2ConfigByte*)&config;
}

static void ps2WriteConfig(PS2ConfigByte config) {
  outb(CMD_PORT, 0x60);
  outb(DATA_PORT, *(uint8_t*)&config);
}

static PS2StatusRegister ps2ReadStatus() {
  uint8_t status = inb(STATUS_PORT);
  return *(PS2StatusRegister*)&status;
}

static void ps2FlushOutBuffer()
{
  /* don't poll the status byte here, because if
   * there's no controller, we could get stuck
   * in an endless loop. */
  inb(DATA_PORT);
}

static bool ps2SelfTest()
{
  /* send the self test command */
  outb(CMD_PORT, 0xaa);

  /* try to read the response five times, abort otherwise */
  size_t attempts = 5;
  while (attempts-- && ps2ReadStatus().outBufferFull)
    sched::yield();

  /* some PS/2 controllers apparently return 0x65 instead
   * of 0x55, so allow that, too. */
  uint8_t result = inb(DATA_PORT);
  return result == 0x55 || result == 0x65;
}

static void ps2SetPort1IrqEnable(bool enable = true)
{
  auto config = ps2ReadConfig();
  config.irqPort1 = enable;
  ps2WriteConfig(config);
}

static void ps2SetPort1Enable(bool enable = true)
{
  outb(CMD_PORT, enable ? 0xae : 0xad);
}

static void ps2SetPort2Enable(bool enable = true)
{
  outb(CMD_PORT, enable ? 0xa8 : 0xa7);
}

class PS2Keyboard : public devices::UartDevice
{
  public:
    PS2Keyboard();

    ssize_t write(char*, size_t) {
      return -ENOTSUP;
    }

    void irq() {
      char c = inb(DATA_PORT);
      receive(&c, 1);
    }
};

static void irqHandler(PS2Keyboard* kbd)
{
  kbd->irq();
}

PS2Keyboard::PS2Keyboard()
{
  ps2SetPort1Enable();
  ps2SetPort1IrqEnable();
  registerIrq(1, (void*)irqHandler, this);
}

void ps2_init()
{
  assert(sizeof(PS2ConfigByte) == 1);

  /* disable both ports during init */
  ps2SetPort1Enable(false);
  ps2SetPort2Enable(false);

  ps2FlushOutBuffer();

  /* performing a configuration update */
  auto config = ps2ReadConfig();
  bool couldBeDualChannel = config.clkPort2;
  config.irqPort1 = 0;
  config.irqPort2 = 0;
  config.translPort1 = 0;
  ps2WriteConfig(config);

  /* perform the PS2 controller self test. by now, this is the only
   * way to know whether there actually is a controller or not.
   * better: detect via ACPI */
  if (!ps2SelfTest())
  {
    debug(DEVICES) << "ps/2: self test failed. probably not present\n";
    return;
  }

  /* check if the controller supports two channels */
  bool isDualChannel = false;
  if (couldBeDualChannel)
  {
    ps2SetPort2Enable();
    config = ps2ReadConfig();
    if (config.clkPort2 == 0)
      isDualChannel = true;
    ps2SetPort2Enable(false);
  }
  debug(DEVICES) << "ps/2 channels: "
                 << (isDualChannel ? "2" : "1") << "\n";


  auto kbd = new PS2Keyboard();
  dev::registerDevice("keyboard", kbd);
}
