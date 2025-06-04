# ESP32 ESP-IDF 项目配置
cmake_minimum_required(VERSION 3.16)

# 设置额外组件目录
set(EXTRA_COMPONENT_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/../../framework"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../components"
    "${CMAKE_CURRENT_SOURCE_DIR}/components"
)

# 包含 ESP-IDF 项目系统
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# 定义项目 (必须在include之后)
project(MinimalUI_ESP32)

# 设置项目特定的配置
idf_build_set_property(COMPILE_OPTIONS "-Wall;-Wextra" APPEND)
idf_build_set_property(COMPILE_DEFINITIONS "PLATFORM_ESP32=1" APPEND)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/components")
