#include "ESP32_SPI_Driver.h"
#include "controllers/DisplayController.h"
#include <cstring>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

namespace MinimalUI {

static const char* TAG = "ESP32_SPI_Driver";

ESP32_SPI_Driver::ESP32_SPI_Driver(const ESP32_SPI_Config& config, std::unique_ptr<DisplayController> controller)
    : config_(config), spi_(nullptr), controller_(std::move(controller)) {
    ESP_LOGI(TAG, "ESP32_SPI_Driver created with controller");
}

ESP32_SPI_Driver::~ESP32_SPI_Driver() {
    freeSPI();
    ESP_LOGI(TAG, "ESP32_SPI_Driver destroyed");
}

bool ESP32_SPI_Driver::initialize() {
    ESP_LOGI(TAG, "Initializing ESP32 SPI Driver");
    
    if (!controller_) {
        ESP_LOGE(TAG, "No display controller provided");
        return false;
    }
    
    // 初始化SPI硬件
    if (!initSPI()) {
        return false;
    }
    
    // 初始化显示控制器
    return controller_->initialize(this);
}

bool ESP32_SPI_Driver::initSPI() {
    // 配置GPIO引脚
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << config_.cs_pin) | (1ULL << config_.dc_pin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    
    if (config_.rst_pin >= 0) {
        io_conf.pin_bit_mask = (1ULL << config_.rst_pin);
        gpio_config(&io_conf);
    }
    
    // 配置SPI总线
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = config_.mosi_pin,
        .miso_io_num = config_.miso_pin,
        .sclk_io_num = config_.sclk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4092,  // 通用大小
        .flags = 0,
        .intr_flags = 0
    };
    
    esp_err_t ret = spi_bus_initialize(config_.spi_host, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        return false;
    }
    
    // 配置SPI设备
    spi_device_interface_config_t dev_cfg = {
        .mode = config_.spi_mode,
        .clock_speed_hz = static_cast<int>(config_.freq),
        .spics_io_num = -1, // 我们手动控制CS引脚
        .flags = 0,
        .queue_size = 7,
        .pre_cb = nullptr,
        .post_cb = nullptr
    };
    
    ret = spi_bus_add_device(config_.spi_host, &dev_cfg, &spi_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add SPI device: %s", esp_err_to_name(ret));
        spi_bus_free(config_.spi_host);
        return false;
    }
    
    // 设置GPIO初始状态
    gpio_set_level((gpio_num_t)config_.cs_pin, 1);
    gpio_set_level((gpio_num_t)config_.dc_pin, 1);
    
    if (config_.rst_pin >= 0) {
        // 硬件复位
        gpio_set_level((gpio_num_t)config_.rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level((gpio_num_t)config_.rst_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level((gpio_num_t)config_.rst_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(120));
    }
    
    ESP_LOGI(TAG, "SPI hardware initialized successfully");
    return true;
}

void ESP32_SPI_Driver::freeSPI() {
    if (spi_) {
        spi_bus_remove_device(spi_);
        spi_bus_free(config_.spi_host);
        spi_ = nullptr;
        ESP_LOGI(TAG, "SPI resources released");
    }
}

void ESP32_SPI_Driver::sendCommand(uint8_t cmd) {
    gpio_set_level((gpio_num_t)config_.dc_pin, 0);  // DC低电平表示命令
    gpio_set_level((gpio_num_t)config_.cs_pin, 0);  // 选中芯片
    
    spi_transaction_t t = createTransaction(&cmd, 8);
    esp_err_t ret = spi_device_transmit(spi_, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI command transmit failed: %s", esp_err_to_name(ret));
    }
    
    gpio_set_level((gpio_num_t)config_.cs_pin, 1);  // 取消选中
}

void ESP32_SPI_Driver::sendData(uint8_t data) {
    gpio_set_level((gpio_num_t)config_.dc_pin, 1);  // DC高电平表示数据
    gpio_set_level((gpio_num_t)config_.cs_pin, 0);  // 选中芯片
    
    spi_transaction_t t = createTransaction(&data, 8);
    esp_err_t ret = spi_device_transmit(spi_, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPI data transmit failed: %s", esp_err_to_name(ret));
    }
    
    gpio_set_level((gpio_num_t)config_.cs_pin, 1);  // 取消选中
}

void ESP32_SPI_Driver::sendBuffer(const uint8_t* buffer, size_t size) {
    gpio_set_level((gpio_num_t)config_.dc_pin, 1);  // DC高电平表示数据
    gpio_set_level((gpio_num_t)config_.cs_pin, 0);  // 选中芯片
    
    // 对于大数据，可能需要分块发送
    const size_t max_transfer_size = 4092; // ESP32 SPI最大传输大小
    size_t remaining = size;
    const uint8_t* ptr = buffer;
    
    while (remaining > 0) {
        size_t chunk_size = (remaining > max_transfer_size) ? max_transfer_size : remaining;
        
        spi_transaction_t t = createTransaction(ptr, chunk_size * 8);
        esp_err_t ret = spi_device_transmit(spi_, &t);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "SPI buffer transmit failed: %s", esp_err_to_name(ret));
            break;
        }
        
        ptr += chunk_size;
        remaining -= chunk_size;
    }
    
