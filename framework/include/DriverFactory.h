// framework/include/DriverFactory.h
#pragma once

#include "GraphicsDriver.h"

class DriverFactory {
public:
    enum class Platform {
        ESP32,
        STM32,
        JETSON
    };

    static GraphicsDriver* createDriver(Platform platform);
};
