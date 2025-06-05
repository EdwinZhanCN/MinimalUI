---
title: ESP32 Platform Guide
description: Complete guide for developing with MinimalUI on ESP32 platforms
---

# ESP32 Platform Guide

ESP32 is the primary and most mature platform for MinimalUI, offering robust SPI display support with the ESP-IDF framework. This guide covers everything you need to know for ESP32 development.

## Supported ESP32 Variants

### ✅ Tested and Supported

| Chip | CPU Cores | WiFi | Bluetooth | Max Freq | RAM | Flash |
|------|-----------|------|-----------|----------|-----|-------|
| **ESP32** | Dual Xtensa | 802.11 b/g/n | Classic + BLE | 240MHz | 520KB | 4MB+ |
| **ESP32-S3** | Dual Xtensa | 802.11 b/g/n | BLE 5.0 | 240MHz | 512KB | 8MB+ |
| **ESP32-C6** | Single RISC-V | 802.11 ax | BLE 5.0 + 802.15.4 | 160MHz | 512KB | 4MB+ |

### 🔄 Compatible (Not Fully Tested)

| Chip | CPU Cores | WiFi | Bluetooth | Max Freq | RAM | Flash |
|------|-----------|------|-----------|----------|-----|-------|
| **ESP32-S2** | Single Xtensa | 802.11 b/g/n | None | 240MHz | 320KB | 4MB+ |
| **ESP32-C3** | Single RISC-V | 802.11 b/g/n | BLE 5.0 | 160MHz | 400KB | 4MB+ |

## Development Environment Setup

### Prerequisites

- **ESP-IDF v5.4.1** or later (recommended v5.4.1)
- **Python 3.8+** with pip
- **Git**
- **CMake 3.16+**
- **Ninja build system**

### ESP-IDF Installation

#### Ubuntu/Debian
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

# Create ESP directory
mkdir -p ~/esp
cd ~/esp

# Clone ESP-IDF
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf

# Install ESP-IDF
cd esp-idf
./install.sh

# Set up environment
echo 'export IDF_PATH="$HOME/esp/esp-idf"' >> ~/.bashrc
echo 'alias get_idf=". $IDF_PATH/export.sh"' >> ~/.bashrc
source ~/.bashrc
```

#### macOS
```bash
# Install dependencies
brew install cmake ninja dfu-util

# Create ESP directory
mkdir -p ~/esp
cd ~/esp

# Clone ESP-IDF
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf

# Install ESP-IDF
cd esp-idf
./install.sh

# Set up environment
echo 'export IDF_PATH="$HOME/esp/esp-idf"' >> ~/.zshrc
echo 'alias get_idf=". $IDF_PATH/export.sh"' >> ~/.zshrc
source ~/.zshrc
```

#### Windows
1. Download ESP-IDF installer from Espressif website
2. Run the installer and follow the setup wizard
3. Use ESP-IDF Command Prompt for development

### Verification
```bash
# Activate ESP-IDF environment
get_idf

# Check installation
idf.py --version
```

## Hardware Setup

### Recommended Development Boards

#### ESP32-C6 (Seeed Studio XIAO ESP32-C6)
- **Advantages**: Latest chip, WiFi 6, small form factor
- **Considerations**: Single core, newer ecosystem
- **Perfect for**: Compact projects, latest features

#### ESP32-S3 DevKitC
- **Advantages**: Dual core, large RAM, mature ecosystem  
- **Considerations**: Higher power consumption
- **Perfect for**: Complex applications, multi-tasking

#### ESP32 DevKit V1
- **Advantages**: Proven design, extensive community support
- **Considerations**: Older architecture
- **Perfect for**: Learning, prototyping

### Display Connections

#### SSD1309 OLED (128x64) - Recommended
```
ESP32 Pin    OLED Module    Function
---------    -----------    --------
3.3V         VCC            Power supply
GND          GND            Ground
GPIO18       DIN/SDA        Data (MOSI)
GPIO19       CLK/SCK        Serial Clock
GPIO2        CS             Chip Select
GPIO1        DC             Data/Command
GPIO0        RST            Reset
```

#### Connection Quality Tips
- **Use short wires** (< 10cm) for stable signals
- **Add 100nF capacitor** between VCC and GND near OLED
- **Consider PCB/protoboard** for permanent projects
- **Check connections twice** - loose wires cause 90% of issues

### Alternative Pin Configurations

#### For ESP32-C6
```cpp
// Recommended pins for ESP32-C6
ESP32_SPI_Config config;
config.dc_pin = 1;      // GPIO1 - Data/Command
config.cs_pin = 2;      // GPIO2 - Chip Select  
config.rst_pin = 0;     // GPIO0 - Reset
config.mosi_pin = 18;   // GPIO18 - MOSI
config.sclk_pin = 19;   // GPIO19 - SCLK
```

#### For ESP32-S3
```cpp
// Recommended pins for ESP32-S3
ESP32_SPI_Config config;
config.dc_pin = 4;      // GPIO4 - Data/Command
config.cs_pin = 5;      // GPIO5 - Chip Select
config.rst_pin = 6;     // GPIO6 - Reset  
config.mosi_pin = 11;   // GPIO11 - MOSI (HSPI)
config.sclk_pin = 12;   // GPIO12 - SCLK (HSPI)
```

## Project Structure

### Minimal Project Layout
```
my_esp32_project/
├── CMakeLists.txt              # Root CMake file
├── main/
│   ├── CMakeLists.txt          # Main component
│   └── main.cpp                # Application code
├── components/                 # Additional components
│   └── minimalui/             # MinimalUI as component
├── sdkconfig.defaults         # Default configuration
└── README.md
```

### Integration Methods

#### Method 1: As ESP-IDF Component (Recommended)
```cmake
# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS 
    "path/to/MinimalUI/framework"
    "path/to/MinimalUI/platforms/esp32/components"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_esp32_project)
