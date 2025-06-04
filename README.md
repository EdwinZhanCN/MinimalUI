# MinimalUI
MinimalUI is A Cross Platform UI Framework For SPI Display

## Documentation
Install Vitepress Denpendencies:

```shell
npm install
```

start the documentation server.

```shell
npm run docs:dev
```

Check you terminal to find the url that web runs on.


## Development

Project Structure

```txt
project-root/
├── framework/                  # Core UI framework (platform-agnostic)
│   ├── include/
│   │   ├── Component.h
│   │   ├── GraphicsDriver.h
│   │   └── Event.h
│   └── src/
│       ├── Component.cpp
│       └── ...
├── platforms/                  # Platform-specific implementations
│   ├── esp32/
│   │   ├── ESP32_SPI_Driver.h
│   │   ├── ESP32_SPI_Driver.cpp
│   │   └── main_esp32.cpp
│   ├── stm32/
│   │   ├── STM32_SPI_Driver.h
│   │   ├── STM32_SPI_Driver.cpp
│   │   └── main_stm32.cpp
│   └── jetson/
│       ├── Jetson_FB_Driver.h
│       ├── Jetson_FB_Driver.cpp
│       └── main_jetson.cpp
├── components/                 # Reusable UI components
│   ├── include/
│   │   ├── Label.h
│   │   └── Rectangle.h
│   └── src/
│       ├── Label.cpp
│       └── Rectangle.cpp
├── examples/
│   └── basic_ui/
│       └── App.cpp             # Example UI layout
└── CMakeLists.txt
```

## Contribute

**IN COMMIT MESSAGE**, use following tags prefix:
- [Doc] - Documentation Update
- [BugFix] - Bug fixing
- [Refactor] - File/Project Refactoring
- [Feature] - Making Progress on Feature
