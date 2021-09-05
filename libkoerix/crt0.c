#include <unistd.h>

extern int main(int argc, char** argv);

void __attribute__((noreturn)) _start(
    int argc, char** argv, char** envp)
{
  int ret_value = main(argc, argv);
  _exit(ret_value);
}
