cmake_minimum_required(VERSION 3.16)
project(MinimalUI VERSION 0.1.0 LANGUAGES CXX)

# 启用C++17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 目标平台检测（如果没有定义则默认为esp32）
if(NOT DEFINED TARGET_PLATFORM)
    set(TARGET_PLATFORM "esp32")
endif()
message(STATUS "Building for platform: ${TARGET_PLATFORM}")

# 公共编译选项和定义
add_compile_options(-Wall -Wextra)

# 是否启用测试
option(BUILD_TESTS "Build test programs" OFF)

# 是否启用示例
option(BUILD_EXAMPLES "Build example programs" ON)

# 是否启用文档
option(BUILD_DOCS "Build documentation" OFF)

# 添加核心框架库目录
add_subdirectory(framework)

# 添加组件目录（如果存在）
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/components)
    add_subdirectory(components)
endif()

# 为CLion IDE创建虚拟目标，以便于代码补全和导航
# 这个目标不会被实际构建，仅用于IDE识别
if(TARGET_PLATFORM STREQUAL "esp32")
    # 收集ESP32平台相关的源文件
    file(GLOB_RECURSE ESP32_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/components/*/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/components/*/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/components/*/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/main/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/main/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/main/*.h"
    )

    # 创建IDE辅助目标
    add_library(esp32_ide_helper STATIC EXCLUDE_FROM_ALL ${ESP32_SOURCES})

    # 设置包含目录
    target_include_directories(esp32_ide_helper PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/components"
        "${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32/main"
        "${CMAKE_CURRENT_SOURCE_DIR}/framework/include"
        # Add ESP-IDF include paths if available
        "$ENV{IDF_PATH}/components/driver/include"
        "$ENV{IDF_PATH}/components/esp_common/include"
        "$ENV{IDF_PATH}/components/esp_system/include"
        "$ENV{IDF_PATH}/components/freertos/FreeRTOS-Kernel/include"
        "$ENV{IDF_PATH}/components/soc/include"
        "$ENV{IDF_PATH}/components/hal/include"
        "$ENV{IDF_PATH}/components/spi_flash/include"
    )

    # 设置编译定义以匹配ESP-IDF
    target_compile_definitions(esp32_ide_helper PRIVATE
        PLATFORM_ESP32=1
        CONFIG_UI_MAX_ELEMENTS=32
    )
endif()

# 根据平台添加平台特定代码
if(TARGET_PLATFORM STREQUAL "esp32")
    # ESP32 特殊处理 - 不直接添加子目录，而是提供构建指令
    if(DEFINED ENV{IDF_PATH})
        message(STATUS "ESP-IDF environment detected")
        message(STATUS "To build for ESP32, please use:")
        message(STATUS "  cd platforms/esp32")
        message(STATUS "  idf.py build")

        # 创建便捷的构建目标
        add_custom_target(esp32_build
            COMMAND ${CMAKE_COMMAND} -E echo "Switching to ESP32 directory and building..."
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/platforms/esp32 && idf.py build
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Building ESP32 project with ESP-IDF"
        )

        # 不要添加 ESP32 子目录到常规 CMake 构建中
        # add_subdirectory(platforms/esp32)  # 注释掉这行
    else()
        message(WARNING "IDF_PATH not set. ESP32 build requires ESP-IDF environment")
        message(STATUS "Please set up ESP-IDF and use 'cd platforms/esp32 && idf.py build'")
    endif()
elseif(TARGET_PLATFORM STREQUAL "stm32")
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platforms/stm32)
        add_subdirectory(platforms/stm32)
    else()
        message(FATAL_ERROR "STM32 platform directory not found")
    endif()
elseif(TARGET_PLATFORM STREQUAL "jetson")
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/platforms/jetson)
        add_subdirectory(platforms/jetson)
    else()
        message(FATAL_ERROR "Jetson platform directory not found")
    endif()
else()
    message(FATAL_ERROR "Unsupported platform: ${TARGET_PLATFORM}")
endif()

# 如果启用了示例，添加示例目录
if(BUILD_EXAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples)
    add_subdirectory(examples)
endif()

# 如果启用了测试，添加测试目录
if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    enable_testing()
    add_subdirectory(tests)
endif()

# 打印构建信息
message(STATUS "MinimalUI configuration:")
message(STATUS "  Target platform: ${TARGET_PLATFORM}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build docs: ${BUILD_DOCS}")
