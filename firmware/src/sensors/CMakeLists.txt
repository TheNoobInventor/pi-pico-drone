add_library(drone_sensors INTERFACE)

target_sources(drone_sensors INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/mpu6050.cpp
        # ${CMAKE_CURRENT_LIST_DIR}/mpu605000.c
        #${CMAKE_CURRENT_LIST_DIR}/bmp280.c
        )

target_include_directories(drone_sensors INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(drone_sensors INTERFACE
        pico_stdlib
        hardware_i2c
        )


