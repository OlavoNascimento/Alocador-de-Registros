#pragma once

#include <stdio.h>

#ifdef REGALLOC_DEBUG
    #define LOG_INFO(fmt, ...) \
        printf("\033[0;34mINFO\033[0m:%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define LOG_AVISO(fmt, ...) \
        printf("\033[0;33mAVISO\033[0m:%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
    #define LOG_ERRO(fmt, ...) \
        fprintf(stderr, "\033[0;31mERRO\033[0m:%s:%d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define LOG_INFO(fmt, ...)
    #define LOG_AVISO(fmt, ...)
    #define LOG_ERRO(fmt, ...)
#endif

int max(int a, int b);
int min(int a, int b);
