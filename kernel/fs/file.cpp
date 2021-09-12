#include <fs/file.h>
#include <fs/fd.h>

using namespace fs;

FileDesc File::open(int& error)
{
  return FileDesc(this, true);
}
