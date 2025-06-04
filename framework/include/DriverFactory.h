#pragma once

#include "GraphicsDriver.h"
#include <memory>
#include <functional>
#include <unordered_map>

namespace MinimalUI {

/**
 * @class DriverFactory
 * @brief 驱动工厂类，负责创建不同平台的图形驱动
 */
class DriverFactory {
public:
    using CreatorFunc = std::function<std::shared_ptr<GraphicsDriver>()>;

    /**
     * @brief 创建指定类型的驱动
     * @param type 驱动类型
     * @return 图形驱动的智能指针，如果创建失败则返回nullptr
     */
    static std::shared_ptr<GraphicsDriver> createDriver(DriverType type);

    /**
     * @brief 注册驱动创建函数
     * @param type 驱动类型
     * @param creator 创建函数
     */
    static void registerCreator(DriverType type, CreatorFunc creator);

private:
    static std::unordered_map<DriverType, CreatorFunc> creators;
};

} // namespace MinimalUI