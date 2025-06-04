#include "SSD1309Controller.h"
#include "../ESP32_SPI_Driver.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstring>

namespace MinimalUI {

static const char* TAG = "SSD1309Controller";

SSD1309Controller::SSD1309Controller(const SSD1309Config& config)
    : config_(config), frame_buffer_(nullptr), buffer_size_(0), dirty_(true) {
    
    // 计算缓冲区大小：宽度 * 高度 / 8 (每个字节存储8个像素)
    buffer_size_ = (config_.width * config_.height) / 8;
    frame_buffer_ = new uint8_t[buffer_size_];
    
    // 初始化缓冲区为全黑
    memset(frame_buffer_, 0x00, buffer_size_);
    
    ESP_LOGI(TAG, "SSD1309Controller created: %dx%d, buffer size: %zu bytes", 
             config_.width, config_.height, buffer_size_);
}

bool SSD1309Controller::initialize(ESP32_SPI_Driver* spi_driver) {
    spi_driver_ = spi_driver;
    
    if (!spi_driver_) {
        ESP_LOGE(TAG, "SPI driver is null");
        return false;
    }
    
    ESP_LOGI(TAG, "Initializing SSD1309 OLED controller");
    
    // 发送初始化命令序列
    initializeCommands();
    
    // 清屏
    clearScreen();
    
    // 开启显示
    sendCommand(SSD1309_DISPLAYON);
    
    ESP_LOGI(TAG, "SSD1309 initialization completed");
    return true;
}

void SSD1309Controller::initializeCommands() {
    // 关闭显示
    sendCommand(SSD1309_DISPLAYOFF);
    
    // 设置显示时钟分频
    sendCommand(SSD1309_SETDISPLAYCLOCKDIV, 0x80);
    
    // 设置多路复用比
    sendCommand(SSD1309_SETMULTIPLEX, config_.height - 1);
    
    // 设置显示偏移
    sendCommand(SSD1309_SETDISPLAYOFFSET, 0x00);
    
    // 设置起始行
    sendCommand(SSD1309_SETSTARTLINE | 0x0);
    
    // 设置电荷泵
    if (config_.external_vcc) {
        sendCommand(SSD1309_CHARGEPUMP, 0x10);
    } else {
        sendCommand(SSD1309_CHARGEPUMP, 0x14);
    }
    
    // 设置内存地址模式为水平模式
    sendCommand(SSD1309_MEMORYMODE, 0x00);
    
    // 设置段重映射
    if (config_.flip_horizontal) {
        sendCommand(SSD1309_SEGREMAP | 0x1);
    } else {
        sendCommand(SSD1309_SEGREMAP | 0x0);
    }
    
    // 设置COM扫描方向
    if (config_.flip_vertical) {
        sendCommand(SSD1309_COMSCANDEC);
    } else {
        sendCommand(SSD1309_COMSCANINC);
    }
    
    // 设置COM引脚配置
    uint8_t com_pins = 0x02;
    if (config_.height == 64) {
        com_pins = 0x12;
    } else if (config_.height == 32) {
        com_pins = 0x02;
    }
    sendCommand(SSD1309_SETCOMPINS, com_pins);
    
    // 设置对比度
    sendCommand(SSD1309_SETCONTRAST, 0xCF);
    
    // 设置预充电周期
    if (config_.external_vcc) {
        sendCommand(SSD1309_SETPRECHARGE, 0x22);
    } else {
        sendCommand(SSD1309_SETPRECHARGE, 0xF1);
    }
    
    // 设置VCOM检测电平
    sendCommand(SSD1309_SETVCOMDETECT, 0x40);
    
    // 恢复正常显示
    sendCommand(SSD1309_DISPLAYALLON_RESUME);
    
    // 设置正常显示模式
    sendCommand(SSD1309_NORMALDISPLAY);
}

void SSD1309Controller::setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) {
    // SSD1309使用页模式，设置列和页地址范围
    int16_t x2 = x + w - 1;
    int16_t y2 = y + h - 1;
    
    // 确保坐标在有效范围内
    if (x2 >= config_.width) x2 = config_.width - 1;
    if (y2 >= config_.height) y2 = config_.height - 1;
    
    // 设置列地址范围
    sendCommand(SSD1309_COLUMNADDR);
    sendCommand(x);
    sendCommand(x2);
    
    // 设置页地址范围 (每页8像素高)
    sendCommand(SSD1309_PAGEADDR);
    sendCommand(y / 8);
    sendCommand(y2 / 8);
}

void SSD1309Controller::writePixelData(const uint8_t* data, size_t length) {
    if (!spi_driver_ || !data || length == 0) {
        return;
    }
    
    // SSD1309直接发送数据
    spi_driver_->sendBuffer(data, length);
}

void SSD1309Controller::setPixel(int16_t x, int16_t y, bool color) {
    if (x < 0 || x >= config_.width || y < 0 || y >= config_.height) {
        return; // 越界检查
    }
    
    // 计算在帧缓冲区中的位置
    // SSD1309使用页模式：每页8像素高，按列存储
    int16_t page = y / 8;
    int16_t bit = y % 8;
    size_t index = page * config_.width + x;
    
    if (index >= buffer_size_) {
        return;
    }
    
    // 设置或清除对应的位
    if (color) {
        frame_buffer_[index] |= (1 << bit);
    } else {
        frame_buffer_[index] &= ~(1 << bit);
    }
    
    dirty_ = true;
}

void SSD1309Controller::clearScreen() {
    // 清空帧缓冲区
    memset(frame_buffer_, 0x00, buffer_size_);
    dirty_ = true;
    
    // 立即刷新到显示器
    refresh();
}

void SSD1309Controller::refresh() {
    if (!spi_driver_ || !dirty_) {
        return;
    }
    
    // 设置整个显示区域
    setAddrWindow(0, 0, config_.width, config_.height);
    
    // 发送整个帧缓冲区
    writePixelData(frame_buffer_, buffer_size_);
    
    dirty_ = false;
}

void SSD1309Controller::sendCommand(uint8_t cmd) {
    if (spi_driver_) {
        spi_driver_->sendCommand(cmd);
    }
}

void SSD1309Controller::sendCommand(uint8_t cmd, uint8_t param) {
    if (spi_driver_) {
        spi_driver_->sendCommand(cmd);
        spi_driver_->sendData(param);
    }
}

} // namespace MinimalUI