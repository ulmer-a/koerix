#include <dev/uart.h>
#include <arch/asm.h>

using namespace devices;

size_t UartDevice::s_minorCounter = 0;

UartDevice::UartDevice()
  : CharDev(getMajor(), atomic_add(&s_minorCounter, 1))
{

}

ssize_t UartDevice::read(char* buffer, size_t len)
{
  return m_recvBuffer.read(buffer, len);
}

void UartDevice::receive(char* buffer, size_t len)
{
  m_recvBuffer.write(buffer, len);
}

size_t UartDevice::getMajor()
{
  static size_t myMajor = (size_t)-1;
  if (myMajor == (size_t)-1)
    myMajor = CharDev::getNewMajor();
  return myMajor;
}
