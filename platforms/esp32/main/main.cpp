#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../components/esp32_drivers/ESP32_SPI_Driver.h"
#include "../components/esp32_drivers/controllers/SSD1309Controller.h"

using namespace MinimalUI;

static const char* TAG = "MinimalUI_ESP32";

// 创建ESP32 SPI驱动实例 (SSD1309 OLED)
MinimalUI::ESP32_SPI_Driver* createESP32Driver() {
    // 配置SPI接口
    ESP32_SPI_Config spi_config;
    spi_config.spi_host = HSPI_HOST;
    spi_config.dc_pin = 4;      // 数据/命令引脚
    spi_config.cs_pin = 5;      // 片选引脚
    spi_config.rst_pin = 16;    // 复位引脚
    spi_config.mosi_pin = 13;   // MOSI引脚
    spi_config.sclk_pin = 14;   // SCLK引脚
    spi_config.miso_pin = -1;   // 不使用MISO
    spi_config.freq = 8000000;  // 8MHz (OLED通常频率较低)
    spi_config.spi_mode = 0;    // SPI模式0

    // 配置SSD1309控制器
    SSD1309Config oled_config;
    oled_config.width = 128;
    oled_config.height = 64;
    oled_config.external_vcc = false;  // 使用内部电源
    oled_config.flip_horizontal = false;
    oled_config.flip_vertical = false;

    // 创建控制器实例
    auto controller = std::make_unique<SSD1309Controller>(oled_config);
    
    return new ESP32_SPI_Driver(spi_config, std::move(controller));
}

// 绘制示例图形 (适配SSD1309 OLED)
void drawDemoGraphics(ESP32_SPI_Driver* driver) {
    // 清屏为黑色
    driver->clear(0); // BLACK
    
    // 绘制标题区域
    driver->fillRect(0, 0, driver->width(), 8, 1); // 顶部白色条
    
    // 绘制矩形 (单色OLED只有0和1)
    driver->fillRect(10, 15, 30, 20, 1); // 实心矩形
    driver->drawRect(50, 15, 30, 20, 1);  // 空心矩形
    
    // 绘制线条
    driver->drawLine(10, 40, 118, 40, 1);  // 水平线
    driver->drawVLine(64, 45, 15, 1);      // 垂直线
    
    // 绘制圆形 (适配小屏幕)
    driver->fillCircle(30, 50, 8, 1);      // 实心圆
    driver->drawCircle(98, 50, 8, 1);      // 空心圆
    
    // 刷新显示
    driver->display();
}

// ESP32 app_main入口点
extern "C" void app_main(void) {
    ESP_LOGI(TAG, "MinimalUI ESP32 Demo Starting");
    
    // 创建ESP32 SPI驱动
    ESP32_SPI_Driver* driver = createESP32Driver();
    
    if (!driver) {
        ESP_LOGE(TAG, "Failed to create driver");
        return;
    }
    
    // 初始化驱动
    if (!driver->initialize()) {
        ESP_LOGE(TAG, "Failed to initialize driver");
        delete driver;
        return;
    }
    
    ESP_LOGI(TAG, "Driver initialized successfully");
    
    // 绘制演示图形
    drawDemoGraphics(driver);
    
    ESP_LOGI(TAG, "Graphics demo completed");
    
    // ESP32应用中的无限循环
    while (1) {
        // 可以在这里添加其他任务
        // 例如：处理用户输入、更新显示等
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
    }
    
    // 清理资源（实际上不会到达这里）
    delete driver;
}