    gpio_set_level((gpio_num_t)config_.cs_pin, 1);  // 取消选中
}

spi_transaction_t ESP32_SPI_Driver::createTransaction(const void* data, size_t length) {
    spi_transaction_t t = {};
    t.length = length;
    t.tx_buffer = data;
    t.rx_buffer = nullptr;
    t.flags = 0;
    return t;
}

// 根据不同的显示控制器实现，获取屏幕宽度和高度
int16_t ESP32_SPI_Driver::width() const {
    return controller_ ? controller_->getWidth() : 0;
}

int16_t ESP32_SPI_Driver::height() const {
    return controller_ ? controller_->getHeight() : 0;
}

void ESP32_SPI_Driver::drawPixel(int16_t x, int16_t y, Color color) {
    if (!controller_) return;
    
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
        return;  // 越界检查
    }
    
    controller_->setAddrWindow(x, y, 1, 1);
    
    uint8_t pixel_data[4]; // 支持不同像素格式
    uint8_t pixel_size = controller_->getPixelSize();
    convertColor(color, pixel_data, pixel_size);
    controller_->writePixelData(pixel_data, pixel_size);
}

void ESP32_SPI_Driver::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) {
    if (!controller_) return;
    
    int16_t screen_width = width();
    int16_t screen_height = height();
    
    if (x >= screen_width || y >= screen_height || w <= 0 || h <= 0) {
        return;  // 参数检查
    }
    
    // 调整绘图区域，确保不超出屏幕
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > screen_width) {
        w = screen_width - x;
    }
    if (y + h > screen_height) {
        h = screen_height - y;
    }
    
    controller_->setAddrWindow(x, y, w, h);
    
    // 计算像素总数和每像素字节数
    uint32_t pixelCount = w * h;
    uint8_t pixel_size = controller_->getPixelSize();
    
    // 创建颜色缓冲区
    const size_t max_buffer_size = 1024;
    size_t pixels_per_chunk = max_buffer_size / pixel_size;
    uint8_t* color_buffer = new uint8_t[pixels_per_chunk * pixel_size];
    
    // 预填充一个像素的颜色数据
    uint8_t pixel_data[4];
    convertColor(color, pixel_data, pixel_size);
    
    // 填充颜色缓冲区
    for (size_t i = 0; i < pixels_per_chunk; i++) {
        memcpy(color_buffer + i * pixel_size, pixel_data, pixel_size);
    }
    
    // 分块发送数据
    uint32_t remaining_pixels = pixelCount;
    while (remaining_pixels > 0) {
        uint32_t chunk_pixels = std::min(remaining_pixels, (uint32_t)pixels_per_chunk);
        controller_->writePixelData(color_buffer, chunk_pixels * pixel_size);
        remaining_pixels -= chunk_pixels;
    }
    
    delete[] color_buffer;
}

void ESP32_SPI_Driver::drawHLine(int16_t x, int16_t y, int16_t w, Color color) {
    int16_t screen_width = width();
    int16_t screen_height = height();
    
    if (y < 0 || y >= screen_height || x >= screen_width || w <= 0) {
        return;
    }
    
    if (x < 0) {
        w += x;
        x = 0;
    }
    if (x + w > screen_width) {
        w = screen_width - x;
    }
    
    fillRect(x, y, w, 1, color);
}

