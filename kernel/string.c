#include <string.h>

void* memset(void* dest, int val, size_t len) {
    unsigned char* temp = (unsigned char*)dest;
    for (; len != 0; len--) *temp++ = (unsigned char)val;
    return dest;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* ptr1 = (const unsigned char*)s1;
    const unsigned char* ptr2 = (const unsigned char*)s2;
    size_t i = 0;

    while (i < n) {
        if (ptr1[i] == ptr2[i])
            i++;
        else if (ptr1[i] < ptr2[i])
            return -1;
        else
            return 1;
    }
    return 0;
}

void* memcpy(void* dst, const void* src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    }
    return dst;
}

void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*)dstptr;
    const unsigned char* src = (const unsigned char*)srcptr;

    if (dst < src) {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--)
            dst[i - 1] = src[i - 1];
    }
    return dstptr;
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
