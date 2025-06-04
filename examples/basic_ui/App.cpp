#include <iostream>
#include "GraphicsDriver.h"
#include "DriverFactory.h"

using namespace MinimalUI;

// 注册驱动创建函数
void registerDrivers() {
    // 这里通常会注册各种平台的驱动
    // 在实际应用中，这些注册通常在平台特定的代码中完成
    std::cout << "Driver registration would happen here in a real app" << std::endl;
    // 例如: DriverFactory::registerCreator(DriverType::ESP32_SPI, createESP32Driver);
}

// 创建简单的UI布局
void drawSimpleUI(std::shared_ptr<GraphicsDriver> driver) {
    // 清屏为白色背景
    driver->clear(Colors::WHITE);
    
    // 绘制标题栏
    driver->fillRect(0, 0, driver->width(), 30, Colors::BLUE);
    
    // 绘制两个按钮
    // 按钮1 - 红色
    driver->fillRect(20, 50, 80, 40, Colors::RED);
    driver->drawRect(20, 50, 80, 40, Colors::BLACK); // 边框
    
    // 按钮2 - 绿色
    driver->fillRect(140, 50, 80, 40, Colors::GREEN);
    driver->drawRect(140, 50, 80, 40, Colors::BLACK); // 边框
    
    // 绘制分隔线
    driver->drawLine(0, 110, driver->width(), 110, Colors::GRAY);
    
    // 绘制状态指示器
    driver->drawCircle(40, 150, 15, Colors::BLACK);
    driver->fillCircle(40, 150, 13, Colors::GREEN);
    
    // 绘制进度条背景
    driver->drawRect(80, 140, 140, 20, Colors::BLACK);
    
    // 绘制进度条（70%进度）
    driver->fillRect(82, 142, 98, 16, Colors::BLUE);
    
    // 绘制信息区域背景
    driver->fillRect(10, 180, driver->width() - 20, 60, Colors::CYAN);
    driver->drawRect(10, 180, driver->width() - 20, 60, Colors::BLACK);
    
    // 底部状态栏
    driver->fillRect(0, driver->height() - 20, driver->width(), 20, Colors::GRAY);
}

// 绘制交互效果示例
void simulateInteraction(std::shared_ptr<GraphicsDriver> driver) {
    // 模拟按下按钮1
    std::cout << "Simulating button 1 press..." << std::endl;
    
    // 改变按钮1外观
    driver->fillRect(20, 50, 80, 40, Colors::BLUE); // 按下时颜色变化
    driver->drawRect(20, 50, 80, 40, Colors::BLACK); // 边框
    
    // 更新状态指示器
    driver->fillCircle(40, 150, 13, Colors::RED);
    
    // 更新进度条（100%进度）
    driver->fillRect(82, 142, 136, 16, Colors::BLUE);
    
    // 更新信息区域
    driver->fillRect(10, 180, driver->width() - 20, 60, Colors::YELLOW);
    driver->drawRect(10, 180, driver->width() - 20, 60, Colors::BLACK);
}

int main() {
    std::cout << "MinimalUI Basic UI Example" << std::endl;
    
    // 注册驱动
    registerDrivers();
    
    // 在实际应用中，我们会使用工厂方法创建驱动
    // 这里为了演示，我们创建一个模拟驱动
    std::cout << "Creating a driver instance (simulated)..." << std::endl;
    
    // 假设我们成功创建了驱动
    auto driver = DriverFactory::createDriver(DriverType::ESP32_SPI);
    
    // 在实际应用中，我们需要检查驱动是否成功创建
    if (!driver) {
        std::cerr << "WARNING: Driver couldn't be created in this demo environment." << std::endl;
        std::cerr << "This is expected without proper driver registration." << std::endl;
        std::cerr << "In a real application, we would exit here." << std::endl;
        
        // 为了演示，我们继续执行，但在真实应用中应该返回错误
        // return 1;
        
        // 打印一条消息表示我们在没有真实驱动的情况下继续
        std::cout << "Continuing with simulated UI rendering..." << std::endl;
    } else {
        // 初始化驱动
        if (!driver->initialize()) {
            std::cerr << "Failed to initialize driver" << std::endl;
            return 1;
        }
        
        // 绘制UI
        std::cout << "Drawing UI..." << std::endl;
        drawSimpleUI(driver);
        
        // 模拟交互
        std::cout << "Simulating user interaction..." << std::endl;
        simulateInteraction(driver);
        
        // 刷新显示
        driver->display();
    }
    
    std::cout << "UI example completed" << std::endl;
    return 0;
}