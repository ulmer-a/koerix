#pragma once

#include <types.h>

class Fifo
{
  public:
    Fifo(size_t size);
    Fifo(char* buffer, size_t size);
    ~Fifo();

    void put(char c, bool lock = true);
    char get();

    ssize_t read(char *buffer, size_t len);
    ssize_t write(char *buffer, size_t len);

  private:
    char* m_data;
    size_t m_available;
    const size_t m_size;

    size_t m_readPtr;
    size_t m_writePtr;
};

