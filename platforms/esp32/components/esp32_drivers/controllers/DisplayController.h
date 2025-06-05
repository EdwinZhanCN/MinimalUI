#pragma once

#include <cstdint>
#include <cstddef>  // This header defines size_t

namespace MinimalUI {

// 前向声明
class ESP32_SPI_Driver;

/**
 * @brief 显示控制器抽象接口
 * 不同的显示控制器芯片需要实现这个接口
 */
class DisplayController {
public:
    virtual ~DisplayController() = default;

    /**
     * @brief 初始化显示控制器
     * @param spi_driver SPI通信驱动
     * @return 初始化是否成功
     */
    virtual bool initialize(ESP32_SPI_Driver* spi_driver) = 0;

    /**
     * @brief 设置绘图窗口
     * @param x 起始X坐标
     * @param y 起始Y坐标
     * @param w 宽度
     * @param h 高度
     */
    virtual void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) = 0;

    /**
     * @brief 写入像素数据
     * @param data 像素数据缓冲区
     * @param length 数据长度
     */
    virtual void writePixelData(const uint8_t* data, size_t length) = 0;

    /**
     * @brief 清屏
     */
    virtual void clearScreen() = 0;

    /**
     * @brief 刷新显示（如果需要）
     */
    virtual void refresh() = 0;

    /**
     * @brief 获取显示器宽度
     */
    virtual int16_t getWidth() const = 0;

    /**
     * @brief 获取显示器高度
     */
    virtual int16_t getHeight() const = 0;

    /**
     * @brief 获取像素格式大小（字节数）
     */
    virtual uint8_t getPixelSize() const = 0;

protected:
    ESP32_SPI_Driver* spi_driver_ = nullptr;
};

} // namespace MinimalUI