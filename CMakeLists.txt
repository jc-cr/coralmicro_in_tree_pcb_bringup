cmake_minimum_required(VERSION 3.16)
project(coralmicro_in_tree_pcb_bringup)

# Add the VL53L8CX driver subdirectory first
add_subdirectory(libs/coralmicro_VL53L8CX_ULD_driver)

# Define paths for task configuration
set(TASK_CONFIG_YAML "${CMAKE_CURRENT_SOURCE_DIR}/config/tasks_config.yaml")
set(TASK_CONFIG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/task_config.hh")
set(TASK_CONFIG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/task_config.cc")
set(TASK_GENERATOR_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_tasks.py")

# Define task source files
set(TASK_SOURCES
)

# Add custom command to generate task configuration
add_custom_command(
    OUTPUT ${TASK_CONFIG_HEADER} ${TASK_CONFIG_SOURCE}
    COMMAND python3 ${TASK_GENERATOR_SCRIPT} 
            ${TASK_CONFIG_YAML} 
            ${TASK_CONFIG_HEADER} 
            ${TASK_CONFIG_SOURCE}
    DEPENDS ${TASK_CONFIG_YAML} ${TASK_GENERATOR_SCRIPT}
    COMMENT "Generating task configuration files"
    VERBATIM
)

# Create a custom target for task configuration generation
add_custom_target(${PROJECT_NAME}_generate_task_config
    DEPENDS ${TASK_CONFIG_HEADER} ${TASK_CONFIG_SOURCE}
)

# M4 Core Executable
add_executable_m4(${PROJECT_NAME}_m4
    src/main_m4.cc
)

# Configure M4 executable
target_compile_options(${PROJECT_NAME}_m4 
    PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Os
        -ffunction-sections
        -fdata-sections
        -fno-exceptions
        -fno-rtti
        -g0
        -ffast-math
        -fshort-enums
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        -Wall
        -Wextra
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
)

target_link_options(${PROJECT_NAME}_m4
    PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -Wl,-Map=${PROJECT_NAME}_m4.map
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -Wl,--sort-section=alignment
        -Wl,--cref
)

target_link_libraries(${PROJECT_NAME}_m4
    PRIVATE
        libs_base-m4_freertos
)

# M7 Core Executable
add_executable_m7(${PROJECT_NAME}
    src/main_m7.cc
    ${TASK_CONFIG_SOURCE}
    ${TASK_SOURCES}
    M4_EXECUTABLE
    ${PROJECT_NAME}_m4
)

# Set include directories
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/VL53L8CX_ULD_driver_2.0.0/Platform
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/VL53L8CX_ULD_driver_2.0.0/VL53L8CX_ULD_API/inc
)

# Add dependency on task configuration generation
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_generate_task_config)

# M7 compiler flags remain the same
target_compile_options(${PROJECT_NAME} 
    PRIVATE
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-d16
        -mfloat-abi=hard
        -Os
        -ffunction-sections
        -fdata-sections
        -fno-exceptions
        -fno-rtti
        -g0
        -ffast-math
        -fshort-enums
        -fno-unwind-tables
        -fno-asynchronous-unwind-tables
        -Wall
        -Wextra
        $<$<COMPILE_LANGUAGE:C>:-std=c11>
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
)

# M7 linker flags remain the same
target_link_options(${PROJECT_NAME}
    PRIVATE
        -mcpu=cortex-m7
        -mthumb
        -mfpu=fpv5-d16
        -mfloat-abi=hard
        -Wl,-Map=output.map
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -Wl,--sort-section=alignment
        -Wl,--cref
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        vl53l8cx_driver
        libs_base-m7_freertos
)