cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set project name
SET(project_name pi-pico-drone-firmware-v0.1)

# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/FreeRTOS-Kernel)

# SET(EIGEN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/Eigen)

# Pull in Raspberry Pi Pico SDK (must be before project) and import other libraries
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define project
project(${project_name})

# Include directories
include_directories(include/Eigen)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add subdirectories
add_subdirectory(src)
