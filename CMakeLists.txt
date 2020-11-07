cmake_minimum_required(VERSION 3.10)

project(visualization)

set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake")
set(BOOST_POLYGON_INCLUDE_DIRS "${VCPKG_ROOT}installed/x64-windows/include/boost/polygon/")

aux_source_directory(src/ sources)
aux_source_directory(data/ datas)
aux_source_directory(resource/ resources)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

file(COPY resource/ DESTINATION resource/) 

foreach(data_file IN ${datas})
    configure_file(data_file data/ COPYONLY)
endforeach(data_file)


add_executable(visualization ${sources})

target_include_directories(visualization PRIVATE ${BOOST_POLYGON_INCLUDE_DIRS} include/)

target_link_libraries(visualization PRIVATE sfml-graphics)