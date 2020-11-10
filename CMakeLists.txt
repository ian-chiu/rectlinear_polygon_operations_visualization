# A CMakeLists.txt file used to build the project along with vcpkg having downloaded 
# the following dependencies already:
#    - SFML 
#    - Thor 
#    - Boost.Polygon
# (Please install dependencies above in vcpkg before building the project)
# (Hint: You have to add "-DCMAKE_TOOLCHAIN_FILE:<your-path-to-vcpkg.cmake>" when you 
#   configure the project)

cmake_minimum_required(VERSION 3.10)

project(visualization)

set(CMAKE_TOOLCHAIN_FILE "${_VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake")
set(BOOST_POLYGON_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/boost/polygon/")

aux_source_directory(src/ SOURCES)
aux_source_directory(data/ DATAS)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# Copy FindThor.cmake to vcpkg, and then find_package() can automatically find thor library for us
file(COPY cmake/FindThor.cmake DESTINATION "${_VCPKG_ROOT_DIR}/packages/thor_${VCPKG_TARGET_TRIPLET}/cmake")
list(APPEND CMAKE_MODULE_PATH "${_VCPKG_ROOT_DIR}/packages/thor_${VCPKG_TARGET_TRIPLET}/cmake")
find_package(Thor 2.0 REQUIRED)

file(COPY data/ DESTINATION data/)
file(COPY resource/ DESTINATION resource/) 

foreach(data_file IN ${DATAS})
    configure_file(data_file data/ COPYONLY)
endforeach(data_file)

add_executable(visualization ${SOURCES})

target_include_directories(visualization 
    PRIVATE ${BOOST_POLYGON_INCLUDE_DIR} 
    PRIVATE include/
)

target_link_libraries(visualization 
    PRIVATE sfml-graphics
    PRIVATE ${THOR_LIBRARY}
)


# -------------check all cmake variables----------------
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()