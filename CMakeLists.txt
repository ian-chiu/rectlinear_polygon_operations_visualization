cmake_minimum_required(VERSION 3.10)

project(visualization)

set(CMAKE_TOOLCHAIN_FILE "${_VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake")
set(BOOST_POLYGON_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/boost/polygon/")
# set(THOR_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/Thor")
# set(THOR_LIB "${VCPKG_CMAKE_FIND_ROOT_PATH}/lib/")

aux_source_directory(src/ sources)
aux_source_directory(data/ datas)
aux_source_directory(resource/ resources)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

# build and find thor
file(COPY cmake/FindThor.cmake DESTINATION "${_VCPKG_ROOT_DIR}/packages/thor_${VCPKG_TARGET_TRIPLET}/cmake")
list(APPEND CMAKE_MODULE_PATH "${_VCPKG_ROOT_DIR}/packages/thor_${VCPKG_TARGET_TRIPLET}/cmake")
find_package(Thor 2.0 REQUIRED)
# message(${CMAKE_SOURCE_DIR}/dependencies/Thor/cmake/)
# find_package(Thor 2.0 REQUIRED)

file(COPY data/ DESTINATION data/)
file(COPY resource/ DESTINATION resource/) 

foreach(data_file IN ${datas})
    configure_file(data_file data/ COPYONLY)
endforeach(data_file)

add_executable(visualization ${sources})

target_include_directories(visualization 
    PRIVATE ${BOOST_POLYGON_INCLUDE_DIR} 
    PRIVATE include/
)

target_link_libraries(visualization 
    PRIVATE sfml-graphics
    PRIVATE ${THOR_LIBRARY}
)


# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()