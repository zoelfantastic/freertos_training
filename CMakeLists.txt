cmake_minimum_required(VERSION 3.10)
project(freertos-qemu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

message (STATUS "Selected tutorial: ${TUTORIAL}" )

if( NOT DEFINED TUTORIAL )
    set( TUTORIAL createtask )
endif()

set( FREERTOS_KERNEL_PATH "FreeRTOS-Kernel" )
file( GLOB FREERTOS_TUTORIAL_SOURCES "source/tutorial_${TUTORIAL}/*.c" )

# Add the freertos_config for FreeRTOS-Kernel.
message (INCLUDE_DIRECTORIES "Include directories: ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorial_${TUTORIAL}/include" )
add_library( freertos_config INTERFACE )
target_include_directories( freertos_config
                            INTERFACE
                            ${CMAKE_CURRENT_SOURCE_DIR}/source/tutorial_${TUTORIAL}/include )

# Select heap 3.
set( FREERTOS_HEAP "3" CACHE STRING "" FORCE )

# Select the Posix port.
set( FREERTOS_PORT "GCC_POSIX" CACHE STRING "" FORCE )

# Add the FreeRTOS-Kernel subdirectory.
add_subdirectory( ${FREERTOS_KERNEL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/FreeRTOS-Kernel )

add_executable( freertos_example
               ${FREERTOS_TUTORIAL_SOURCES} )

target_link_libraries( freertos_example freertos_kernel freertos_config )