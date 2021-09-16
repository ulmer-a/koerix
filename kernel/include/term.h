// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer <alexulmer1999@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <types.h>
#include <fs/fd.h>
#include <lib/shared_ptr.h>

class Terminal
{
  public:
    Terminal(fs::FileDesc& fd);
    ~Terminal() = default;

    static void init();

    ssize_t read(char* buffer, size_t len);
    ssize_t write(char* buffer, size_t len);

    static inline Terminal* getMainTerm() {
      return s_mainTerm.get();
    }

    static inline void setMainTerm(lib::shared_ptr<Terminal> term) {
      s_mainTerm = term;
    }

  private:
    fs::FileDesc m_fd;
    static lib::shared_ptr<Terminal> s_mainTerm;
};
