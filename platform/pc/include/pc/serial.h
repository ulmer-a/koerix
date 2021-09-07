#pragma once

#include <types.h>
#include <dev/chardev.h>
#include <fifo.h>

namespace pc {

  class SerialPort : public dev::CharDev
  {
    public:
      enum Port {
        COM1 = 0x3f8,
        COM2 = 0x2f8,
      };

      static const size_t RECV_BUFFER_SIZE = 4096;

    public:
      // dummy constructor for static allocation
      SerialPort();

      // normal initialization
      SerialPort(Port port, bool noIrq = false);
      ~SerialPort() = default;

      // called by the irq subsystem, dont' call!
      void handleIrq();

      // operations:
      int ioctrl(size_t cmd, size_t *arg);
      ssize_t write(char* buffer, size_t len) final;
      ssize_t read(char* buffer, size_t len) final;

      enum BitMode {
        BITS_5        = 0x00,
        BITS_6        = 0x01,
        BITS_7        = 0x02,
        BITS_8        = 0x03
      };

      enum ParityMode {
        PARITY_NONE   = 0x00,
        PARITY_ODD    = 0x08,
        PARITY_EVEN   = 0x18,
        PARITY_MARK   = 0x28,
        PARITY_SPACE  = 0x38
      };

      enum StopBitMode {
        STOP_ONE     = 0x00,
        STOP_OTHER   = 0x04
      };

      enum BaudRate {
        BAUD_115200   = 1,
        BAUD_57600    = 2,
        BAUD_38400    = 3,
        BAUD_28800    = 4,
        BAUD_19200    = 6,
        BAUD_12800    = 9,
        BAUD_11520    = 10,
        BAUD_9600     = 12
      };

      enum IoCtrlCmd {
        CMD_SET_BAUD
      };

    private:
      // modesetting -> do it via ioctl()
      void setIrqMode(bool dataReadyIrq, bool txEmptyIrq);
      void setMode(BitMode bit, ParityMode parity, StopBitMode stop);
      void setBaudRate(BaudRate baud);

      char m_recvBuffer[RECV_BUFFER_SIZE];
      Fifo m_fifo;

      // io port base
      uint16_t m_port;

      // accounting related
      static size_t getMajor();
      static size_t s_minorCounter;
  };

}
