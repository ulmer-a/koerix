#pragma once

#include <types.h>
#include <dev/devfs.h>
#include <fifo.h>

namespace dev {

  class CharDev : public DeviceFile
  {
    public:
      static const size_t RECV_BUFFER_SIZE = 4096;

      CharDev(size_t major, size_t minor);

      ssize_t read(char* buffer, size_t len) final;

      FileType getType() const final { return CHARDEV; }

    protected:
      void onReceive(char* buffer, size_t len);
      static size_t getNewMajor();

    private:
      char m_recvBuffer[RECV_BUFFER_SIZE];
      Fifo m_recvFifo;
  };

}
