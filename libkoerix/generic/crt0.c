#include <unistd.h>
#include "threads/pthread_impl.h"

extern int main(int argc, char** argv);

static thread_info_t __main_thread_info = { 0 };

void __attribute__((noreturn)) _start(
    int argc, char** argv, char** envp)
{
  set_thread_ptr(&__main_thread_info);

  int ret_value = main(argc, argv);
  _exit(ret_value);
}
