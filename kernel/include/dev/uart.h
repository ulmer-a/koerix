// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <queue.h>
#include <dev/chardev.h>

namespace devices {

  class UartDevice : public CharDev
  {
    public:
      UartDevice();
      ~UartDevice() = default;

      size_t getMajor();

    protected:
      ssize_t read(char *buffer, size_t len) final;

      void receive(char* buffer, size_t len);

    private:
      static size_t s_minorCounter;
      IrqContextQueue m_recvBuffer;
  };

}
