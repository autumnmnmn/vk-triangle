#ifndef GENERAL_MACROS
#define GENERAL_MACROS

#include <iostream>
#include <vector>
#include <optional>
#include <stdexcept>

template<class T>
using vec = std::vector<T>;

template<class T>
using opt = std::optional<T>;

#define STDOUT(x) std::cout << x << std::endl
#define STDERR(x) std::cerr << x << std::endl
#define THROW(x) throw std::runtime_error(x)

#endif

