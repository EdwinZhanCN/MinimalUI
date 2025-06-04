#pragma once

#include "../../framework/include/GraphicsDriver.h"
#include <driver/spi_master.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <cstdint>
#include <memory>

namespace MinimalUI {

// 前向声明
class DisplayController;

/**
 * @brief ESP32 SPI驱动配置结构
 */
struct ESP32_SPI_Config {
    spi_host_device_t spi_host = HSPI_HOST;  // SPI主机
    int8_t dc_pin;      // 数据/命令引脚
    int8_t cs_pin;      // 片选引脚
    int8_t rst_pin;     // 复位引脚
    int8_t mosi_pin;    // MOSI引脚
    int8_t sclk_pin;    // SCLK引脚
    int8_t miso_pin = -1; // MISO引脚，通常不需要
    uint32_t freq;      // SPI频率
    uint8_t spi_mode = SPI_MODE0; // SPI模式
};

/**
 * @class ESP32_SPI_Driver
 * @brief ESP32平台的通用SPI显示驱动
 * 支持可插拔的显示控制器
 */
class ESP32_SPI_Driver : public GraphicsDriver {
public:
    /**
     * @brief 构造函数
     * @param config SPI配置
     * @param controller 显示控制器实现
     */
    ESP32_SPI_Driver(const ESP32_SPI_Config& config, std::unique_ptr<DisplayController> controller);

    /**
     * @brief 析构函数
     */
    ~ESP32_SPI_Driver() override;

    // 实现GraphicsDriver接口
    bool initialize() override;
    void drawPixel(int16_t x, int16_t y, Color color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) override;
    void drawHLine(int16_t x, int16_t y, int16_t w, Color color) override;
    void drawVLine(int16_t x, int16_t y, int16_t h, Color color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) override;
    void drawCircle(int16_t x0, int16_t y0, int16_t r, Color color) override;
    void fillCircle(int16_t x0, int16_t y0, int16_t r, Color color) override;
    void drawChar(int16_t x, int16_t y, char c, Color color, Color bg, uint8_t size = 1) override;
    void display() override;
    void clear(Color color = 0x0000) override;
    int16_t width() const override;
    int16_t height() const override;

    // SPI通信方法 - 供控制器使用
    void sendCommand(uint8_t cmd);
    void sendData(uint8_t data);
    void sendBuffer(const uint8_t* buffer, size_t size);

protected:
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners,
                         int16_t delta, Color color) override;

private:
    ESP32_SPI_Config config_;
    spi_device_handle_t spi_;
    std::unique_ptr<DisplayController> controller_;

    // 初始化SPI硬件
    bool initSPI();

    // 释放SPI资源
    void freeSPI();

    // 创建SPI事务
    spi_transaction_t createTransaction(const void* data, size_t length);

    // 颜色转换辅助方法
    void convertColor(Color color, uint8_t* buffer, uint8_t pixel_size);
};

} // namespace MinimalUI
