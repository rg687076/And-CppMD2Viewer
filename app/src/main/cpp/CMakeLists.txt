cmake_minimum_required(VERSION 3.18.1)

project("cppmd2viewer")

add_library(jni SHARED Jni.cpp)
add_library(cppmd2viewer SHARED AssetsData.cpp)

find_library(log-lib log)

target_link_libraries(cppmd2viewer ${log-lib} android)
target_link_libraries(jni ${log-lib} cppmd2viewer)
