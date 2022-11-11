cmake_minimum_required(VERSION 3.18.1)

project("cppmd2viewer")

add_library(jni SHARED Jni.cpp)
add_library(md2viewer SHARED CgViewer.cpp Md2Model.cpp)
add_library(cg3dviewer SHARED CG3DViewer.cpp)

find_library(log-lib log)

target_link_libraries(jni ${log-lib} android md2viewer cg3dviewer)
target_link_libraries(md2viewer ${log-lib})
