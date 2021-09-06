#include <sys/uio.h>
#include <unistd.h>

ssize_t writev(int fd, const struct iovec *iov, int count)
{
  /* instead of implementing the writev() system call, we just
   * implement it in user space. [ulm] */
  ssize_t len = 0;
  for (int i = 0; i < count; i++) {
    ssize_t ret = write(fd, iov[i].iov_base, iov[i].iov_len);
    if (ret < 0)
      return ret;
    len += ret;
  }
  return len;
}
