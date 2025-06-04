# 基础UI示例

# 检查是否有App.cpp，如果没有创建一个简单的示例
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/App.cpp)
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
"#include <iostream>
#include \"GraphicsDriver.h\"
#include \"DriverFactory.h\"

using namespace MinimalUI;

int main() {
    std::cout << \"MinimalUI Basic Example\" << std::endl;
    
    // 创建驱动
    auto driver = DriverFactory::createDriver(DriverType::ESP32_SPI);
    
    if (!driver) {
        std::cerr << \"Failed to create driver\" << std::endl;
        return 1;
    }
    
    // 初始化驱动
    if (!driver->initialize()) {
        std::cerr << \"Failed to initialize driver\" << std::endl;
        return 1;
    }
    
    // 清屏
    driver->clear(Colors::WHITE);
    
    // 绘制基本UI
    driver->fillRect(0, 0, driver->width(), 40, Colors::BLUE);
    driver->fillRect(10, 60, 100, 40, Colors::RED);
    driver->fillRect(130, 60, 100, 40, Colors::GREEN);
    
    driver->drawLine(10, 120, 230, 120, Colors::BLACK);
    
    driver->drawCircle(60, 180, 40, Colors::MAGENTA);
    driver->fillCircle(180, 180, 40, Colors::CYAN);
    
    // 刷新显示
    driver->display();
    
    std::cout << \"UI rendering completed\" << std::endl;
    return 0;
}")
endif()

# 添加可执行文件
add_executable(basic_ui App.cpp)

# 链接必要的库
target_link_libraries(basic_ui
    PRIVATE
        MinimalUI::framework_core
        MinimalUI::ui_components
)

# 根据目标平台链接不同的驱动库
if(TARGET_PLATFORM STREQUAL "esp32")
    # ESP32 platform: Check if we're in ESP-IDF build context or regular CMake
    if(DEFINED ESP_PLATFORM)
        # ESP-IDF context - the target exists
        target_link_libraries(basic_ui PRIVATE MinimalUI::esp32_drivers)
    else()
        # Regular CMake context - link to the IDE helper target instead
        # and add a message indicating this is just for IDE support
        if(TARGET esp32_ide_helper)
            target_link_libraries(basic_ui PRIVATE esp32_ide_helper)
            message(STATUS "Linking basic_ui to esp32_ide_helper for IDE support only")
        else()
            message(STATUS "ESP32 drivers not available in this build context - skipping link")
            # Add compiler definition to disable ESP32-specific code when building in this context
            target_compile_definitions(basic_ui PRIVATE ESP32_DRIVERS_NOT_AVAILABLE=1)
        endif()
    endif()
elseif(TARGET_PLATFORM STREQUAL "stm32")
    target_link_libraries(basic_ui PRIVATE MinimalUI::stm32_drivers)
elseif(TARGET_PLATFORM STREQUAL "jetson")
    target_link_libraries(basic_ui PRIVATE MinimalUI::jetson_drivers)
endif()

# 安装规则
install(TARGETS basic_ui RUNTIME DESTINATION bin/examples)