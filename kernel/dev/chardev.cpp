// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <dev/chardev.h>
#include <arch/asm.h>
#include <errno.h>

namespace dev {

  CharDev::CharDev(size_t major, size_t minor)
    : DeviceFile(major, minor)
    , m_recvFifo(m_recvBuffer, RECV_BUFFER_SIZE)
  {

  }

  ssize_t CharDev::read(char* buffer, size_t len)
  {
    return m_recvFifo.read(buffer, len);
  }

  void CharDev::onReceive(char* buffer, size_t len)
  {
    m_recvFifo.write(buffer, len);
  }

  size_t CharDev::getNewMajor()
  {
    static size_t majorCounter = 0;
    return atomic_add(&majorCounter, 1);
  }

}
