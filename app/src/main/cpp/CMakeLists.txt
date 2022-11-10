cmake_minimum_required(VERSION 3.18.1)

project("cg3dviewer")

add_library( cg3dcom SHARED	CG3DCom.cpp
                            format/CG3D.cpp)
add_library( cg3dviewer SHARED	CG3DViewer.cpp
                                Jni.cpp
                                OpenGL/GLES2.cpp
                                )
add_library( mqo STATIC format/MatVec.cpp
                        format/MQO.cpp
                        )

find_library( log-lib log )
find_library( EGL_LIBRARY EGL )
find_library( GLES_LIBRARY GLESv2 )
find_library( ANDROID_LIBRARY android )

target_link_libraries( cg3dcom ${log-lib})
target_link_libraries( mqo cg3dcom )
target_link_libraries( cg3dviewer cg3dcom mqo ${log-lib} ${EGL_LIBRARY} ${GLES_LIBRARY} android)
