#pragma once

#include "DisplayController.h"
#include <iostream>

namespace MinimalUI {

/**
 * @brief SSD1309 OLED显示控制器配置
 */
struct SSD1309Config {
    int16_t width = 128;        // 显示宽度
    int16_t height = 64;        // 显示高度
    uint8_t i2c_address = 0x3C; // I2C地址（如果使用I2C）
    bool external_vcc = false;  // 是否使用外部VCC
    bool flip_horizontal = false; // 水平翻转
    bool flip_vertical = false;   // 垂直翻转
};

/**
 * @brief SSD1309 OLED显示控制器实现
 * 支持128x64单色OLED显示器
 */
class SSD1309Controller : public DisplayController {
public:
    explicit SSD1309Controller(const SSD1309Config& config);
    virtual ~SSD1309Controller() = default;

    // 实现DisplayController接口
    bool initialize(ESP32_SPI_Driver* spi_driver) override;
    void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) override;
    void writePixelData(const uint8_t* data, size_t length) override;
    void clearScreen() override;
    void refresh() override;
    int16_t getWidth() const override { return config_.width; }
    int16_t getHeight() const override { return config_.height; }
    uint8_t getPixelSize() const override { return 1; } // 1位单色

    /**
     * @brief 设置单个像素
     * @param x X坐标
     * @param y Y坐标
     * @param color 颜色（0=黑，1=白）
     */
    void setPixel(int16_t x, int16_t y, bool color);

    /**
     * @brief 获取帧缓冲区
     * @return 指向帧缓冲区的指针
     */
    uint8_t* getFrameBuffer() { return frame_buffer_; }

private:
    SSD1309Config config_;
    uint8_t* frame_buffer_;     // 帧缓冲区
    size_t buffer_size_;        // 缓冲区大小
    bool dirty_;                // 是否需要刷新

    // SSD1309命令定义
    static constexpr uint8_t SSD1309_SETCONTRAST = 0x81;
    static constexpr uint8_t SSD1309_DISPLAYALLON_RESUME = 0xA4;
    static constexpr uint8_t SSD1309_DISPLAYALLON = 0xA5;
    static constexpr uint8_t SSD1309_NORMALDISPLAY = 0xA6;
    static constexpr uint8_t SSD1309_INVERTDISPLAY = 0xA7;
    static constexpr uint8_t SSD1309_DISPLAYOFF = 0xAE;
    static constexpr uint8_t SSD1309_DISPLAYON = 0xAF;
    static constexpr uint8_t SSD1309_SETDISPLAYOFFSET = 0xD3;
    static constexpr uint8_t SSD1309_SETCOMPINS = 0xDA;
    static constexpr uint8_t SSD1309_SETVCOMDETECT = 0xDB;
    static constexpr uint8_t SSD1309_SETDISPLAYCLOCKDIV = 0xD5;
    static constexpr uint8_t SSD1309_SETPRECHARGE = 0xD9;
    static constexpr uint8_t SSD1309_SETMULTIPLEX = 0xA8;
    static constexpr uint8_t SSD1309_SETLOWCOLUMN = 0x00;
    static constexpr uint8_t SSD1309_SETHIGHCOLUMN = 0x10;
    static constexpr uint8_t SSD1309_SETSTARTLINE = 0x40;
    static constexpr uint8_t SSD1309_MEMORYMODE = 0x20;
    static constexpr uint8_t SSD1309_COLUMNADDR = 0x21;
    static constexpr uint8_t SSD1309_PAGEADDR = 0x22;
    static constexpr uint8_t SSD1309_COMSCANINC = 0xC0;
    static constexpr uint8_t SSD1309_COMSCANDEC = 0xC8;
    static constexpr uint8_t SSD1309_SEGREMAP = 0xA0;
    static constexpr uint8_t SSD1309_CHARGEPUMP = 0x8D;

    // 私有方法
    void sendCommand(uint8_t cmd);
    void sendCommand(uint8_t cmd, uint8_t param);
    void initializeCommands();
    void setPageMode();
    void setHorizontalMode();
};

} // namespace MinimalUI