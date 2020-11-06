cmake_minimum_required(VERSION 3.0)

set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(BOOST_POLYGON_INCLUDE_DIRS "D:/vcpkg/vcpkg/installed/x64-windows/include/boost/polygon/")

set(SOURCES 
    src/main.cpp 
    ${BOOST_POLYGON_INCLUDE_DIRS}polygon.hpp 
    include/gtl_poly_serialize.h 
    include/gtl_poly_types.h
)

set(data_folder data/)
set(resource_folder resource/)


project(visualization)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
# find_package(Thor 2 REQUIRED)

file(COPY ${data_folder} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data) 
file(COPY ${resource_folder} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resource) 

add_executable(visualization ${SOURCES})

target_include_directories(visualization PRIVATE ${BOOST_POLYGON_INCLUDE_DIRS} include)

target_link_libraries(visualization PRIVATE sfml-graphics)
# target_link_libraries(visualization PRIVATE ${THOR_LIBRARY})