```

#### Method 2: Git Submodule
```bash
# Add as submodule
git submodule add https://github.com/MinimalUI/MinimalUI.git components/minimalui

# Update CMakeLists.txt
set(EXTRA_COMPONENT_DIRS "components/minimalui/framework"
                         "components/minimalui/platforms/esp32/components")
```

### Main Component Setup
```cmake
# main/CMakeLists.txt
idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES driver esp_system freertos esp32_drivers
)
```

## Configuration Options

### SPI Configuration Parameters

```cpp
struct ESP32_SPI_Config {
    spi_host_device_t spi_host;    // SPI2_HOST or SPI3_HOST
    int8_t dc_pin;                 // Data/Command control pin
    int8_t cs_pin;                 // Chip Select pin
    int8_t rst_pin;                // Reset pin
    int8_t mosi_pin;               // Master Out Slave In
    int8_t sclk_pin;               // Serial Clock
    int8_t miso_pin;               // Master In Slave Out (usually -1)
    uint32_t freq;                 // SPI frequency in Hz
    uint8_t spi_mode;              // SPI mode (0-3)
};
```

### Frequency Guidelines

| Display Type | Recommended Frequency | Max Frequency | Notes |
|--------------|----------------------|---------------|-------|
| **SSD1309 OLED** | 4-8 MHz | 20 MHz | Start with 4MHz |
| **SSD1306 OLED** | 4-8 MHz | 20 MHz | Very stable |
| **ILI9341 TFT** | 20-40 MHz | 80 MHz | Faster for color |
| **ST7735 TFT** | 15-30 MHz | 50 MHz | Good balance |

### SPI Mode Selection

| Mode | CPOL | CPHA | Usage |
|------|------|------|-------|
| **0** | 0 | 0 | Most OLED displays |
| **1** | 0 | 1 | Some TFT displays |
| **2** | 1 | 0 | Rare |
| **3** | 1 | 1 | Some special cases |

## ESP-IDF Configuration (sdkconfig)

### Essential Settings
```ini
# ESP32 configuration defaults for MinimalUI

# Enable C++ support
CONFIG_COMPILER_CXX_EXCEPTIONS=y
CONFIG_COMPILER_CXX_RTTI=y

# FreeRTOS configuration
CONFIG_FREERTOS_HZ=1000
CONFIG_FREERTOS_TIMER_TASK_STACK_DEPTH=2048

# SPI configuration
CONFIG_SPI_MASTER_IN_IRAM=y
CONFIG_SPI_MASTER_ISR_IN_IRAM=y

# Memory configuration
CONFIG_ESP32C6_DEFAULT_CPU_FREQ_160=y  # For ESP32-C6
CONFIG_ESP32_DEFAULT_CPU_FREQ_240=y    # For ESP32/S3

# Logging configuration
CONFIG_LOG_DEFAULT_LEVEL_INFO=y
CONFIG_LOG_DEFAULT_LEVEL=3

# Main task configuration
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192

