#pragma once

#include <cstdint>

namespace MinimalUI {

// 基本颜色定义
typedef uint16_t Color;

namespace Colors {
    constexpr Color BLACK       = 0x0000;
    constexpr Color WHITE       = 0xFFFF;
    constexpr Color RED         = 0xF800;
    constexpr Color GREEN       = 0x07E0;
    constexpr Color BLUE        = 0x001F;
    constexpr Color CYAN        = 0x07FF;
    constexpr Color MAGENTA     = 0xF81F;
    constexpr Color YELLOW      = 0xFFE0;
    constexpr Color GRAY        = 0x7BEF;
}

// 图形驱动抽象接口
class GraphicsDriver {
public:
    virtual ~GraphicsDriver() = default;
    
    // 初始化显示器
    virtual bool initialize() = 0;
    
    // 基本绘图操作
    virtual void drawPixel(int16_t x, int16_t y, Color color) = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) = 0;
    virtual void drawHLine(int16_t x, int16_t y, int16_t w, Color color) = 0;
    virtual void drawVLine(int16_t x, int16_t y, int16_t h, Color color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color) = 0;
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) = 0;
    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, Color color) = 0;
    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, Color color) = 0;
    
    // 字符绘制（可选实现）
    virtual void drawChar(int16_t x, int16_t y, char c, Color color, Color bg, uint8_t size = 1) {
        // 默认空实现
    }
    
    // 设备控制
    virtual void display() = 0;
    virtual void clear(Color color = Colors::BLACK) = 0;
    
    // 辅助函数
    virtual int16_t width() const { return 240; }  // 默认宽度
    virtual int16_t height() const { return 320; } // 默认高度

protected:
    // 辅助函数，用于填充圆角
    virtual void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, 
                                 int16_t delta, Color color) {}
};

// 驱动类型枚举
enum class DriverType {
    NONE,
    ESP32_SPI,
    STM32_SPI,
    JETSON_FB
};

} // namespace MinimalUI