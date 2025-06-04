idf_component_register(
    SRCS "ESP32_SPI_Driver.cpp"
         "controllers/SSD1309Controller.cpp"
    INCLUDE_DIRS "." "controllers"
    REQUIRES driver spi_flash esp_system freertos framework
    PRIV_REQUIRES esp_common
)

# Add C++17 support
target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)

# Add platform-specific definitions
target_compile_definitions(${COMPONENT_LIB} PRIVATE
    PLATFORM_ESP32=1
    CONFIG_UI_MAX_ELEMENTS=32
)
