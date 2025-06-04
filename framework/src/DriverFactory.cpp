// framework/src/DriverFactory.cpp
#include "../include/DriverFactory.h"
#include <unordered_map>

namespace {
    std::unordered_map<DriverFactory::Platform, DriverFactory::CreatorFunc> creators;
}

GraphicsDriver* DriverFactory::createDriver(Platform platform) {
    if (creators.find(platform) != creators.end()) {
        return creators[platform]();
    }
    return nullptr; // 或者抛出异常
}

void DriverFactory::registerCreator(Platform platform, CreatorFunc creator) {
    creators[platform] = creator;
}
