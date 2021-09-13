#pragma once

#include <fs/file.h>

class StdIO : public fs::File
{
  public:
    ~StdIO() = default;
    static StdIO& get();

    fs::FileDesc getFd();

  protected:
    ssize_t read(char* buf, size_t len) final;
    ssize_t write(char* buf, size_t len) final;

  private:
    StdIO() = default;
    static StdIO* s_stdio;
};
