cmake_minimum_required(VERSION 3.10)
project(JniDemo)

set(CMAKE_CXX_STANDARD 17)

# JNI headers
include_directories("E:/Programs/JDK/include")
include_directories("E:/Programs/JDK/include/win32")

# Build the DLL
add_library(JniDemo SHARED Demo.cpp)

# Link required Windows libraries
if(WIN32)
    target_link_libraries(JniDemo PRIVATE jvm)
endif()
