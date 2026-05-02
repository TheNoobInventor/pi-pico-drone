# Add subdirectories
add_subdirectory(power_status)
add_subdirectory(sensors)

# Add executable
add_executable(${project_name} main.cpp )

# Add the standard include files to the build
target_include_directories(${project_name} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add the standard library to the build
target_link_libraries(${project_name}
        drone_sensors
        pico_stdlib
        pico_cyw43_arch_none
        pico_multicore
        power_status_adc
        hardware_adc
        hardware_i2c
        hardware_uart
        FreeRTOS-Kernel-Heap4
        )

# Enable USB output and UART output
pico_enable_stdio_usb(${project_name} 1)
pico_enable_stdio_uart(${project_name} 1)

# Create uf2, map, bin and hex files
pico_add_extra_outputs(${project_name})
