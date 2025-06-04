// framework/include/GraphicsDriver.h
#pragma once

#include <cstdint>

class GraphicsDriver {
public:
    virtual ~GraphicsDriver() = default;

    // 初始化显示
    virtual bool init() = 0;

    // 基础绘图功能
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
    virtual void fillScreen(uint16_t color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;

    // 高级功能
    virtual void drawBuffer(uint16_t* buffer, uint32_t size) = 0;
    virtual void setRotation(uint8_t rotation) = 0;

    // 显示控制
    virtual void displayOn() = 0;
    virtual void displayOff() = 0;

    // 获取显示参数
    virtual uint16_t getWidth() const = 0;
    virtual uint16_t getHeight() const = 0;
};
