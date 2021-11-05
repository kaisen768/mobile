#ifndef ME909S_BASIC_H_
#define ME909S_BASIC_H_

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum __me909s_cmd_motion_e me909s_cmd_motion_t;

enum __me909s_cmd_motion_e {
    CMD_MOTION_SETUP = 0,
    CMD_MOTION_INQUIRE = 1,
};

#define ME909S_Debug_message 0

#if ME909S_Debug_message
    #define ME909S_DEBUG_PRINTF(x)  printf x
#else
    #define ME909S_DEBUG_PRINTF(x)
#endif

static inline bool strstr_me909s(const char *str, const char *prefix)
{
    if (!str || !prefix)
        return false;

    for (; *str != '\0' && *prefix != '\0'; str++, prefix++) {
        if (*str != *prefix)
            return false;
    }

    return *prefix == '\0';
}

static inline int hex_charator_to_value(char ch)
{
    int val = 0;

    switch (ch)
    {
        case '0' ... '9':
                val = (int)(ch - '0');
                break;
        case 'a' ... 'z':
                val = (int)(ch - 'a') + 10;
                break;
        case 'A' ... 'Z':
                val = (int)(ch - 'A') + 10;
                break;
        default:
                val = -1;
                break;
    }

    return val;
}

static inline unsigned int hexadecimal_to_uint32(const char *hex)
{
    unsigned int retval = 0;
    unsigned int tmpval = 0;

    if (!hex)
        return retval;

    while (*hex && *hex != ' ')
    {
            tmpval = hex_charator_to_value(*hex);
            retval = (retval << 4) + tmpval;
            hex++;
    }

    return retval;
}

static inline uint64_t hexadecimal_to_uint64(char *hex)
{
    uint64_t retval = 0;
    uint64_t tmpval = 0;

    if (!hex)
        return retval;

    while (*hex && *hex != ' ')
    {
            tmpval = hex_charator_to_value(*hex);
            retval = (retval << 4) + tmpval;
            hex++;
    }

    return retval;
}

static inline char *me909s_strcopy(char *dest, const char *src, size_t n)
{
    if (!dest || !src)
        return dest;
    
    if (n == 0)
        return dest;

    const char *src_h = src;
    
    /* calc space bytes of tail */
    int real_l = 0;
    if (src_h[n-1] != '\0') {
        real_l = n;
    } else {
        while (src_h[real_l++]);
        real_l -= 1;
    }

    while ((real_l-1 >= 0) && ((src_h[real_l-1] == ' ') || (src_h[real_l-1] == '\"')))
        real_l--;

    while ((*src && *src == ' ') || (*src && *src == '\"')) /* remove head spaces */
        src++;

    return (char*)memcpy((void*)dest, (const void*)src, &src_h[real_l] - src);
}

#endif // !ME909S_COMMON_TOOLCHAIN_H_

