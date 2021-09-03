/* Koerix OS Console Driver
 * Copyright (C) 2021 Alexander Ulmer
 *
 * This program will use the framebuffer to create a
 * text console on the screen and capture keyboard input.
 * It will write any input to stdout and print everything
 * received from stdin to the console.
 */
#include <koerix/io.h>

int main()
{
  char buffer[256];
  for (;;)
  {
    ssize_t len = read(STDIN_FD, buffer, 256);
    write(STDOUT_FD, buffer, len);
  }
}
