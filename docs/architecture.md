---
title: Architecture Overview
description: Understanding MinimalUI's modular design and architecture
---

# Architecture Overview

MinimalUI is designed with a layered, modular architecture that provides clean separation between platform-specific code and application logic. This design enables easy porting to new platforms while maintaining a consistent API.

## Core Design Principles

### 🎯 Platform Abstraction
- **Hardware Independence**: UI logic is completely separated from hardware implementation
- **Unified API**: Same interface across all supported platforms
- **Plugin Architecture**: Easy to add new display controllers and platforms

### 🔧 Modular Components
- **Interchangeable Drivers**: Display controllers can be swapped without changing application code
- **Reusable Components**: UI elements work across all platforms
- **Minimal Dependencies**: Each module has clear, minimal dependencies

### ⚡ Performance Focused
- **Memory Efficient**: Optimized for embedded systems with limited RAM
- **Real-time Capable**: Deterministic performance for time-critical applications
- **Hardware Acceleration**: Leverages platform-specific optimizations

## Architecture Layers

```
┌─────────────────────────────────────────────────────────────┐
│                     Application Layer                       │
│  ┌─────────────────┐ ┌─────────────────┐ ┌───────────────┐  │
│  │   User App      │ │   UI Components │ │   Examples    │  │
│  │   main.cpp      │ │   Label, Button │ │   Demos       │  │
│  └─────────────────┘ └─────────────────┘ └───────────────┘  │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                    Framework Layer                          │
│  ┌─────────────────────────────────────────────────────────┐ │
│  │            GraphicsDriver Interface                     │ │
│  │  drawPixel() | fillRect() | drawLine() | display()     │ │
│  └─────────────────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                   Platform Layer                            │
│  ┌─────────────────┐ ┌─────────────────┐ ┌───────────────┐  │
│  │   ESP32_SPI     │ │   STM32_SPI     │ │  Jetson_FB    │  │
│  │   Driver        │ │   Driver        │ │  Driver       │  │
│  └─────────────────┘ └─────────────────┘ └───────────────┘  │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                 Display Controller Layer                    │
│  ┌─────────────────┐ ┌─────────────────┐ ┌───────────────┐  │
│  │  SSD1309        │ │  SSD1306        │ │   ILI9341     │  │
│  │  Controller     │ │  Controller     │ │   Controller  │  │
│  └─────────────────┘ └─────────────────┘ └───────────────┘  │
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                    Hardware Layer                           │
│  ┌─────────────────┐ ┌─────────────────┐ ┌───────────────┐  │
│  │      SPI        │ │      I2C        │ │  Framebuffer  │  │
│  │   Interface     │ │   Interface     │ │   Interface   │  │
│  └─────────────────┘ └─────────────────┘ └───────────────┘  │
└─────────────────────────────────────────────────────────────┘
```

## Framework Components

### GraphicsDriver Interface

The core abstraction that defines the graphics API:

```cpp
class GraphicsDriver {
public:
    // Initialization
    virtual bool initialize() = 0;
    
    // Basic drawing operations
    virtual void drawPixel(int16_t x, int16_t y, Color color) = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, Color color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, Color color) = 0;
    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, Color color) = 0;
    
    // Display control
    virtual void display() = 0;
    virtual void clear(Color color = Colors::BLACK) = 0;
    
    // Properties
    virtual int16_t width() const = 0;
    virtual int16_t height() const = 0;
};
```

### Display Controller Interface

Abstracts specific display controller chips:

```cpp
class DisplayController {
public:
    virtual bool initialize(ESP32_SPI_Driver* spi_driver) = 0;
    virtual void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) = 0;
    virtual void writePixelData(const uint8_t* data, size_t length) = 0;
    virtual void clearScreen() = 0;
    virtual void refresh() = 0;
    
    virtual int16_t getWidth() const = 0;
    virtual int16_t getHeight() const = 0;
    virtual uint8_t getPixelSize() const = 0;
};
```

## Platform Implementation

### ESP32 Platform

**File Structure:**
```
platforms/esp32/
├── CMakeLists.txt              # ESP-IDF project configuration
├── main/
│   ├── CMakeLists.txt          # Main component
│   └── main.cpp                # Application entry point
├── components/
│   └── esp32_drivers/
│       ├── ESP32_SPI_Driver.h  # ESP32 SPI implementation
│       ├── ESP32_SPI_Driver.cpp
│       └── controllers/        # Display controllers
│           ├── DisplayController.h
│           ├── SSD1309Controller.h
│           └── SSD1309Controller.cpp
├── sdkconfig.defaults          # ESP-IDF configuration
└── README.md
```

**Key Features:**
- **ESP-IDF Integration**: Native ESP-IDF component system
- **FreeRTOS Support**: Multi-task environment
- **Hardware SPI**: ESP32 hardware SPI acceleration
- **Multiple SPI Hosts**: Support for SPI2_HOST and SPI3_HOST
- **GPIO Management**: Flexible pin configuration

### STM32 Platform (In Development)

**Planned Structure:**
```
platforms/stm32/
├── CMakeLists.txt
├── Core/
│   ├── Src/
│   │   └── main.cpp
│   └── Inc/
├── Drivers/
│   ├── STM32_SPI_Driver.h
│   └── STM32_SPI_Driver.cpp
└── Controllers/
```

### Jetson Platform (Planned)

