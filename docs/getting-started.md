---
title: Getting Started
description: Quick start guide for MinimalUI framework
---

# Getting Started

This guide will help you set up MinimalUI on your development environment and create your first project.

## Prerequisites

### For ESP32 Development

- **ESP-IDF v5.4.1** or later
- **CMake 3.16** or later
- **Python 3.8** or later
- **Git**

### For STM32 Development (Coming Soon)

- **STM32CubeIDE** or **STM32CubeMX**
- **ARM GCC Toolchain**

### For Jetson Development (Planned)

- **Ubuntu 18.04** or later
- **GCC 7** or later

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/EdwinZhanCN/MinimalUI.git
cd MinimalUI
```

### 2. ESP32 Setup

#### Install ESP-IDF

```bash
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

#### Build ESP32 Example

```bash
# Navigate to ESP32 platform
cd MinimalUI/platforms/esp32

# Set up ESP-IDF environment
get_idf

# Configure project (optional)
idf.py menuconfig

# Build project
idf.py build

# Flash to device
idf.py -p /dev/<your PORT> flash monitor
```

## Hardware Setup

### ESP32 + SSD1309 OLED

#### Required Components

- ESP32 development board (ESP32-C6, ESP32-S3, or ESP32)
- SSD1309 OLED display (128x64)
- Jumper wires
- Breadboard (optional)

#### Wiring Diagram

| ESP32 Pin | OLED Pin | Function |
|-----------|----------|----------|
| 3.3V | VCC | Power |
| GND | GND | Ground |
| GPIO 18 | DIN/SDA | Data (MOSI) |
| GPIO 19 | CLK/SCK | Clock |
| GPIO 2 | CS | Chip Select |
| GPIO 1 | DC | Data/Command |
| GPIO 0 | RST | Reset |

#### Connection Notes

- Ensure stable connections - loose wires are the most common cause of issues
- Use short wires to minimize signal interference
- Add a 100nF capacitor between VCC and GND near the OLED for power stability
- Consider using a PCB or protoboard for permanent projects

## Your First Project

### 1. Basic Example

Create a simple display test:

```cpp
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ESP32_SPI_Driver.h"
#include "controllers/SSD1309Controller.h"

using namespace MinimalUI;

extern "C" void app_main(void) {
    // Configure SPI
    ESP32_SPI_Config spi_config;
    spi_config.spi_host = SPI2_HOST;
    spi_config.dc_pin = 1;
    spi_config.cs_pin = 2;
    spi_config.rst_pin = 0;
    spi_config.mosi_pin = 18;
    spi_config.sclk_pin = 19;
    spi_config.freq = 4000000;  // 4MHz
    spi_config.spi_mode = 0;

    // Configure OLED
    SSD1309Config oled_config;
    oled_config.width = 128;
    oled_config.height = 64;

    // Create driver
    auto controller = std::make_unique<SSD1309Controller>(oled_config);
    ESP32_SPI_Driver driver(spi_config, std::move(controller));

    // Initialize
    if (!driver.initialize()) {
        ESP_LOGE("MAIN", "Failed to initialize driver");
        return;
    }

    // Draw something
    driver.clear(0);                           // Clear screen
    driver.fillRect(10, 10, 50, 30, 1);       // White rectangle
    driver.drawCircle(80, 32, 15, 1);         // White circle
    driver.display();                         // Update display

    ESP_LOGI("MAIN", "Display initialized successfully!");

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```

### 2. Project Structure

Your project should follow this structure:

```
your_project/
├── CMakeLists.txt
├── main/
│   ├── CMakeLists.txt
│   └── main.cpp
├── components/
│   └── minimalui/          # MinimalUI as a component
└── sdkconfig
```

### 3. Integration as Component

To use MinimalUI as an ESP-IDF component:

```cmake
# In your main CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS "path/to/MinimalUI/framework"
                         "path/to/MinimalUI/platforms/esp32/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(your_project)
```

## Configuration Options

### SPI Configuration

```cpp
ESP32_SPI_Config spi_config;
spi_config.spi_host = SPI2_HOST;     // SPI2_HOST or SPI3_HOST
spi_config.freq = 4000000;           // 1MHz - 20MHz (start with 4MHz)
spi_config.spi_mode = 0;             // SPI mode (0-3)
```

### Display Configuration

```cpp
SSD1309Config oled_config;
oled_config.width = 128;              // Display width
oled_config.height = 64;              // Display height
oled_config.external_vcc = false;     // Use internal charge pump
oled_config.flip_horizontal = false;  // Mirror horizontally
oled_config.flip_vertical = false;    // Mirror vertically
```

## Troubleshooting

### Common Issues

**1. Black Screen / No Display**
- Check wiring connections
- Verify power supply (3.3V stable)
- Try lower SPI frequency (1MHz)
- Add hardware reset delay

**2. Flickering Display**
- Reduce refresh rate
- Check for loose connections
- Add decoupling capacitors

**3. Compilation Errors**
- Ensure ESP-IDF environment is set up
- Check component dependencies
- Verify C++17 support is enabled

**4. Connection Instability**
- Use quality jumper wires
- Consider soldering connections
- Add pull-up resistors if needed

### Debug Tips

```cpp
// Enable detailed logging
esp_log_level_set("ESP32_SPI_Driver", ESP_LOG_DEBUG);
esp_log_level_set("SSD1309Controller", ESP_LOG_DEBUG);

// Test individual functions
driver.clear(1);    // Full white - should be visible
driver.display();
vTaskDelay(pdMS_TO_TICKS(2000));

driver.clear(0);    // Full black
driver.display();
```

## Next Steps

- [Explore the Architecture](/architecture?_immersive_translate_auto_translate=1) to understand the framework design
- [Check Platform-Specific Guides](/platforms/?_immersive_translate_auto_translate=1) for detailed platform information
- [Browse Examples](/examples/?_immersive_translate_auto_translate=1) for more complex projects
- [API Reference](/api/?_immersive_translate_auto_translate=1) for detailed function documentation

## Community and Support

- **GitHub Issues**: Report bugs and request features
- **Discussions**: Ask questions and share projects
- **Wiki**: Community-contributed guides and tips

Happy coding with MinimalUI! 🚀
