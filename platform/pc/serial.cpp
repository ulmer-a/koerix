#include <pc/serial.h>
#include <x86/ports.h>
#include <errno.h>
#include <debug.h>
#include <asm.h>
#include <string.h>

using namespace pc;

size_t SerialPort::s_minorCounter = 0;

SerialPort::SerialPort(SerialPort::Port port)
  : m_port(port)
  , m_minor(atomic_add(&s_minorCounter, 1))
{
  debug() << "init PC serial port #" << m_minor
          << " at port " << DEBUG_HEX << (size_t)port << "\n";

  /* generate device name */
  m_name[0] = 0;
  char int_buffer[10];
  strcpy(m_name, "uart");
  strcat(m_name, utoa(m_minor, int_buffer, 10));

  /* temporarily disable all IRQs */
  setIrqMode(false, false);

  setBaudRate(BAUD_115200);
  setMode(BITS_8, PARITY_NONE, STOP_ONE);

  /* FIFO Control Register -> +0x02 W
   * enable, clear and set to 14B threshold */
  outb(m_port + 0x02, 0b1100111);

  /* Modem Control Register -> 0x04 W
   * enable irq's and set RTS and DTR */
  outb(m_port + 0x04, 0b00001111);

  /* enable interrupts */
  //setIrqMode(true, false);
}

int SerialPort::ioctrl(size_t cmd, size_t* arg)
{
  if (cmd == CMD_SET_BAUD)
  {
    BaudRate baud;
    switch (*arg)
    {
    case 115200:  baud = BAUD_115200; break;
    case 38400:   baud = BAUD_38400;  break;
    case 19200:   baud = BAUD_19200;  break;
    case 9600:    baud = BAUD_9600;   break;
    default: return -EINVAL;
    }
    setBaudRate(baud);
    return 0;
  }

  return -ENOTSUP;
}

ssize_t SerialPort::write(char* buffer, size_t len)
{
  while (len--)
  {
    /* wait until new data can be written
     * TODO: make this more efficient, e.g. IRQs? */
    while ((inb(m_port + 0x05) & 0x20) == 0);

    /* Data Register -> +0x00 W */
    outb(m_port, *buffer++);
  }
  return len;
}

const char* SerialPort::getName() const
{
  return m_name;
}

void SerialPort::setIrqMode(bool dataReadyIrq, bool txEmptyIrq)
{
  /* Write to the Interrupt Enable Register (IER)*/
  uint8_t ier = 0x00;
  if (dataReadyIrq) ier |= 0x01;
  if (txEmptyIrq)   ier |= 0x02;
  outb(m_port, ier);
}

void SerialPort::setMode(SerialPort::BitMode bit,
    SerialPort::ParityMode parity, SerialPort::StopBitMode stop)
{
  /* To set the protocol mode, we modify the line control
   * register while preserving the DLAB bit (bit 7). */
  uint8_t dlab = inb(m_port + 3) & 0x80;
  outb(m_port + 3, dlab | bit | parity | stop);
}

void SerialPort::setBaudRate(SerialPort::BaudRate baud)
{
  /* Baud Rate can be set by taking the divisor and writing
   * it to the appropriate register. before the divisor can
   * be written, the DLAB bit has to be set. it has to be
   * cleared again afterwards to be able to read/write the
   * data register. */
  uint8_t line_ctrl = inb(m_port + 3);
  outb(m_port + 3, line_ctrl | 0x80);   // set DLAB bit
  outb(m_port + 0, baud & 0xff);        // low byte
  outb(m_port + 1, baud >> 8);          // high byte
  outb(m_port + 3, line_ctrl & 0x7f);   // clear DLAB bit

  debug() << "uart" << m_minor << ": baud_rate="
          << (115200 / baud) << "\n";
}

void SerialPort::handleIrq()
{
  char buffer[128];
  size_t bytes_received = 0;
  while (inb(m_port + 5) & 1)
  {
    if (bytes_received == 128)
    {
      onRead(buffer, bytes_received);
      bytes_received = 0;
    }

    buffer[bytes_received++] = inb(m_port);
  }
}

