cmake_minimum_required(VERSION 3.18.1)

project("cppmd2viewer")

add_library(jni SHARED Jni.cpp)
add_library(md2model SHARED Md2Obj.cpp TexObj.cpp)
add_library(globj SHARED GlObj.cpp)

find_library(log-lib log)

target_link_libraries(jni ${log-lib} android md2model globj)
target_link_libraries(md2model ${log-lib} globj)
target_link_libraries(globj ${log-lib} EGL GLESv2)
