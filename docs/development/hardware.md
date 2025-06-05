---
title: Hardware Connections Guide
description: Complete guide for wiring displays and hardware setup for MinimalUI
---

# Hardware Connections Guide

This guide provides detailed information about connecting displays and other hardware components to your development board for use with MinimalUI.

## General Connection Principles

### Signal Integrity
- **Keep wires short** (< 10cm for SPI signals)
- **Use twisted pairs** for clock and data lines when possible
- **Avoid parallel routing** of high-speed signals
- **Add ground reference** between signal groups

### Power Supply
- **Stable 3.3V supply** is critical for display operation
- **Adequate current capacity** (typically 50-100mA for small displays)
- **Decoupling capacitors** near each IC (100nF ceramic + 10µF electrolytic)
- **Separate analog and digital grounds** when possible

### Mechanical Considerations
- **Strain relief** for cables and connectors
- **Secure mounting** to prevent connection issues
- **Environmental protection** for exposed connections
- **Accessibility** for debugging and maintenance

## SPI Display Connections

### Standard 4-Wire SPI Interface

All SPI displays use a common interface with these signals:

| Signal | Function | Direction | Notes |
|--------|----------|-----------|-------|
| **VCC** | Power Supply | MCU → Display | 3.3V (some accept 5V) |
| **GND** | Ground | MCU ↔ Display | Common reference |
| **SCK/CLK** | Serial Clock | MCU → Display | Up to 80MHz depending on display |
| **SDA/MOSI** | Serial Data | MCU → Display | Data from master to slave |
| **CS** | Chip Select | MCU → Display | Active low device selection |
| **DC/A0** | Data/Command | MCU → Display | High=data, Low=command |
| **RST/RES** | Reset | MCU → Display | Active low reset signal |

### Pin Functions Explained

#### VCC (Power)
- **Voltage**: Typically 3.3V, some displays accept 3.3V-5V
- **Current**: 20-100mA depending on display size and brightness
- **Filtering**: Add 100nF ceramic capacitor close to display VCC pin
- **Protection**: Consider reverse polarity protection diode

#### GND (Ground)
- **Connection**: Direct connection to MCU ground
- **Quality**: Use shortest, thickest wire possible
- **Multiple grounds**: Connect multiple GND pins if available on display

#### SCK/CLK (Serial Clock)
- **Frequency**: Start with 1-4MHz, increase gradually up to 20-80MHz
- **Duty cycle**: 50% recommended for best signal integrity
- **Rise/fall time**: Fast edges for high-speed operation
- **Termination**: May need series resistor for very high speeds

#### SDA/MOSI (Master Out Slave In)
- **Data timing**: Valid on clock edge (mode dependent)
- **Signal integrity**: Most critical signal for data accuracy
- **Drive strength**: Ensure adequate MCU output drive capability

#### CS (Chip Select)
- **Logic**: Active low (0V = selected, 3.3V = deselected)
- **Timing**: Setup and hold times critical for reliable operation
- **Multiple devices**: Unique CS for each device on shared SPI bus

#### DC/A0 (Data/Command)
- **Function**: Selects between command (low) and data (high) mode
- **Timing**: Must be stable before and during SPI transaction
- **Critical signal**: Incorrect DC state causes display malfunction

#### RST/RES (Reset)
- **Function**: Hardware reset of display controller
- **Active state**: Active low (0V = reset active)
- **Timing**: Hold low for minimum 10ms, wait 100ms after release
- **Power-on reset**: Essential for reliable initialization

## ESP32 Platform Connections

### ESP32-C6 (Seeed Studio XIAO)

#### Recommended Pinout
```
ESP32-C6 Pin    Display Pin     Function        Notes
-----------     -----------     --------        -----
3.3V            VCC             Power           Stable 3.3V required
GND             GND             Ground          Multiple connections OK
GPIO 18         DIN/SDA         Data (MOSI)     Can use other GPIO
GPIO 19         CLK/SCK         Clock           Can use other GPIO  
GPIO 2          CS              Chip Select     Any available GPIO
GPIO 1          DC/A0           Data/Command    Any available GPIO
GPIO 0          RST/RES         Reset           Any available GPIO (avoid strapping pins)
```

#### Alternative Pinouts
**Option 1 - Using different GPIOs:**
```cpp
spi_config.mosi_pin = 4;    // GPIO4 - MOSI
spi_config.sclk_pin = 5;    // GPIO5 - SCLK
spi_config.cs_pin = 6;      // GPIO6 - CS
spi_config.dc_pin = 7;      // GPIO7 - DC
spi_config.rst_pin = 8;     // GPIO8 - RST
```

