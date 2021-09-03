#pragma once

#include <types.h>
#include <fs/async_file.h>

namespace dev {

  class CharDev : public fs::AsyncFile
  {
    public:
      CharDev() = default;
      virtual ~CharDev() = default;
  };

}
