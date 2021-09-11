#include <unistd.h>
#include <stdio.h>
#include "threads/pthread_impl.h"

extern int main(int argc, char** argv);

void __attribute__((noreturn)) _start(
    int argc, char** argv, char** envp)
{
  thread_info_t __main_thread_info;
  __main_thread_info.tid = 0;
  set_thread_ptr(&__main_thread_info);

  int ret_value = main(argc, argv);

  fflush(stdout);
  fflush(stderr);
  _exit(ret_value);
}
