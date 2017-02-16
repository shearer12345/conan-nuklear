PROJECT(PackageTest)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

ADD_EXECUTABLE(example example.cpp)
TARGET_LINK_LIBRARIES(example ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "example")
