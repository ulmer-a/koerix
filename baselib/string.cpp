#include <string.h>

size_t strlen(const char* str)
{
  size_t len = 0;
  while (*str++)
    len++;
  return len;
}

void *memcpy(void *dest, const void *src, size_t length)
{
  unsigned char *s1 = (unsigned char *)dest;
  const unsigned char *s2 = (const unsigned char *)src;

  while (length--)
    *s1++ = *s2++;
  return dest;
}

void *memmove(void *dest, const void *src, size_t length);
void *memccpy(void *dest, const void *src, uint8_t c, size_t length);

void *memset(void *block, uint8_t c, size_t size)
{
  unsigned char *s = (unsigned char*)block;
  while (size--)
    *s++ = c;
  return block;
}

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
  return nullptr;
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

static const char *exceptions[] = {
  "Divide by zero",
  "Debug",
  "NMI",
  "Breakpoint",
  "Overflow",
  "Bounds check",
  "Invalid opcode",
  "Device not available",
  "Double fault",
  "Coprocessor segment overrun",
  "Invalid TSS",
  "Segment not present",
  "Stack segment fault",
  "General protection fault",
  "Page fault",
  "reserved",
  "Floating-point error",
  "Alignment check",
  "Machine check",
  "SIMD floating point exception",
};

static const char *err_messages[] = {
    "success",
    "operation not permitted",
    "no such file or directory",
    "no such process",
    "interrupted system call",
    "input/output error",
    "no such device or address",
    "argument list too long",
    "exec format error",
    "bad file descriptor",
    "no child processes",
    "resource temporarily unavailable",
    "cannot allocate memory",
    "permission denied",
    "bad address",
    "block device required",
    "device or resource busy",
    "file exists",
    "invalid cross-device link",
    "no such device",
    "not a directory",
    "is a directory",
    "invalid argument",
    "too many open files in system",
    "too many open files",
    "inappropriate ioctl for device",
    "text file busy",
    "file too large",
    "no space left on device",
    "illegal seek",
    "read-only file system",
    "too many links",
    "broken pipe",
    "numerical argument out of domain",
    "numerical result out of range",
    "resource deadlock avoided",
    "file name too long",
    "no locks available",
    "function not implemented",
    "directory not empty",
    "too many levels of symbolic links",
    "resource temporarily unavailable",
    "no message of desired type",
    "identifier removed",
    "channel number out of range",
    "level 2 not synchronized",
    "level 3 halted",
    "level 3 reset",
    "link number out of range",
    "protocol driver not attached",
    "no csi structure available",
    "level 2 halted",
    "invalid exchange",
    "invalid request descriptor",
    "exchange full",
    "no anode",
    "invalid request code",
    "invalid slot",
    "resource deadlock avoided",
    "bad font file format",
    "device not a stream",
    "no data available",
    "timer expired",
    "out of streams resources",
    "machine is not on the network",
    "package not installed",
    "object is remote",
    "link has been severed",
    "advertise error",
    "srmount error",
    "communication error on send",
    "protocol error",
    "multihop attempted",
    "rfs specific error",
    "bad message",
    "value too large for defined data type",
    "name not unique on network",
    "file descriptor in bad state",
    "remote address changed",
    "can not access a needed shared library",
    "accessing a corrupted shared library",
    ".lib section in a.out corrupted",
    "attempting to link in too many shared libraries",
    "cannot exec a shared library directly",
    "invalid or incomplete multibyte or wide character",
    "interrupted system call should be restarted",
    "streams pipe error",
    "too many users",
    "socket operation on non-socket",
    "destination address required",
    "message too long",
    "protocol wrong type for socket",
    "protocol not available",
    "protocol not supported",
    "socket type not supported",
    "operation not supported",
    "protocol family not supported",
    "address family not supported by protocol",
    "address already in use",
    "cannot assign requested address",
    "network is down",
    "network is unreachable",
    "network dropped connection on reset",
    "software caused connection abort",
    "connection reset by peer",
    "no buffer space available",
    "transport endpoint is already connected",
    "transport endpoint is not connected",
    "cannot send after transport endpoint shutdown",
    "too many references: cannot splice",
    "connection timed out",
    "connection refused",
    "host is down",
    "no route to host",
    "operation already in progress",
    "operation now in progress",
    "stale file handle",
    "structure needs cleaning",
    "not a xenix named type file",
    "no xenix semaphores available",
    "is a named type file",
    "remote i/o error",
    "disk quota exceeded",
    "no medium found",
    "wrong medium type",
    "operation canceled",
    "required key not available",
    "key has expired",
    "key has been revoked",
    "key was rejected by service",
    "owner died",
    "state not recoverable",
    "operation not possible due to rf-kill",
    "memory page has hardware error",
    "operation not supported",
    "filesystem not supported",
    "interference conflict"
};

const char *strerror(unsigned err)
{
    if (err < sizeof(err_messages))
        return err_messages[err];
    return "Unknown error";
}

const char *strexcept(unsigned exc)
{
  if (exc < sizeof(exceptions))
    return exceptions[exc];
  return "unknown";
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
