#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
  pid_t pid = fork();
  if (pid == 0)
  {
    printf("hello from the child with PID %d\n", getpid());
  }
  else if (pid > 0)
  {
    printf("hello from the parent with PID %d, child has %d\n", getpid(), pid);
  }
  else
  {
    printf("error: fork(): %s\n", strerror(errno));
  }

  return 0;
}
