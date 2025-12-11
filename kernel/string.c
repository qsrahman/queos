#include <string.h>

void* memset(void* dest, char val, size_t len) {
    char* temp = (char*)dest;
    for (; len != 0; len--) *temp++ = val;
    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    char* ptr1 = (char*)s1;
    char* ptr2 = (char*)s2;
    int i = 0;

    while (i < (int)n)
        if (ptr1[i] == ptr2[i])
            i++;
        else if (ptr1[i] < ptr2[i])
            return (-1);
        else
            return 1;
    return 0;
}

void* memcpy(void* dst, const void* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
    return dst;
}

void* memmove(void* dst, const void* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((char*)dst)[i] = ((char*)src)[i];
    }
    return dst;
}

size_t strlen(const char* str) {
    size_t retval;
    for (retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

size_t strnlen(const char* s, size_t size) {
    int n = 0;
    for (; size > 0 && *s != '\0'; s++, size--) n++;
    return n;
}

char* strcpy(char* dst, const char* src) {
    int i;

    for (i = 0; src[i] != '\0'; i++) dst[i] = src[i];
    dst[i] = '\0';

    return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dst[i] = src[i];
        if (src[i] == '\0')
            break;
    }
    return dst;
}

char* strcat(char* s, const char* append) {
    strcpy(s + strlen(s), append);
    return s;
}

char* strncat(char* s, const char* append, size_t n) {
    strncpy(s + strlen(s), append, n);
    return s;
}

char* strchr(char* s, int c) {
    int i;

    for (i = 0; s[i] != '\0'; i++) {
        if (c == s[i])
            break;
    }
    if (s[i] == '\0')
        return NULL;
    return s + i;
}

int strcmp(const char* s1, const char* s2) {
    int i = 0;

    for (;;) {
        if (s1[i] == '\0' && s2[i] == '\0')
            return 0;
        else if (s1[i] == '\0' && s2[i] != '\0')
            return (-1);
        else if (s1[i] != '\0' && s2[i] == '\0')
            return 1;

        if (s1[i] == s2[i])
            i++;
        else if (s1[i] < s2[i])
            return (-1);
        else
            return 1;
    }
}

int strncmp(const char* s1, const char* s2, size_t n) {
    size_t i = 0;

    while (i < n) {
        if (s1[i] == '\0' && s2[i] == '\0')
            return 0;
        else if (s1[i] == '\0' && s2[i] != '\0')
            return (-1);
        else if (s1[i] != '\0' && s2[i] == '\0')
            return 1;

        if (s1[i] == s2[i])
            i++;
        else if (s1[i] < s2[i])
            return (-1);
        else
            return 1;
    }
    return 0;
}

char* strrchr(char* s, int c) {
    int i, len;

    len = strlen(s);
    if (len == 0)
        return NULL;

    for (i = len - 1; i >= 0; i--)
        if (c == s[i])
            break;
    if (i < 0)
        return NULL;
    return s + i;
}

char* strstr(char* haystack, const char* needle) {
    int haystacklen, i, j, needlelen;

    haystacklen = strlen(haystack);
    needlelen = strlen(needle);
    if (needlelen > haystacklen)
        return NULL;

    for (i = 0; i < haystacklen - needlelen; i++) {
        for (j = 0; j < needlelen; j++)
            if (needle[j] != haystack[i + j])
                break;
        if (j == needlelen)
            return haystack + i;
    }
    return NULL;
}
