#include <koerix/keyboard.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

static int __kbd_fd = -1;

enum SpecialKeys
{
  NONE = 0x80,
  ESCAPE = 0x81,
  BACKSPACE = 0x82,
  TAB = 0x83,
  ENTER = 0x84,
  CTRL_LEFT = 0x85,
  SHIFT_LEFT = 0x86,
  SHIFT_RIGHT = 0x87,
  ALT_LEFT = 0x88,
  CAPS_LOCK = 0x89,
  F1 = 0x8a, F2 = 0x8b, F3 = 0x8c, F4 = 0x8d, F5 = 0x8e, F6 = 0x8f,
  F7 = 0x90, F8 = 0x91, F9 = 0x92, F10 = 0x93, F11 = 0x94, F12 = 0x95,
  NUM_LOCK = 0x96,
  SCROLL_LOCK = 0x97
};

static unsigned char layout_base[] = {
  NONE, F9, NONE, F5, F3, F1, F2, F12, NONE, F10, F8, F6, F4,
  TAB, '`', NONE, NONE, ALT_LEFT, SHIFT_LEFT, NONE, CTRL_LEFT,
  'q', '1', NONE, NONE, NONE, 'y', 's', 'a', 'w', '2', NONE,
  NONE, 'c', 'x', 'd', 'e', '4', '3', NONE, NONE, ' ', 'v', 'f',
  't', 'r', '5', NONE, NONE, 'n', 'b', 'h', 'g', 'y', '6', NONE,
  NONE, NONE, 'm', 'j', 'u', '7', '8', NONE, NONE, ',', 'k', 'i',
  'o', '0', '9', NONE, NONE, '.', '-', 'l', ';', 'p', 's', NONE,
  NONE, NONE, '\'', NONE, 'u', '=', NONE, NONE, CAPS_LOCK, SHIFT_RIGHT,
  '\n', '+', NONE, '\\', NONE, NONE, NONE, NONE, NONE, NONE, NONE,
  '\b', NONE, NONE, '1', NONE, '4', '7', NONE, NONE, NONE,
  '0', '.', '2', '5', '6', '8', ESCAPE, NUM_LOCK, F11, '+', '3', '-',
  '*', '9', SCROLL_LOCK, NONE, NONE, NONE, NONE, F7
};

int kbd_init()
{
  int fd = open("keyboard", O_RDONLY);
  if (fd < 0)
    return fd;
  __kbd_fd = fd;
  return 0;
}

static unsigned char do_getchar()
{
  unsigned char c;
  read(__kbd_fd, &c, 1);
  return c;
}

char kbd_getchar()
{
  unsigned char c = do_getchar();
  if (c < sizeof(layout_base))
  {
    c = layout_base[c];
    if (c < 0x80)
      return c;
    else
      return kbd_getchar();
  }
  else if (c == 0xf0)
  {
    // ignore the release
    do_getchar();
    return kbd_getchar();
  }
  else if (c == 0xe0)
  {
    // ignore the special key
    do_getchar();
    return kbd_getchar();
  }
  return 0;
}