**Option 2 - Avoiding strapping pins:**
```cpp
// Avoid GPIO0, GPIO8, GPIO9 (strapping pins)
spi_config.mosi_pin = 18;   // GPIO18 - MOSI
spi_config.sclk_pin = 19;   // GPIO19 - SCLK
spi_config.cs_pin = 20;     // GPIO20 - CS
spi_config.dc_pin = 21;     // GPIO21 - DC
spi_config.rst_pin = 10;    // GPIO10 - RST
```

#### Pin Considerations for ESP32-C6
- **Strapping pins**: GPIO0, GPIO8, GPIO9 - use carefully
- **USB pins**: GPIO12, GPIO13 - avoid if using USB
- **JTAG pins**: GPIO4, GPIO5, GPIO6, GPIO7 - can be used but affects debugging
- **SPI flash**: GPIO24-29 - avoid these pins

### ESP32-S3 DevKitC

#### Standard Configuration
```
ESP32-S3 Pin    Display Pin     Function        Notes
-----------     -----------     --------        -----
3.3V            VCC             Power           Up to 500mA available
GND             GND             Ground          Multiple connections recommended
GPIO 11         DIN/SDA         Data (MOSI)     HSPI default MOSI
GPIO 12         CLK/SCK         Clock           HSPI default SCLK
GPIO 10         CS              Chip Select     Any available GPIO
GPIO 9          DC/A0           Data/Command    Any available GPIO
GPIO 8          RST/RES         Reset           Any available GPIO
```

#### Using FSPI (SPI2) instead of HSPI (SPI3)
```cpp
spi_config.spi_host = SPI2_HOST;  // Use SPI2 instead of SPI3
spi_config.mosi_pin = 35;         // SPI2 default MOSI
spi_config.sclk_pin = 36;         // SPI2 default SCLK
// CS, DC, RST can be any available GPIO
```

### ESP32 (Original)

#### Classic ESP32 DevKit Pinout
```
ESP32 Pin       Display Pin     Function        Notes
---------       -----------     --------        -----
3.3V            VCC             Power           Check current capability
GND             GND             Ground          Use thick wire
GPIO 23         DIN/SDA         Data (MOSI)     VSPI default MOSI
GPIO 18         CLK/SCK         Clock           VSPI default SCLK
GPIO 5          CS              Chip Select     Any available GPIO
GPIO 2          DC/A0           Data/Command    Any available GPIO
GPIO 4          RST/RES         Reset           Any available GPIO
```

#### Pin Restrictions for ESP32
- **Input only**: GPIO34, GPIO35, GPIO36, GPIO39
- **Strapping pins**: GPIO0, GPIO2, GPIO5, GPIO12, GPIO15
- **SPI flash**: GPIO6-11 (avoid these)
- **Internal use**: GPIO1, GPIO3 (UART)

## Display-Specific Connections

### SSD1309 OLED (128x64)

#### Module Types
**Type 1 - 7-pin module:**
```
Pin#    Name    Connection
----    ----    ----------
1       GND     ESP32 GND
2       VCC     ESP32 3.3V
3       SCK     ESP32 GPIO (Clock)
4       SDA     ESP32 GPIO (Data)
5       RES     ESP32 GPIO (Reset)
6       DC      ESP32 GPIO (Data/Command)
7       CS      ESP32 GPIO (Chip Select)
```

**Type 2 - 4-pin I2C/SPI module:**
```
SPI Mode Configuration:
Pin#    Name    Connection          Notes
----    ----    ----------          -----
1       GND     ESP32 GND           Ground
2       VCC     ESP32 3.3V          Power (3.3V-5V)
3       SCL     ESP32 GPIO (Clock)  SPI Clock
4       SDA     ESP32 GPIO (Data)   SPI Data
```

#### Power Requirements
- **Voltage**: 3.3V (can accept up to 5V on some modules)
- **Current**: 20-50mA typical, 100mA peak
- **Startup current**: May spike to 150mA briefly during initialization

#### Configuration Jumpers
Some modules have solder jumpers for interface selection:
- **J1**: SPI/I2C selection (bridge for SPI mode)
- **J2**: 3.3V/5V selection
- **J3**: Address selection (for I2C mode)

### SSD1306 OLED (Future Support)

#### Differences from SSD1309
- Very similar interface and connections
- Minor command differences in software
- Same electrical characteristics
- Same pinout and wiring

### ILI9341 TFT (Planned Support)

#### Expected Connections
```
ESP32 Pin       ILI9341 Pin     Function
---------       -----------     --------
3.3V            VCC/LED         Power and backlight
GND             GND             Ground
GPIO 23         SDI/MOSI        Data
GPIO 18         SCK             Clock
GPIO 5          CS              Chip Select
GPIO 2          DC/RS           Data/Command
GPIO 4          RST             Reset
GPIO 15         SDO/MISO        Data out (optional)
```

#### Additional ILI9341 Considerations
- **Backlight**: LED pin may need current limiting resistor
- **Touch interface**: Additional pins for touch controller
- **Higher current**: Up to 200mA with backlight
- **Faster SPI**: Can handle 20-80MHz SPI clock

