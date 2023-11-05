cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(pico_sdk_import.cmake)

project(fws_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_BOARD pico_w)
set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(fws_pico
	main.c
	usb_descriptors.c
)

target_include_directories(fws_pico 
	PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# Pull in our pico_stdlib...
target_link_libraries(fws_pico 
	pico_stdlib 
	tinyusb_device 
	tinyusb_board 
	pico_cyw43_arch_none
)

# create map/bin/hex file etc.
pico_add_extra_outputs(fws_pico)
