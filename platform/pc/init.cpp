#include <platform.h>
#include <debug.h>
#include <pc/serial.h>

void timer_reset(uint16_t freqHz);

void init_drivers()
{
  debug() << "init platform drivers\n";

  /* init timer to 50hz */
  timer_reset(50);

  /* initialize serial port drivers */
  new pc::SerialPort(pc::SerialPort::COM1);
  new pc::SerialPort(pc::SerialPort::COM2);
}