## Breadboard vs. PCB Considerations

### Breadboard Connections

#### Advantages
- Quick prototyping
- Easy to modify
- No soldering required
- Reusable components

#### Disadvantages
- Unreliable connections
- Parasitic capacitance and inductance
- Limited to low-speed signals
- Mechanical instability

#### Best Practices for Breadboard
- Use quality breadboard with tight connections
- Keep wire lengths minimal
- Use solid core wire, not stranded
- Make multiple ground connections
- Test connections with multimeter

### Perfboard/Protoboard

#### Advantages
- More reliable than breadboard
- Solderable connections
- Supports higher frequencies
- Good for permanent prototypes

#### Layout Guidelines
- Place bypass capacitors close to IC power pins
- Use ground plane or grid where possible
- Keep high-speed signals short and direct
- Separate analog and digital sections

### PCB Design

#### Professional Development
For production or reliable development:
- Design custom PCB with proper layout
- Use 4-layer board with dedicated ground/power planes
- Follow high-speed design guidelines
- Include test points for debugging

#### PCB Layout Rules
- **Trace width**: Minimum 0.1mm for signals, 0.2mm+ for power
- **Via size**: 0.2mm drill, 0.4mm pad for hand soldering
- **Spacing**: Minimum 0.1mm between traces
- **Ground plane**: Continuous ground plane under high-speed signals

## Connection Quality and Testing

### Visual Inspection
- **Solder joints**: Shiny, concave fillets
- **Wire integrity**: No nicks or damaged insulation
- **Connector engagement**: Fully seated connections
- **Strain relief**: No mechanical stress on connections

### Electrical Testing

#### Continuity Testing
```bash
# Use multimeter in continuity mode
# Test each connection from MCU pin to display pin
# Should beep/show low resistance (< 1Ω)
```

#### Power Supply Testing
```bash
# Measure voltage at display VCC pin
# Should be stable 3.3V ± 0.1V
# Measure current consumption
# Should be < 100mA for small displays
```

#### Signal Quality Testing
For advanced debugging, use oscilloscope to check:
- **Clock signal**: Clean square wave, correct frequency
- **Data signal**: Valid logic levels, proper timing
- **Setup/hold times**: Data stable relative to clock edges

### Common Connection Problems

#### Intermittent Connections
**Symptoms**: Works sometimes, fails randomly
**Causes**: Loose connections, cold solder joints, flexing wires
**Solutions**: Re-solder connections, add strain relief, use proper connectors

#### Ground Loops
**Symptoms**: Display noise, erratic behavior, EMI issues
**Causes**: Multiple ground paths, poor grounding
**Solutions**: Single-point grounding, star ground topology

#### Power Supply Issues
**Symptoms**: Display doesn't initialize, random resets, dim display
**Causes**: Voltage drop, inadequate current, noise
**Solutions**: Thicker power wires, better regulation, bypass capacitors

## Safety Considerations

### Electrical Safety
- **ESD protection**: Use anti-static wrist strap when handling components
- **Power sequencing**: Apply power before signals when possible
- **Overcurrent protection**: Use current-limited power supplies
- **Isolation**: Isolate high-voltage circuits from low-voltage logic

### Mechanical Safety
- **Sharp edges**: File or cover sharp PCB edges
- **Hot components**: Allow cooling time after soldering
- **Tool safety**: Use proper tools and techniques
- **Eye protection**: Safety glasses when soldering or cutting

## Troubleshooting Connection Issues

### Systematic Debugging Approach

#### 1. Power Supply Check
- Verify 3.3V at display VCC pin
- Check current consumption (should be reasonable)
- Look for voltage drops under load
- Test with different power source

#### 2. Signal Continuity
- Test each signal path with multimeter
- Verify pin assignments match code configuration
- Check for short circuits between adjacent pins
- Test signal integrity with oscilloscope if available

#### 3. Timing Verification
- Start with low SPI frequency (1MHz)
- Gradually increase frequency while testing
- Check setup and hold times if using oscilloscope
- Verify clock polarity and phase settings

#### 4. Software Configuration
- Double-check pin assignments in code
- Verify SPI mode settings (CPOL, CPHA)
- Check initialization sequence
- Enable debug logging for detailed information

### Advanced Debugging Tools

#### Logic Analyzer
- Capture SPI transactions
- Verify command sequences
- Check timing relationships
- Decode protocol automatically

#### Oscilloscope
- Measure signal quality and timing
- Check for reflections and overshoot
- Verify power supply stability
- Analyze noise and interference

#### Protocol Analyzer
- Specialized SPI/I2C analyzers
- Real-time protocol decoding
- Error detection and analysis
- Performance measurement

This comprehensive hardware guide should help you create reliable connections for your MinimalUI projects. Remember that good connections are the foundation of any successful embedded project!