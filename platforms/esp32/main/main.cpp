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
    spi_config.spi_host = SPI2_HOST;
    spi_config.dc_pin = 1;      // 数据/命令引脚
    spi_config.cs_pin = 2;      // 片选引脚
    spi_config.rst_pin = 0;    // 复位引脚
    spi_config.mosi_pin = 18;   // MOSI引脚
    spi_config.sclk_pin = 19;   // SCLK引脚
    spi_config.miso_pin = -1;   // 不使用MISO
    spi_config.freq = 1000000;  // 1MHz (OLED通常频率较低)
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

// 简单测试图案1：全屏填充
void testPattern1_FullFill(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 1: Full white screen");
    driver->clear(1);  // 全白
    driver->display();
}

// 简单测试图案2：四个角落的方块
void testPattern2_CornerBlocks(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 2: Corner blocks");
    driver->clear(0);  // 清屏
    
    // 四个角落的小方块 (8x8像素)
    driver->fillRect(0, 0, 8, 8, 1);           // 左上角
    driver->fillRect(120, 0, 8, 8, 1);         // 右上角  
    driver->fillRect(0, 56, 8, 8, 1);          // 左下角
    driver->fillRect(120, 56, 8, 8, 1);        // 右下角
    
    driver->display();
}

// 简单测试图案3：水平条纹
void testPattern3_HorizontalStripes(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 3: Horizontal stripes");
    driver->clear(0);  // 清屏
    
    // 每隔4像素画一条水平线
    for (int y = 0; y < 64; y += 8) {
        driver->fillRect(0, y, 128, 4, 1);
    }
    
    driver->display();
}

// 简单测试图案4：垂直条纹
void testPattern4_VerticalStripes(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 4: Vertical stripes");
    driver->clear(0);  // 清屏
    
    // 每隔8像素画一条垂直线
    for (int x = 0; x < 128; x += 16) {
        driver->fillRect(x, 0, 8, 64, 1);
    }
    
    driver->display();
}

// 简单测试图案5：中心十字
void testPattern5_CenterCross(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 5: Center cross");
    driver->clear(0);  // 清屏
    
    // 水平线穿过中心
    driver->fillRect(0, 30, 128, 4, 1);
    // 垂直线穿过中心  
    driver->fillRect(62, 0, 4, 64, 1);
    
    driver->display();
}

// 简单测试图案6：棋盘格
void testPattern6_Checkerboard(ESP32_SPI_Driver* driver) {
    ESP_LOGI(TAG, "Test 6: Checkerboard pattern");
    driver->clear(0);  // 清屏
    
    // 8x8 棋盘格
    for (int x = 0; x < 128; x += 16) {
        for (int y = 0; y < 64; y += 16) {
            if ((x/16 + y/16) % 2 == 0) {
                driver->fillRect(x, y, 8, 8, 1);
            }
        }
    }
    
    driver->display();
}

// 运行所有测试图案
void runAllTests(ESP32_SPI_Driver* driver) {
    const int delay_ms = 3000;  // 每个测试显示3秒
    
    testPattern1_FullFill(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    testPattern2_CornerBlocks(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    testPattern3_HorizontalStripes(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    testPattern4_VerticalStripes(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    testPattern5_CenterCross(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    testPattern6_Checkerboard(driver);
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
    
    ESP_LOGI(TAG, "All test patterns completed");
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
    
    // 运行测试图案序列
    ESP_LOGI(TAG, "Starting test pattern sequence...");
    
    // ESP32应用中的无限循环
    while (1) {
        runAllTests(driver);
        ESP_LOGI(TAG, "Test cycle completed, restarting in 2 seconds...");
        vTaskDelay(pdMS_TO_TICKS(2000)); // 等待2秒后重新开始
    }
    
    // 清理资源（实际上不会到达这里）
    delete driver;
}