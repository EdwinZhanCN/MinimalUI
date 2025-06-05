---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "MinimalUI"
  text: "Cross-Platform UI Framework for SPI Displays"
  tagline: Lightweight, fast, and designed for embedded systems - ESP32, STM32, and Jetson support
  actions:
    - theme: brand
      text: Get Started
      link: /getting-started?_immersive_translate_auto_translate=1
    - theme: alt
      text: View on GitHub
      link: https://github.com/EdwinZhanCN/MinimalUI

features:
  - title: 🚀 Multi-Platform Support
    details: Native support for ESP32 (ESP-IDF), STM32 (HAL), and Jetson (Linux framebuffer) with unified API
  - title: 🎯 SPI Display Optimized
    details: Specifically designed for SPI displays including SSD1306, SSD1309, ILI9341, and more controllers
  - title: 🔧 Modular Architecture
    details: Pluggable display controllers, platform-specific drivers, and reusable UI components
  - title: ⚡ High Performance
    details: Optimized for real-time embedded systems with minimal memory footprint and fast rendering
  - title: 🛠️ Easy Integration
    details: Simple CMake integration with ESP-IDF projects and other build systems
  - title: 📦 Rich Components
    details: Built-in UI components like labels, buttons, rectangles, and custom drawing primitives
---

## Quick Example

```cpp
#include "ESP32_SPI_Driver.h"
#include "SSD1309Controller.h"

// Create SPI driver for ESP32
ESP32_SPI_Config config;
config.dc_pin = 1;
config.cs_pin = 2;
config.rst_pin = 0;

auto controller = std::make_unique<SSD1309Controller>(oled_config);
ESP32_SPI_Driver driver(config, std::move(controller));

// Initialize and draw
driver.initialize();
driver.fillRect(10, 10, 50, 30, 1);
driver.display();
```

## Supported Hardware

| Platform | MCU/SoC | Display Controllers | Status |
|----------|---------|-------------------|---------|
| ESP32 | ESP32, ESP32-S3, ESP32-C6 | SSD1309, SSD1306 | ✅ Active |
| STM32 | STM32F4, STM32H7 | SSD1306, ILI9341 | 🚧 In Progress |
| Jetson | Jetson Nano, Xavier | Framebuffer based | 🚧 Planned |

## Why MinimalUI?

**🎯 Built for Embedded Systems**
- Designed specifically for resource-constrained environments
- Minimal RAM usage with efficient frame buffering
- Real-time performance considerations

**🔌 Hardware Abstraction**
- Clean separation between UI logic and hardware implementation
- Easy to port to new platforms and display controllers
- Consistent API across all supported platforms

**📱 Modern C++ Design**
- RAII-based resource management
- Smart pointers for memory safety
- Template-based component system

**🧪 Production Ready**
- Extensively tested on real hardware
- Comprehensive error handling
- Detailed documentation and examples
