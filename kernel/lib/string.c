#include <lib/string.h>

void *memmove(void *dest, const void *src, size_t length);
void *memccpy(void *dest, const void *src, uint8_t c, size_t length);

#ifndef x86_64
  void *memcpy(void *dest, const void *src, size_t length)
  {
    unsigned char *s1 = (unsigned char *)dest;
    const unsigned char *s2 = (const unsigned char *)src;

    while (length--)
      *s1++ = *s2++;
    return dest;
  }

  void *memset(void *block, uint8_t c, size_t size)
  {
    unsigned char *s = (unsigned char*)block;
    while (size--)
      *s++ = c;
    return block;
  }

  size_t strlen(const char* str)
  {
    size_t len = 0;
    while (*str++)
      len++;
    return len;
  }
#endif

char *strcpy(char *dest, const char* src)
{
  const size_t len = strlen(src);
  return (char*)memcpy(dest, src, len + 1);
}

size_t strnlen(const char *src, size_t n)
{
  for (size_t i = 0; i < n; i++)
  {
    if (src[i] == 0)
      return i + 1;
  }
  return n;
}

char *strncpy(char *dest, const char* src, size_t size)
{
  char* dest_orig = dest;
  size_t len = strnlen(src, size);
  while (len--)
    *dest++ = *src++;
  return dest_orig;
}

const char* strccpy(char *dest, const char *src, char terminator)
{
  char c;
  while ((c = *(src++)))
  {
    if (c == terminator)
      break;
    *(dest++) = c;
  }
  if (c == 0)
    src--;
  *dest = 0;
  return src;
}

char *strdup(const char *src);

char *strcat(char *dest, const char* append)
{
  char* orig_dest = dest;
  while (*dest)
    dest++;
  strcpy(dest, append);
  return orig_dest;
}

char *strncat(char *dest, const char*append, size_t size);
size_t strlcat(char *dest, const char*append, size_t size);

int32_t memcmp(const void *region1, const void *region2, size_t size)
{
  const unsigned char *s1 = (const unsigned char*)region1;
  const unsigned char *s2 = (const unsigned char*)region2;

  while (size-- > 0)
  {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;
}

int32_t strcmp(const char *str1, const char *str2)
{
  while (*str1 && (*str1 == *str2))
    str1++, str2++;
  return *(const unsigned char*)str1
      - *(const unsigned char*)str2;
}

int32_t strncmp(const char *str1, const char *str2, size_t n);
void *memnotchr(const void *block, uint8_t c, size_t size);

void *memchr(const void *block, uint8_t c, size_t size)
{
  const unsigned char *src = (const unsigned char *)block;

  while (size-- > 0)
  {
    if (*src == c)
     return (void*)src;
    src++;
  }
  return NULL;
}

void *memrchr(const void *block, uint8_t c, size_t size);
char *strchr(const char* str, char c);
char *strrchr(const char* str, char c);
char* strtok(char* str, const char* delimiters);

char nchar(unsigned index)
{
  if (index < 10)
    return '0' + index;
  return 'a' + index - 10;
}

char* strrev(char *buf)
{
  unsigned long len = strlen(buf);
  for (unsigned long i = 0; i < len / 2; i++)
  {
    char tmp = buf[i];
    buf[i] = buf[len - 1 - i];
    buf[len - 1 -i] = tmp;
  }
  return buf;
}

char* itoa(int value, char * str, int base)
{
  char *bufPtr = str;
  int isNegative = 0;
  if (value < 0)
  {
    isNegative = 1;
    value = -value;
  }

  int div = base;
  while (1)
  {
    int n = value % div;
    *bufPtr++ = nchar(n);

    value /= base;
    if (value == 0)
      break;
  }

  if (isNegative)
    *bufPtr++ = '-';
  *bufPtr = 0;

  return strrev(str);
}

char* ltoa(long value, char * str, int base)
{
  char *bufPtr = str;
  int isNegative = 0;
  if (value < 0)
  {
    isNegative = 1;
    value = -value;
  }

  int div = base;
  while (1)
  {
    int n = value % div;
    *bufPtr++ = nchar(n);

    value /= base;
    if (value == 0)
      break;
  }

  if (isNegative)
    *bufPtr++ = '-';
  *bufPtr = 0;

  return strrev(str);
}

char* utoa(unsigned int value, char * str, int base)
{
  char *bufPtr = str;
  int div = base;
  while (1)
  {
    int n = value % div;
    *bufPtr++ = nchar(n);

    value /= base;
    if (value == 0)
      break;
  }

  *bufPtr = 0;
  return strrev(str);
}

char* ultoa(unsigned long value, char * str, int base)
{
  char *bufPtr = str;
  int div = base;
  while (1)
  {
    int n = value % div;
    *bufPtr++ = nchar(n);

    value /= base;
    if (value == 0)
      break;
  }

  *bufPtr = 0;
  return strrev(str);
}

char* ptoa(void* value, char *str)
{
  char *bufPtr = str;
  *bufPtr++ = '0';
  *bufPtr++ = 'x';

  int i = sizeof(void*) * 2;
  while (i--)
  {
    *bufPtr++ = nchar(((unsigned long)value
                       >> i * 4) & 0xf);
  }
  *bufPtr = 0;
  return str;
}

size_t strclen(const char *src, char terminator)
{
  size_t length = 0;
  while ((*src != terminator) && (*src != 0))
  {
    src++;
    length++;
  }
  return length;
}