**Planned Structure:**
```
platforms/jetson/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── drivers/
│   ├── Jetson_FB_Driver.h
│   └── Jetson_FB_Driver.cpp
└── examples/
```

## Component Architecture

### Display Controllers

Each display controller implements the `DisplayController` interface:

**SSD1309 Controller:**
- **Monochrome OLED**: 1-bit per pixel
- **Page Mode**: 8-pixel high pages
- **Frame Buffer**: Complete frame buffer in RAM
- **Batch Updates**: Efficient full-screen updates

**SSD1306 Controller (Planned):**
- **Similar to SSD1309**: Minor command differences
- **I2C Support**: Additional I2C interface option

**ILI9341 Controller (Planned):**
- **Color TFT**: 16-bit RGB565 format
- **Direct Mode**: Pixel-by-pixel updates
- **Window Mode**: Efficient region updates

### Memory Management

**Frame Buffer Strategy:**
```cpp
// SSD1309 Example - 1 bit per pixel
class SSD1309Controller {
private:
    uint8_t* frame_buffer_;    // 128*64/8 = 1024 bytes
    size_t buffer_size_;
    bool dirty_;               // Optimization flag
};
```

**Color Format Handling:**
```cpp
void convertColor(Color color, uint8_t* buffer, uint8_t pixel_size) {
    switch (pixel_size) {
        case 1: // Monochrome (SSD1309)
            buffer[0] = (color != 0) ? 0xFF : 0x00;
            break;
        case 2: // RGB565 (ILI9341)
            buffer[0] = color >> 8;
            buffer[1] = color & 0xFF;
            break;
        case 3: // RGB888
            buffer[0] = (color >> 11) << 3; // R
            buffer[1] = ((color >> 5) & 0x3F) << 2; // G
            buffer[2] = (color & 0x1F) << 3; // B
            break;
    }
}
```

## Communication Protocols

### SPI Implementation

**ESP32 SPI Configuration:**
```cpp
struct ESP32_SPI_Config {
    spi_host_device_t spi_host = SPI2_HOST;
    int8_t dc_pin;      // Data/Command control
    int8_t cs_pin;      // Chip select
    int8_t rst_pin;     // Reset
    int8_t mosi_pin;    // Master Out Slave In
    int8_t sclk_pin;    // Serial Clock
    uint32_t freq;      // SPI frequency
    uint8_t spi_mode;   // SPI mode (0-3)
};
```

**Transaction Handling:**
```cpp
void ESP32_SPI_Driver::sendCommand(uint8_t cmd) {
    gpio_set_level((gpio_num_t)config_.dc_pin, 0);  // Command mode
    gpio_set_level((gpio_num_t)config_.cs_pin, 0);  // Select device
    
    spi_transaction_t t = createTransaction(&cmd, 8);
    esp_err_t ret = spi_device_transmit(spi_, &t);
    
    gpio_set_level((gpio_num_t)config_.cs_pin, 1);  // Deselect
}
```

## Build System Integration

### CMake Structure

**Root CMakeLists.txt:**
```cmake
cmake_minimum_required(VERSION 3.16)
project(MinimalUI VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection
if(NOT DEFINED TARGET_PLATFORM)
    set(TARGET_PLATFORM "esp32")
endif()

# Add framework and platform-specific code
add_subdirectory(framework)
if(TARGET_PLATFORM STREQUAL "esp32")
    # ESP32 uses ESP-IDF build system
    message(STATUS "Use: cd platforms/esp32 && idf.py build")
endif()
```

**ESP-IDF Component Registration:**
```cmake
idf_component_register(
    SRCS "ESP32_SPI_Driver.cpp"
         "controllers/SSD1309Controller.cpp"
    INCLUDE_DIRS "." "controllers"
    REQUIRES driver spi_flash esp_system freertos framework
    PRIV_REQUIRES esp_common
)
```

## Error Handling Strategy

### Graceful Degradation
- **Initialization Failures**: Return false, allow retry
- **Communication Errors**: Log and continue
- **Resource Exhaustion**: Clean shutdown

### Debug Support
```cpp
static const char* TAG = "ESP32_SPI_Driver";

// Comprehensive logging
ESP_LOGI(TAG, "Initializing SPI driver");
ESP_LOGD(TAG, "SPI frequency: %lu Hz", config_.freq);
ESP_LOGE(TAG, "Failed to initialize: %s", esp_err_to_name(ret));
```

## Performance Considerations

### Memory Optimization
- **Stack Usage**: Careful stack allocation in embedded systems
- **Heap Fragmentation**: Use fixed-size allocations where possible
- **Frame Buffer**: Optimize for target platform memory constraints

### Real-time Performance
- **DMA Usage**: Hardware acceleration where available
- **Interrupt Handling**: Non-blocking operations
- **Task Priority**: Appropriate FreeRTOS task priorities

## Extensibility

### Adding New Platforms

1. **Create Platform Directory**: `platforms/new_platform/`
2. **Implement GraphicsDriver**: Platform-specific driver class
3. **Add Build Support**: CMake integration
4. **Create Examples**: Platform-specific demos

### Adding New Controllers

1. **Implement DisplayController**: Controller-specific class
2. **Add Initialization**: Platform integration
3. **Test and Validate**: Hardware verification
4. **Document**: API and usage examples

This architecture provides a solid foundation for cross-platform embedded UI development while maintaining simplicity and performance.