# Framework core library component for ESP-IDF

# Register the component
idf_component_register(
    SRCS
        "src/DriverFactory.cpp"  # Using the actual source file that exists
    INCLUDE_DIRS
        "include"
    PRIV_INCLUDE_DIRS
        "src"
    # REQUIRES other_component  # Add dependencies if needed
)
