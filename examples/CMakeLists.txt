# 示例应用

# 检查目录结构
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/basic_ui)
    message(STATUS "Creating basic_ui example directory")
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/basic_ui)
endif()

# 添加示例子目录(如果有的话)
file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_DIR})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_DIR}/CMakeLists.txt)
            add_subdirectory(${EXAMPLE_DIR})
        else()
            message(STATUS "Example directory ${EXAMPLE_DIR} needs a CMakeLists.txt")
        endif()
    endif()
endforeach()

# 如果没有examples，创建一个示例
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/basic_ui/CMakeLists.txt)
    message(STATUS "Creating basic example CMakeLists.txt")
    file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/basic_ui/CMakeLists.txt
"# 基础UI示例

# 如果没有App.cpp，创建一个简单的示例
if(NOT EXISTS \${CMAKE_CURRENT_SOURCE_DIR}/App.cpp)
    file(WRITE \${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
\"#include <iostream>
#include \\\"Component.h\\\"

int main() {
    std::cout << \\\"MinimalUI Basic Example\\\" << std::endl;
    return 0;
}
\")
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
if(TARGET_PLATFORM STREQUAL \\\"esp32\\\")
    target_link_libraries(basic_ui PRIVATE MinimalUI::esp32_drivers)
elseif(TARGET_PLATFORM STREQUAL \\\"stm32\\\")
    target_link_libraries(basic_ui PRIVATE MinimalUI::stm32_drivers)
elseif(TARGET_PLATFORM STREQUAL \\\"jetson\\\")
    target_link_libraries(basic_ui PRIVATE MinimalUI::jetson_drivers)
endif()

# 安装规则
install(TARGETS basic_ui RUNTIME DESTINATION bin/examples)
")
endif()