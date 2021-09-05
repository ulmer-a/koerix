#pragma once

#pragma once

#include <types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);
size_t strnlen(const char *src, size_t n);
void *memcpy(void *dest, const void *src, size_t length);
void *memmove(void *dest, const void *src, size_t length);
void *memccpy(void *dest, const void *src, uint8_t c, size_t length);
void *memset(void *block, uint8_t c, size_t size);
char *strcpy(char *dest, const char* src);
char *strncpy(char *dest, const char* src, size_t size);
char *strdup(const char *src);
char *strcat(char *dest, const char*append);
char *strncat(char *dest, const char*append, size_t size);
size_t strlcat(char *dest, const char*append, size_t size);
int32_t memcmp(const void *region1, const void *region2, size_t size);
int32_t strcmp(const char *str1, const char *str2);
int32_t strncmp(const char *str1, const char *str2, size_t n);
void *memnotchr(const void *block, uint8_t c, size_t size);
void *memchr(const void *block, uint8_t c, size_t size);
void *memrchr(const void *block, uint8_t c, size_t size);
char *strchr(const char* str, char c);
char *strrchr(const char* str, char c);
char* strtok(char* str, const char* delimiters);
char* itoa(int value, char * str, int base);
char* ptoa(void *value, char * str);
char* ltoa(long value, char * str, int base);
char* utoa(unsigned int value, char * str, int base);
char* ultoa(unsigned long value, char * str, int base);
const char* strccpy(char *dest, const char *src, char terminator);
size_t strclen(const char* src, char terminator);
const char *strexcept(unsigned exc);
const char *strerror(unsigned err);

extern void sprintf(char* dest, const char* fmt, ...);
extern void _sprintf(char* dest, const char* fmt, va_list args);

#ifdef __cplusplus
}
#endif
