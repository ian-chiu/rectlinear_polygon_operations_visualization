# Ensure that this script can only be run with modern versions of CMake that
# use config files
cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(data_folder "${PROJECT_SOURCE_DIR}data/")
# aux_source_directory("${PROJECT_SOURCE_DIR}data/" DATA_FILES)
# message(${DATA_FILES})

# Tell CMake what our project is called
project(visualization)

# Find version 2.5.0 of the SFML library on this computer,
# but only use the graphics package
# If the package can't be found, terminate the script
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

file(COPY ${data_folder} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) 

configure_file(${data_folder}/input.txt
    ${CMAKE_CURRENT_BINARY_DIR}${data_folder} COPYONLY)

# Tell the compiler where the source code is, and add it to the executable
add_executable(visualization src/main.cpp)

# Add the following SFML module to the application:
target_link_libraries(visualization PRIVATE sfml-graphics)