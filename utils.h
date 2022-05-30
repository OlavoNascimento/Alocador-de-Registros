#pragma once

#ifdef REGALLOC_DEBUG
    #define LOG_INFO(message)                                                                   \
        std::cout << "\033[0;34mINFO\033[0m:" << __FILE__ << ":" << __LINE__ << ": " << message \
                  << std::endl;
    #define LOG_WARNING(fmt, ...)                                                                \
        std::cout << "\033[0;33mAVISO\033[0m:" << __FILE__ << ":" << __LINE__ << ": " << message \
                  << std::endl;
    #define LOG_ERROR(fmt, ...)                                                                 \
        std::cerr << "\033[0;31mERRO\033[0m:" << __FILE__ << ":" << __LINE__ << ": " << message \
                  << std::endl;
#else
    #define LOG_INFO(message)
    #define LOG_WARNING(fmt, ...)
    #define LOG_ERROR(fmt, ...)
#endif
