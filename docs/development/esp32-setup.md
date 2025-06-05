---
title: ESP32 Development Setup
description: Step-by-step guide for setting up ESP32 development environment for MinimalUI
---

# ESP32 Development Setup

This guide provides detailed instructions for setting up your development environment for MinimalUI on ESP32 platforms.

## System Requirements

### Minimum Requirements
- **OS**: Ubuntu 18.04+, macOS 10.15+, or Windows 10
- **RAM**: 4GB minimum, 8GB recommended
- **Storage**: 10GB free space for toolchain and projects
- **USB**: Available USB port for ESP32 connection

### Recommended Setup
- **OS**: Ubuntu 22.04 LTS or macOS 13+
- **RAM**: 16GB for comfortable development
- **Storage**: SSD with 20GB+ free space
- **IDE**: VS Code with ESP-IDF extension

## ESP-IDF Installation

### Ubuntu/Debian Setup

#### 1. Install System Dependencies
```bash
sudo apt-get update
sudo apt-get install git wget flex bison gperf python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
```

#### 2. Create ESP Directory Structure
```bash
mkdir -p ~/esp
cd ~/esp
```

#### 3. Clone ESP-IDF
```bash
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf
cd esp-idf
```

#### 4. Install ESP-IDF Tools
```bash
./install.sh esp32,esp32s3,esp32c6
```

#### 5. Set Environment Variables
```bash
echo 'export IDF_PATH="$HOME/esp/esp-idf"' >> ~/.bashrc
echo 'alias get_idf=". $IDF_PATH/export.sh"' >> ~/.bashrc
source ~/.bashrc
```

### macOS Setup

#### 1. Install Homebrew (if not installed)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### 2. Install Dependencies
```bash
brew install cmake ninja dfu-util python3
```

#### 3. Create ESP Directory
```bash
mkdir -p ~/esp
cd ~/esp
```

#### 4. Clone and Install ESP-IDF
```bash
git clone -b v5.4.1 --recursive https://github.com/espressif/esp-idf.git esp-idf
cd esp-idf
./install.sh esp32,esp32s3,esp32c6
```

#### 5. Configure Shell Environment
For Zsh (default on macOS):
```bash
echo 'export IDF_PATH="$HOME/esp/esp-idf"' >> ~/.zshrc
echo 'alias get_idf=". $IDF_PATH/export.sh"' >> ~/.zshrc
source ~/.zshrc
```

For Bash:
```bash
echo 'export IDF_PATH="$HOME/esp/esp-idf"' >> ~/.bash_profile
echo 'alias get_idf=". $IDF_PATH/export.sh"' >> ~/.bash_profile
source ~/.bash_profile
```

### Windows Setup

