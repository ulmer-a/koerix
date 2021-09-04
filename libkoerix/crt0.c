#include <stdlib.h>

//extern char** _envp;
extern int main(int argc, char** argv);

void __attribute__((noreturn)) _start(
    int argc, char** argv, char** envp)
{
  //_envp = envp;
  int ret_value = main(argc, argv);
  exit(ret_value);
}
