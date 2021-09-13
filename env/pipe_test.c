#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

const char* msg1 = "Hello, world!\n";
const char* msg2 = "Test123";
const char* msg3 = "Test123Hello, world!\nTest123";

int main()
{
  int fds[2];
  char buffer[512];
  int sret = pipe(fds);
  assert(sret == 0);

  int readFd = fds[0], writeFd = fds[1];

  ssize_t ret = write(writeFd, msg1, strlen(msg1));
  assert(ret = strlen(msg1));

  ssize_t len = read(readFd, buffer, 512);
  assert(len == strlen(msg1));
  buffer[len] = 0;
  assert(strcmp(buffer, msg1) == 0);

  ret = write(writeFd, msg2, strlen(msg2));
  assert(ret = strlen(msg2));
  ret = write(writeFd, msg1, strlen(msg1));
  assert(ret = strlen(msg1));
  ret = write(writeFd, msg2, strlen(msg2));
  assert(ret = strlen(msg2));

  len = read(readFd, buffer, 512);
  assert(len == strlen(msg3));
  buffer[len] = 0;
  assert(strcmp(buffer, msg3) == 0);

  printf("All pipe tests passed\n");
  return 0;
}