#### 1. Download ESP-IDF Installer
- Visit [Espressif Downloads](https://dl.espressif.com/dl/esp-idf/)
- Download ESP-IDF Windows Installer (latest v5.4.1)

#### 2. Run Installation
- Execute the installer as Administrator
- Select "Full Installation" option
- Choose installation directory (default: C:\Espressif)
- Wait for download and installation to complete

#### 3. Verify Installation
- Open "ESP-IDF Command Prompt" from Start Menu
- Run: `idf.py --version`

## Development Environment Setup

### VS Code with ESP-IDF Extension (Recommended)

#### 1. Install VS Code
```bash
# Ubuntu
sudo snap install --classic code

# macOS
brew install --cask visual-studio-code

# Windows: Download from code.visualstudio.com
```

#### 2. Install ESP-IDF Extension
1. Open VS Code
2. Go to Extensions (Ctrl+Shift+X)
3. Search for "ESP-IDF"
4. Install the official Espressif ESP-IDF extension

#### 3. Configure Extension
1. Open Command Palette (Ctrl+Shift+P)
2. Run "ESP-IDF: Configure ESP-IDF Extension"
3. Select "Use existing setup"
4. Point to your ESP-IDF installation path

### Alternative: CLion Setup

#### 1. Install CLion
Download from JetBrains website (free for students)

#### 2. Configure Toolchain
1. Go to File → Settings → Build → Toolchains
2. Add new toolchain:
   - **Name**: ESP32
   - **CMake**: Point to ESP-IDF cmake
   - **Make**: Point to ninja
   - **C Compiler**: xtensa-esp32-elf-gcc
   - **C++ Compiler**: xtensa-esp32-elf-g++

### Command Line Setup

#### 1. Verify Installation
```bash
# Activate ESP-IDF environment
get_idf

# Check version
idf.py --version

# Should output: ESP-IDF v5.4.1
```

#### 2. Test Hello World
```bash
# Copy hello world example
cp -r $IDF_PATH/examples/get-started/hello_world ~/esp/
cd ~/esp/hello_world

# Build
idf.py build

# Should complete without errors
```

## Hardware Setup

### ESP32 Development Boards

#### Recommended Boards for MinimalUI

**ESP32-C6 (Seeed Studio XIAO)**
- **Pros**: Latest features, WiFi 6, compact
- **Cons**: Single core, newer ecosystem
- **Best for**: IoT projects, space-constrained applications

**ESP32-S3 DevKitC-1**
- **Pros**: Dual core, large RAM, proven design
- **Cons**: Higher power consumption
- **Best for**: Complex applications, multimedia

**ESP32 DevKit V1**
- **Pros**: Mature ecosystem, low cost
- **Cons**: Older architecture
- **Best for**: Learning, prototyping

### Driver Installation

#### Linux
Most Linux distributions include drivers automatically. If not:
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Logout and login again, then check:
ls -l /dev/ttyUSB*
```

#### macOS
Install Silicon Labs CP210x drivers:
1. Download from Silicon Labs website
2. Install the package
3. Restart your Mac

#### Windows
- Most boards work with automatic Windows drivers
- For CH340/CH341 chips, download drivers from manufacturer

### USB Connection Testing

#### 1. Connect ESP32 to Computer
Use a high-quality USB cable (data + power, not just power)

#### 2. Identify Port
```bash
# Linux
ls /dev/ttyUSB*

# macOS
ls /dev/cu.usbserial-* /dev/cu.usbmodem*

# Windows (in Command Prompt)
mode
```

#### 3. Test Communication
```bash
# Replace with your actual port
idf.py -p /dev/ttyUSB0 monitor

# Should show chip information when ESP32 boots
```

## MinimalUI Project Setup

### 1. Clone MinimalUI Repository
```bash
cd ~/esp
git clone https://github.com/MinimalUI/MinimalUI.git
cd MinimalUI
```

### 2. Test ESP32 Example
```bash
cd platforms/esp32

# Activate ESP-IDF
get_idf

# Configure project
idf.py menuconfig
```

### 3. Build Configuration
In menuconfig, verify:
- **Target chip**: ESP32/ESP32-S3/ESP32-C6
- **Flash size**: 4MB or larger
- **Partition table**: Single factory app
- **Component config → ESP32-specific**: Enable SPIRAM if available

### 4. Build and Flash
```bash
# Build project
idf.py build

# Flash to device (replace port)
idf.py -p /dev/ttyUSB0 flash

# Monitor output
idf.py -p /dev/ttyUSB0 monitor
```

## Troubleshooting Common Issues

### ESP-IDF Installation Issues

#### "No module named 'serial'"
```bash
pip install pyserial
```

#### "Command not found: idf.py"
```bash
# Re-run environment setup
get_idf

# Or manually:
source $IDF_PATH/export.sh
```

#### Permission denied on Linux
```bash
sudo usermod -a -G dialout $USER
# Logout and login again
```

### Build Issues

#### "CMake Error: CMAKE_C_COMPILER not found"
```bash
# Reinstall ESP-IDF tools
cd $IDF_PATH
./install.sh --reinstall
```

#### "fatal error: esp_err.h: No such file"
```bash
# Check ESP-IDF environment
get_idf
echo $IDF_PATH

# Should point to your ESP-IDF installation
```

### Flash Issues

#### "No serial data received"
- Check USB cable (must support data)
- Try different USB port
- Press RESET button on ESP32

#### "Chip not found"
- Verify correct port selection
- Check if another program is using the port
- Try holding BOOT button while connecting

#### "Permission denied" on port
```bash
# Linux
sudo chmod 666 /dev/ttyUSB0

# Or add user to dialout group permanently
sudo usermod -a -G dialout $USER
```

### Display Issues

#### Black screen after flash
- Check wiring connections
- Verify pin assignments in code
- Try lower SPI frequency (1MHz)

#### Intermittent display
- Use shorter, better quality wires
- Add 100nF capacitor near display VCC/GND
- Check power supply stability

## Performance Optimization

### Development Speed
```bash
# Use ccache for faster builds
export IDF_CCACHE_ENABLE=1

# Parallel compilation
idf.py -j8 build

# Skip unnecessary components
idf.py menuconfig
# → Component config → Disable unused components
```

### Debug Optimization
```bash
# Enable debug symbols
idf.py menuconfig
# → Compiler options → Optimization Level → Debug (-Og)

# Increase log verbosity
idf.py menuconfig
# → Component config → Log output → Default log verbosity → Debug
```

## Next Steps

### 1. Hardware Connection
Follow the [Hardware Connection Guide](/development/hardware?_immersive_translate_auto_translate=1) to wire your display

### 2. First Project
Create your first MinimalUI project using the [Getting Started Guide](/getting-started?_immersive_translate_auto_translate=1)

### 3. Advanced Topics
- [ESP32 Platform Guide](/platforms/esp32?_immersive_translate_auto_translate=1)
- [API Reference](/api/?_immersive_translate_auto_translate=1)
- [Examples](/examples/?_immersive_translate_auto_translate=1)

## Additional Resources

### Official Documentation
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

### Community Resources
- [ESP32 Forum](https://esp32.com/)
- [MinimalUI GitHub](https://github.com/MinimalUI/MinimalUI)
- [ESP32 Reddit Community](https://www.reddit.com/r/esp32/)

### Tools and Utilities
- [ESP Flash Download Tool](https://www.espressif.com/en/support/download/other-tools)
- [ESP32 Exception Decoder](https://github.com/me-no-dev/EspExceptionDecoder)
- [PlatformIO](https://platformio.org/) (alternative to ESP-IDF)

Your development environment is now ready for MinimalUI development on ESP32! 🚀