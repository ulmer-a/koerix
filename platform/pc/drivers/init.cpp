// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <platform.h>
#include <debug.h>
#include <pc/serial.h>
#include <dev/devices.h>

void timer_reset(uint16_t freqHz);

void init_drivers()
{
  debug() << "init platform drivers\n";

  /* init timer to 50hz */
  timer_reset(50);

  dev::registerDevice("uart0",
    new pc::SerialPort(pc::SerialPort::COM1)
  );
}
