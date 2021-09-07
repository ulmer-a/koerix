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
    new pc::SerialPort(pc::SerialPort::COM2)
  );
}
