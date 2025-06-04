#include "DriverFactory.h"

namespace MinimalUI {

// 静态成员初始化
std::unordered_map<DriverType, DriverFactory::CreatorFunc> DriverFactory::creators;

std::shared_ptr<GraphicsDriver> DriverFactory::createDriver(DriverType type) {
    auto it = creators.find(type);
    if (it != creators.end()) {
        return it->second();
    }
    return nullptr;
}

void DriverFactory::registerCreator(DriverType type, CreatorFunc creator) {
    creators[type] = creator;
}

} // namespace MinimalUI