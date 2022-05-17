
cmake_minimum_required(VERSION 3.17)

project(Chip_8_Emulator)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR C:\\Users\\wagne\\CLionProjects\\libs\\SDL\\include)
set(SDL2_LIB_DIR C:\\Users\\wagne\\CLionProjects\\libs\\SDL\\lib\\x86)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

#set(SFML_DIR "C:\\SFML(32)\\lib\\cmake\\SFML")
#set(SFML_LIB_DIR "C:\\SFML(32)\\lib\\")
#include_directories(${SFML_LIB_DIR})
#find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)
#add_executable(Chip_8_Emulator main.cpp chip.h graphics.h)
#target_link_libraries(Chip_8_Emulator sfml-system sfml-main sfml-window sfml-graphics sfml-audio)
#target_link_libraries(${PROJECT_NAME} SDL2main SDL2)

set(SFML_DIR "C:\\SFML(32)\\lib\\cmake\\SFML")
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)


add_executable(Chip_8_Emulator src/main.cpp src/chip.h src/graphics.h)
target_link_libraries(Chip_8_Emulator sfml-graphics sfml-audio)
add_definitions(-DSDL_MAIN_HANDLED)