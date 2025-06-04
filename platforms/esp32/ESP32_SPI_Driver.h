// platforms/esp32/ESP32_SPI_Driver.h
#pragma once

#include "../../framework/include/GraphicsDriver.h"
#include "driver/spi_master.h"

class ESP32SPIDriver : public GraphicsDriver {
public:
    ESP32SPIDriver(spi_host_device_t host, int dc_pin, int cs_pin, int rst_pin = -1);
    ~ESP32SPIDriver();

    bool init() override;
    // 实现所有抽象方法...

private:
    spi_device_handle_t spi;
    int dc_pin;
    int cs_pin;
    int rst_pin;

    void sendCommand(uint8_t cmd);
    void sendData(uint8_t* data, size_t len);
};
