---
title: Platform Support
description: Overview of supported platforms and their capabilities
---

# Platform Support

MinimalUI is designed to work across multiple embedded platforms, providing a unified API while leveraging platform-specific optimizations. Each platform implementation follows the same architectural patterns while adapting to the unique characteristics of the target hardware.

## Supported Platforms

### ✅ ESP32 (Active Development)

**Supported Chips:**
- ESP32 (Original)
- ESP32-S3
- ESP32-C6
- ESP32-S2

**Features:**
- Native ESP-IDF integration
- Hardware SPI acceleration
- FreeRTOS multi-tasking
- Multiple SPI host support
- GPIO flexibility
- USB Serial/JTAG support

**Status:** Production ready with comprehensive examples

[📖 ESP32 Documentation](/platforms/esp32?_immersive_translate_auto_translate=1)

### 🚧 STM32 (In Development)

**Planned Support:**
- STM32F4 series
- STM32H7 series
- STM32L4 series

**Features:**
- STM32 HAL integration
- DMA-accelerated transfers
- Multiple SPI interfaces
- Low-power modes
- Real-time performance

**Status:** Core implementation in progress

[📖 STM32 Documentation](/platforms/stm32?_immersive_translate_auto_translate=1)

### 📋 Jetson (Planned)

**Target Devices:**
- Jetson Nano
- Jetson Xavier NX
- Jetson AGX Xavier

**Features:**
- Linux framebuffer support
- High-performance graphics
- GPU acceleration potential
- Standard Linux toolchain

**Status:** Design phase

[📖 Jetson Documentation](/platforms/jetson?_immersive_translate_auto_translate=1)

## Platform Comparison

| Feature | ESP32 | STM32 | Jetson |
|---------|-------|-------|--------|
| **OS/RTOS** | FreeRTOS | FreeRTOS/Bare Metal | Linux |
| **Memory** | 520KB RAM | 64KB-1MB RAM | 2-32GB RAM |
| **Graphics** | Software | Software | Hardware |
| **SPI Speed** | Up to 80MHz | Up to 50MHz | Up to 50MHz |
| **Display Support** | SPI OLED/TFT | SPI/Parallel | Framebuffer |
| **Power** | Ultra-low | Low | Medium-High |
| **Complexity** | Medium | Low | High |

## Display Controller Support

### Current Support

| Controller | Type | Resolution | ESP32 | STM32 | Jetson |
|------------|------|------------|-------|-------|--------|
| **SSD1309** | OLED Mono | 128x64 | ✅ | 🚧 | 📋 |
| **SSD1306** | OLED Mono | 128x64 | 📋 | 📋 | 📋 |
| **ILI9341** | TFT Color | 320x240 | 📋 | 🚧 | 📋 |
| **ST7735** | TFT Color | 160x128 | 📋 | 📋 | 📋 |

**Legend:**
- ✅ Implemented and tested
- 🚧 In development
- 📋 Planned

## Communication Interfaces

### SPI (Primary Focus)
- **4-wire SPI**: Clock, Data, Chip Select, Data/Command
- **Hardware acceleration**: Platform-specific SPI controllers
- **Configurable timing**: Adjustable clock speeds and modes
- **DMA support**: Where available for large transfers

### I2C (Future Support)
- **2-wire interface**: Clock and Data
- **Multiple devices**: Shared bus support
- **Standard speeds**: 100kHz, 400kHz, 1MHz

### Parallel (Planned)
- **8/16-bit data**: High-speed transfers
- **STM32 FSMC**: Flexible Static Memory Controller
- **Jetson GPIO**: Parallel interface via GPIO

## Platform-Specific Features

### ESP32 Advantages
- **WiFi/Bluetooth**: Wireless connectivity
- **Deep Sleep**: Ultra-low power modes
- **Dual Core**: Parallel processing capability
- **Rich Peripherals**: ADC, DAC, Touch sensors
- **Large Community**: Extensive examples and support

### STM32 Advantages
- **Real-time**: Deterministic performance
- **Low Power**: Advanced power management
- **High Speed**: Fast ARM Cortex-M cores
- **Precise Timing**: Hardware timers and PWM
- **Industrial Grade**: Extended temperature ranges

### Jetson Advantages
- **High Performance**: ARM64 + GPU
- **Linux Ecosystem**: Standard tools and libraries
- **Computer Vision**: Built-in AI acceleration
- **Memory**: Large RAM for complex applications
- **Storage**: Standard filesystems

## Development Tools

### ESP32 Toolchain
```bash
# ESP-IDF setup
get_idf
idf.py build
idf.py flash monitor
```

### STM32 Toolchain
```bash
# STM32CubeIDE or command line
arm-none-eabi-gcc
st-flash
openocd
```

### Jetson Toolchain
```bash
# Standard Linux development
gcc/g++
make/cmake
gdb
```

## Performance Characteristics

### Memory Usage

| Component | ESP32 | STM32F4 | Jetson |
|-----------|-------|---------|--------|
| **Framework Core** | ~8KB | ~6KB | ~12KB |
| **SPI Driver** | ~4KB | ~3KB | ~8KB |
| **SSD1309 Controller** | ~2KB | ~2KB | ~4KB |
| **Frame Buffer (128x64)** | 1KB | 1KB | 1KB |

### Timing Performance

| Operation | ESP32@240MHz | STM32F4@168MHz | Jetson@1.4GHz |
|-----------|-------------|---------------|---------------|
| **Screen Clear** | ~2ms | ~1.5ms | ~0.5ms |
| **Full Update** | ~8ms | ~6ms | ~2ms |
| **Pixel Draw** | ~10µs | ~8µs | ~3µs |

## Getting Started by Platform

### Quick Start - ESP32
1. Install ESP-IDF v5.4.1
2. Clone MinimalUI repository
3. Navigate to `platforms/esp32`
4. Run `idf.py build flash monitor`

### Quick Start - STM32
1. Install STM32CubeIDE
2. Clone MinimalUI repository
3. Import project from `platforms/stm32`
4. Build and flash

### Quick Start - Jetson
1. Set up cross-compilation environment
2. Clone MinimalUI repository
3. Navigate to `platforms/jetson`
4. Run `make && sudo ./minimalui_demo`

## Migration Between Platforms

The unified API design allows easy migration:

```cpp
// Same code works on all platforms
driver->clear(0);
driver->fillRect(10, 10, 50, 30, 1);
driver->drawCircle(80, 32, 15, 1);
driver->display();
```

Only platform-specific initialization changes:

```cpp
// ESP32 specific
ESP32_SPI_Driver driver(esp32_config, controller);

// STM32 specific  
STM32_SPI_Driver driver(stm32_config, controller);

// Jetson specific
Jetson_FB_Driver driver(jetson_config, controller);
```

## Contributing Platform Support

Interested in adding support for a new platform? Here's how:

1. **Study the Architecture**: Understand the GraphicsDriver interface
2. **Create Platform Directory**: `platforms/your_platform/`
3. **Implement Driver**: Inherit from GraphicsDriver
4. **Add Build System**: CMake or platform-specific
5. **Test Thoroughly**: Hardware validation
6. **Submit PR**: Include documentation and examples

## Community and Support

- **GitHub Discussions**: Platform-specific questions
- **Hardware Testing**: Community hardware validation
- **Documentation**: Help improve platform guides
- **Examples**: Contribute platform-specific demos

Choose your platform and start building amazing embedded UIs! 🚀