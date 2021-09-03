#pragma once

#include <types.h>

class Terminal
{
  public:
    Terminal();
    ~Terminal();

    ssize_t read(char* buffer, size_t len);
    ssize_t write(const char* buffer, size_t len) {
      return len;
    }

    static Terminal* getMainTerm() {
      return nullptr;
    }
};
