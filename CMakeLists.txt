# A CMakeLists.txt file used to build the project along with vcpkg having downloaded 
# the following dependencies already:
#    - SFML 
#    - ImGui
#    - ImGui-SFML
#    - Boost.Polygon
# (Please install dependencies above in vcpkg before building the project)
# (Hint: You have to add "-DCMAKE_TOOLCHAIN_FILE=<your-path-to-vcpkg.cmake>" when you configure the project)

cmake_minimum_required(VERSION 3.10)

project(visualization)

configure_file(cmake/cmake_variables.h.in "${CMAKE_SOURCE_DIR}/include/cmake_variables.h")

set(BOOST_POLYGON_INCLUDE_DIR "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/boost/polygon/")

aux_source_directory(src SOURCES)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)

add_executable(visualization ${SOURCES})

target_include_directories(visualization 
    PRIVATE ${BOOST_POLYGON_INCLUDE_DIR} 
    PRIVATE include/
)

target_link_libraries(visualization 
    PRIVATE sfml-graphics
    PRIVATE ImGui-SFML::ImGui-SFML
    PRIVATE unofficial::nativefiledialog::nfd
)

# -------------check all cmake variables----------------
# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

# message("${SOURCES}")