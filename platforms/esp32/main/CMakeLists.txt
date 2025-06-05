idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES driver esp_system freertos esp32_drivers esp_pm esp_hw_support
)