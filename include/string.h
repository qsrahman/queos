#pragma once

#include <stddef.h>

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);

char* strcpy(char* dst, const char* src);
char* strncpy(char* dst, const char* src, size_t n);
char* strcat(char* s, const char* append);
char* strncat(char* s, const char* append, size_t n);

size_t strlen(const char* s);
size_t strnlen(const char* s, size_t maxlen);

char* strstr(char* haystack, const char* needle);
char* strchr(char* s, int c);
char* strrchr(char* s, int c);

int memcmp(const void* s1, const void* s2, size_t n);
void* memset(void* dest, int val, size_t n);
void* memcpy(void* dst, const void* src, size_t n);
void* memmove(void* dst, const void* src, size_t n);
