#include <platform.h>
#include <debug.h>
#include <pc/serial.h>

void init_drivers()
{
  debug() << "init platform drivers\n";

  /* initialize serial port drivers */
  new pc::SerialPort(pc::SerialPort::COM1);
  new pc::SerialPort(pc::SerialPort::COM2);
}
