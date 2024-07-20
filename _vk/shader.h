#ifndef _VK_SHADER
#define _VK_SHADER

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../general/io.h"
#include "../general/macros.h"

VkShaderModule createShaderModule(VkDevice device, const vec<char> &code) {
    shaderModuleInfo(createInfo,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    );

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }

    return shaderModule;
}

VkShaderModule createShaderModule(VkDevice device, const std::string fileName) {
    return createShaderModule(device, readFile(fileName));
}


#endif

