#pragma once

#include <shared_ptr.h>

namespace fs {

  class FileDesc;
  class Dir;

  FileDesc open(const char* filename, int& error);

  bool mount(const char* filename, ktl::shared_ptr<Dir> fs, int& error);

}
