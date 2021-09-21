#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <alloca.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

extern char** __environ;

const char* banner =
"\n     _  __               _         ___  ____\n"
"    | |/ /___   ___ _ __(_)_  __  / _ \\/ ___|\n"
"    | ' // _ \\ / _ \\ '__| \\ \\/ / | | | \\___ \\\n"
"    | . \\ (_) |  __/ |  | |>  <  | |_| |___) |\n"
"    |_|\\_\\___/ \\___|_|  |_/_/\\_\\  \\___/|____/\n\n\n";

static void parseCommand(const char* cmd)
{
  if (strcmp(cmd, "exit") == 0)
    exit(EXIT_SUCCESS);

  pid_t pid = fork();
  if (pid == 0)
  {
    char* const args[] = { NULL };
    if (execve(cmd, args, __environ) < 0)
    {
      fprintf(stderr, "error: cannot start program: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  int wstatus;
  waitpid(pid, &wstatus, 0);
}

int main()
{
  printf("%s", banner);

  const size_t bufferSize = 4096;
  char* const cmdBuffer = alloca(bufferSize);
  for (;;)
  {
    printf("$ ");
    fflush(stdout);

    char c;
    char* buffer = cmdBuffer;
    while ((c = getchar()) != '\n' &&  c != '\r') {
      *buffer++ = c;
      putchar(c);
    }
    putchar('\n');
    *buffer = 0;

    if (buffer == cmdBuffer)
      continue;

    parseCommand(cmdBuffer);
  }
}
