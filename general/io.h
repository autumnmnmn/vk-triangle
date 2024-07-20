#ifndef GENERAL_IO
#define GENERAL_IO

#include <fstream>
#include <format>

#include "../general/macros.h"

static vec<char> readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(std::format("failed to open file {}", filename));
    }

    size_t fileSize = (size_t) file.tellg();
    vec<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

// TODO glslc / spirv-as / spirv-val invocations

#endif