void ESP32_SPI_Driver::drawVLine(int16_t x, int16_t y, int16_t h, Color color) {
    int16_t screen_width = width();
    int16_t screen_height = height();
    
    if (x < 0 || x >= screen_width || y >= screen_height || h <= 0) {
        return;
    }
    
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (y + h > screen_height) {
        h = screen_height - y;
    }
    
    fillRect(x, y, 1, h, color);
}

void ESP32_SPI_Driver::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color) {
    // 处理水平线和垂直线的特殊情况
    if (x0 == x1) {
        drawVLine(x0, std::min(y0, y1), std::abs(y1 - y0) + 1, color);
        return;
    }
    if (y0 == y1) {
        drawHLine(std::min(x0, x1), y0, std::abs(x1 - x0) + 1, color);
        return;
    }
    
    // Bresenham算法绘制一般直线
    int16_t steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    
    int16_t dx = x1 - x0;
    int16_t dy = std::abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;
    int16_t y = y0;
    
    for (int16_t x = x0; x <= x1; x++) {
        if (steep) {
            drawPixel(y, x, color);
        } else {
            drawPixel(x, y, color);
        }
        
        err -= dy;
        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
}

void ESP32_SPI_Driver::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) {
    // 绘制四条边
    drawHLine(x, y, w, color);          // 顶边
    drawHLine(x, y + h - 1, w, color);  // 底边
    drawVLine(x, y, h, color);          // 左边
    drawVLine(x + w - 1, y, h, color);  // 右边
}

void ESP32_SPI_Driver::drawCircle(int16_t x0, int16_t y0, int16_t r, Color color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    
    drawPixel(x0, y0 + r, color);
    drawPixel(x0, y0 - r, color);
    drawPixel(x0 + r, y0, color);
    drawPixel(x0 - r, y0, color);
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void ESP32_SPI_Driver::fillCircle(int16_t x0, int16_t y0, int16_t r, Color color) {
    // 先画垂直线贯穿圆心
    drawVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

void ESP32_SPI_Driver::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, 
                                       int16_t delta, Color color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;
    int16_t px = x;
    int16_t py = y;
    
    delta++; // 偏移量加1
    
    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        
        x++;
        ddF_x += 2;
        f += ddF_x;
        
        if (x < (y + 1)) {
            if (corners & 1) drawVLine(x0 + x, y0 - y, 2 * y + delta, color);
            if (corners & 2) drawVLine(x0 - x, y0 - y, 2 * y + delta, color);
        }
        
        if (y != py) {
            if (corners & 1) drawVLine(x0 + py, y0 - px, 2 * px + delta, color);
            if (corners & 2) drawVLine(x0 - py, y0 - px, 2 * px + delta, color);
            py = y;
        }
        px = x;
    }
}

void ESP32_SPI_Driver::drawChar(int16_t x, int16_t y, char c, Color color, Color bg, uint8_t size) {
    // 这里可以实现字符绘制逻辑
    // 通常需要字体资源，为简化这里留空
    ESP_LOGD(TAG, "Drawing char '%c' at (%d,%d)", c, x, y);
}

void ESP32_SPI_Driver::display() {
    if (controller_) {
        controller_->refresh();
    }
    ESP_LOGD(TAG, "Display refreshed");
}

void ESP32_SPI_Driver::clear(Color color) {
    if (controller_) {
        if (color == 0x0000) { // 如果是黑色，直接使用控制器的清屏功能
            controller_->clearScreen();
        } else {
            fillRect(0, 0, width(), height(), color);
        }
    }
}

void ESP32_SPI_Driver::convertColor(Color color, uint8_t* buffer, uint8_t pixel_size) {
    switch (pixel_size) {
        case 1: // 单色 (SSD1309等)
            buffer[0] = (color != 0) ? 0xFF : 0x00;
            break;
        case 2: // RGB565 (ILI9341等)
            buffer[0] = color >> 8;
            buffer[1] = color & 0xFF;
            break;
        case 3: // RGB888
            buffer[0] = (color >> 11) << 3; // R
            buffer[1] = ((color >> 5) & 0x3F) << 2; // G
            buffer[2] = (color & 0x1F) << 3; // B
            break;
        default:
            buffer[0] = color & 0xFF;
            break;
    }
}

} // namespace MinimalUI