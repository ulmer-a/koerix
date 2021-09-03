#pragma once

#include <types.h>
#include <fs/fd.h>

class Terminal
{
  public:
    Terminal(fs::FileDesc& fd);
    ~Terminal() = default;

    ssize_t read(char* buffer, size_t len);
    ssize_t write(char* buffer, size_t len);

    static inline Terminal* getMainTerm() {
      return s_mainTerm;
    }

    static inline void setMainTerm(Terminal* term) {
      s_mainTerm = term;
    }

  private:
    fs::FileDesc m_fd;
    static Terminal* s_mainTerm;
};
