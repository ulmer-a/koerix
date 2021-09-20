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

enum PS2DeviceTypes
{
  PS2_KEYBOARD,
  PS2_KEYBOARD_MF2,
  PS2_MOUSE,
  PS2_MOUSE_SCROLL,
  PS2_MOUSE_5BUTTON,
  OTHER
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

static void ps2SetIrqEnable(int channel, bool enable = true)
{
  auto config = ps2ReadConfig();
  if (channel == 0)
    config.irqPort1 = enable;
  else
    config.irqPort2 = enable;
  ps2WriteConfig(config);
}

static void ps2SetEnable(int channel, bool enable = true)
{
  if (channel == 0)
    outb(CMD_PORT, enable ? 0xae : 0xad);
  else
    outb(CMD_PORT, enable ? 0xa8 : 0xa7);
}

static bool ps2WaitForAck()
{
  while (!ps2ReadStatus().outBufferFull)
    sched::yield();
  return inb(DATA_PORT) == 0xfa;
}

static bool ps2WaitForByteTimeout(uint8_t& c)
{
  size_t attempts = 10;
  while (attempts-- && !ps2ReadStatus().outBufferFull)
    sched::yield();
  if (attempts == 0)
    return false;
  c = inb(DATA_PORT);
  return true;
}

static PS2DeviceTypes ps2Identify()
{
  /* identify the device */
  outb(DATA_PORT, 0xf2);
  ps2WaitForAck();

  uint8_t responseBytes = 0;
  uint8_t response[2];

  size_t maxBytes = 2;
  while (maxBytes--) {
    uint8_t c;
    if (!ps2WaitForByteTimeout(c))
      break;
    response[responseBytes++] = c;
  }

  if (responseBytes == 0)
    return PS2_KEYBOARD;
  if (responseBytes == 1)
  {
    if (response[0] == 0x00)
      return PS2_MOUSE;
    if (response[0] == 0x03)
      return PS2_MOUSE_SCROLL;
    if (response[0] == 0x04)
      return PS2_MOUSE_5BUTTON;
  }
  else
  {
    if (response[0] == 0xab)
    {
      if (response[1] == 0x83)
        return PS2_KEYBOARD_MF2;
    }
  }
  return OTHER;
}

class PS2Device : public devices::UartDevice
{
  public:
    PS2Device(int channel);

    ssize_t write(char*, size_t) {
      return -ENOTSUP;
    }

    void irq() {
      char c = inb(DATA_PORT);
      receive(&c, 1);
    }

  private:
    PS2DeviceTypes m_type;
};

static void irqHandler(PS2Device* dev)
{
  dev->irq();
}

PS2Device::PS2Device(int channel)
{
  /* enable the device, and temporarily disable
   * the other device (todo) */
  ps2SetEnable(channel, true);

  /* identify the device type */
  outb(DATA_PORT, 0xf5);
  ps2WaitForAck();
  m_type = ps2Identify();
  outb(DATA_PORT, 0xf4);
  ps2WaitForAck();

  ps2SetIrqEnable(channel, true);
  registerIrq(channel == 0 ? 1 : 12, (void*)irqHandler, this);

  const char* device_name;
  if (m_type == PS2_KEYBOARD || m_type == PS2_KEYBOARD_MF2)
    device_name = "keyboard";
  else
    device_name = "mouse";
  dev::registerDevice(device_name, this);
}

void ps2_init()
{
  assert(sizeof(PS2ConfigByte) == 1);

  /* disable both ports during init */
  ps2SetEnable(0, false);
  ps2SetEnable(1, false);

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
    ps2SetEnable(1, true);
    config = ps2ReadConfig();
    if (config.clkPort2 == 0)
      isDualChannel = true;
    ps2SetEnable(1, false);
  }
  debug(DEVICES) << "ps/2 channels: "
                 << (isDualChannel ? "2" : "1") << "\n";

  /* create device files */
  new PS2Device(0);
  // for now, disregard second channel devices
  //if (isDualChannel)
  //  new PS2Device(1);
}
