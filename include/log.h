#ifndef _CUDA_LOG_H
#define _CUDA_LOG_H

#include <iostream>

namespace cuda {

#define RED   "\e[1;31m"    // Log Fatal & Log error
#define YELLOW "\e[1;33m"   // Log warning
#define WHITE "\e[1;37m"    // Log info
#define DEFA  "\e[0m"

#define LOG_FATAL(msg) do { \
    std::cerr << RED << "FATAL : " << msg << DEFA << std::endl; \
    throw std::runtime_error(""); \
} while (0)

#define LOG_ERROR(msg) do { \
    std::cerr << RED << "ERROR : " << msg << DEFA << std::endl; \
    throw std::runtime_error(""); \
} while (0)

#define LOG_WARN(msg) do { std::cout << YELLOW << "WARNING : " << msg << DEFA << std::endl; } while (0)
#define LOG_INFO(msg) do { std::cout << WHITE << "INFO : " << msg << DEFA << std::endl; } while (0)

}

#endif