# Disable power management for stability
CONFIG_PM_ENABLE=n
CONFIG_FREERTOS_USE_TICKLESS_IDLE=n

# USB Serial JTAG (for ESP32-C6/S3)
CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG=y
```

### Performance Optimization
```ini
# Compiler optimization
CONFIG_COMPILER_OPTIMIZATION_PERF=y

# Enable hardware acceleration
CONFIG_ESP32_SPIRAM_SUPPORT=n  # Usually not needed
CONFIG_SPIRAM_USE_MALLOC=n

# Flash settings
CONFIG_ESPTOOLPY_FLASHMODE_QIO=y
CONFIG_ESPTOOLPY_FLASHFREQ_80M=y
```

## Code Examples

### Basic Initialization
```cpp
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ESP32_SPI_Driver.h"
#include "controllers/SSD1309Controller.h"

using namespace MinimalUI;

static const char* TAG = "MinimalUI_Demo";

extern "C" void app_main(void) {
    ESP_LOGI(TAG, "MinimalUI ESP32 Demo Starting");
    
    // Configure SPI
    ESP32_SPI_Config spi_config;
    spi_config.spi_host = SPI2_HOST;
    spi_config.dc_pin = 1;
    spi_config.cs_pin = 2;
    spi_config.rst_pin = 0;
    spi_config.mosi_pin = 18;
    spi_config.sclk_pin = 19;
    spi_config.miso_pin = -1;
    spi_config.freq = 4000000;  // 4MHz
    spi_config.spi_mode = 0;

    // Configure display
    SSD1309Config oled_config;
    oled_config.width = 128;
    oled_config.height = 64;
    oled_config.external_vcc = false;

    // Create driver
    auto controller = std::make_unique<SSD1309Controller>(oled_config);
    ESP32_SPI_Driver driver(spi_config, std::move(controller));

    // Initialize
    if (!driver.initialize()) {
        ESP_LOGE(TAG, "Failed to initialize display");
        return;
    }

    ESP_LOGI(TAG, "Display initialized successfully");

    // Draw test pattern
    driver.clear(0);
    driver.fillRect(10, 10, 50, 30, 1);
    driver.drawCircle(80, 32, 15, 1);
    driver.display();

    ESP_LOGI(TAG, "Test pattern displayed");

    // Main loop
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### Advanced Multi-Task Example
```cpp
#include "MinimalUI_ESP32.h"

// Global driver instance
ESP32_SPI_Driver* g_driver = nullptr;

// Display update task
void display_task(void* parameter) {
    int counter = 0;
    
    while (1) {
        g_driver->clear(0);
        
        // Draw dynamic content
        g_driver->fillRect(0, 0, 128, 16, 1);  // Header bar
        
        // Counter display
        char text[32];
        snprintf(text, sizeof(text), "Count: %d", counter++);
        // Note: Text rendering would need font implementation
        
        g_driver->drawRect(10, 20, 108, 30, 1); // Frame
        g_driver->fillCircle(64, 35, counter % 10, 1); // Animated circle
        
        g_driver->display();
        
        vTaskDelay(pdMS_TO_TICKS(100)); // 10 FPS
    }
}

// Button input task (example)
void input_task(void* parameter) {
    while (1) {
        // Read button states
        // Update display based on input
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

extern "C" void app_main(void) {
    // Initialize driver (same as basic example)
    // ... initialization code ...
    
    g_driver = &driver;
    
    // Create tasks
    xTaskCreate(display_task, "Display", 4096, NULL, 5, NULL);
    xTaskCreate(input_task, "Input", 2048, NULL, 3, NULL);
    
    // Main task can handle other work
    while (1) {
        // WiFi, sensor reading, etc.
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

## Build and Flash Process

### Standard Build Process
```bash
# Navigate to ESP32 project
cd platforms/esp32

# Set up environment
get_idf

# Configure project (optional)
idf.py menuconfig

# Build project
idf.py build

# Flash and monitor
idf.py -p /dev/ttyUSB0 flash monitor
```

### Development Workflow
```bash
# Quick rebuild and flash
idf.py build flash

# Monitor only (after flash)
idf.py monitor

# Clean build
idf.py fullclean build

# Erase flash completely
idf.py -p /dev/ttyUSB0 erase-flash
```

### Port Detection
```bash
# Linux
ls /dev/tty*

# macOS  
ls /dev/cu.*

# Windows
# Check Device Manager or use:
idf.py -p COM3 flash
```

## Troubleshooting

### Common Issues and Solutions

#### 1. Black Screen / No Display
**Symptoms:** Code runs but display remains black

**Solutions:**
- Check all wiring connections
- Verify 3.3V power supply stability
- Try lower SPI frequency (1MHz)
- Add longer reset delays
- Test with multimeter for continuity

```cpp
// Enhanced reset sequence
gpio_set_level((gpio_num_t)config_.rst_pin, 0);
vTaskDelay(pdMS_TO_TICKS(100));  // Longer reset
gpio_set_level((gpio_num_t)config_.rst_pin, 1);
vTaskDelay(pdMS_TO_TICKS(200));  // Longer stabilization
```

#### 2. Intermittent Display Issues
**Symptoms:** Sometimes works, sometimes doesn't

**Solutions:**
- Improve connection quality (solder vs. breadboard)
- Add decoupling capacitors (100nF near OLED)
- Use shorter, higher quality wires
- Check for loose connections

#### 3. SPI Communication Errors
**Symptoms:** `esp_err_t` errors in logs

**Solutions:**
```cpp
// Add error checking
esp_err_t ret = spi_device_transmit(spi_, &t);
if (ret != ESP_OK) {
    ESP_LOGE(TAG, "SPI transmit failed: %s", esp_err_to_name(ret));
    // Handle error appropriately
}
```

#### 4. Memory Issues
**Symptoms:** Stack overflow, heap allocation failures

**Solutions:**
- Increase main task stack size
- Monitor heap usage
- Use static allocation where possible

```cpp
// In sdkconfig
CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192

// Monitor memory
ESP_LOGI(TAG, "Free heap: %lu", esp_get_free_heap_size());
```

#### 5. Build Errors
**Common Issues:**
- ESP-IDF version mismatch
- Missing dependencies
- Component path issues

```bash
# Verify ESP-IDF version
idf.py --version

# Clean and rebuild
idf.py fullclean
idf.py build
```

### Debug Techniques

#### Enable Verbose Logging
```cpp
// In main.cpp
esp_log_level_set("ESP32_SPI_Driver", ESP_LOG_DEBUG);
esp_log_level_set("SSD1309Controller", ESP_LOG_DEBUG);
esp_log_level_set("*", ESP_LOG_DEBUG);  // All components
```

#### Hardware Testing
```cpp
// Test individual pins
void test_gpio_pins() {
    ESP_LOGI(TAG, "Testing GPIO pins...");
    
    // Test each pin
    gpio_set_level(GPIO_NUM_1, 1);  // DC high
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(GPIO_NUM_1, 0);  // DC low
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Repeat for other pins
}
```

#### Logic Analyzer Integration
For professional debugging, use a logic analyzer to capture SPI signals and verify timing.

## Performance Optimization

### Memory Optimization
- Use frame buffer efficiently
- Avoid dynamic allocation in real-time code
- Optimize data structures for cache efficiency

### Speed Optimization
- Increase SPI frequency gradually
- Use DMA for large transfers
- Batch drawing operations
- Minimize display() calls

### Power Optimization
- Use lower SPI frequencies when possible
- Implement display sleep modes
- Consider ESP32 power management

## Advanced Features

### Custom Display Controllers
```cpp
class MyCustomController : public DisplayController {
public:
    bool initialize(ESP32_SPI_Driver* spi_driver) override {
        // Custom initialization sequence
        return true;
    }
    
    void setAddrWindow(int16_t x, int16_t y, int16_t w, int16_t h) override {
        // Custom addressing
    }
    
    // Implement other required methods...
};
```

### Multiple Display Support
```cpp
// Create multiple drivers for different displays
ESP32_SPI_Driver oled_driver(oled_config, oled_controller);
ESP32_SPI_Driver tft_driver(tft_config, tft_controller);

// Use different SPI hosts
oled_config.spi_host = SPI2_HOST;
tft_config.spi_host = SPI3_HOST;
```

## Production Considerations

### Hardware Design
- Use proper PCB layout guidelines
- Add appropriate bypass capacitors
- Consider signal integrity for high-speed SPI
- Plan for EMI/EMC compliance

### Software Robustness
- Implement proper error handling
- Add watchdog timer support
- Handle power supply variations
- Implement fail-safe modes

### Manufacturing Testing
- Automated display testing procedures
- SPI communication verification
- Visual inspection protocols

This comprehensive guide should help you successfully develop with MinimalUI on ESP32 platforms. For additional support, check the community forums and GitHub issues.