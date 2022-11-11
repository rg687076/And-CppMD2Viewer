#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_tks_cppmd2viewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                      jobjectArray modelnames, jobjectArray md2filenames,
                                      jobjectArray texfilenames, jobjectArray vshfilenames,
                                      jobjectArray fshfilenames) {
    // TODO: implement onStart()
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    // TODO: implement onSurfaceCreated()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    // TODO: implement onSurfaceChanged()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tks_cppmd2viewer_Jni_onDrawFrame(JNIEnv *env, jclass clazz) {
    // TODO: implement onDrawFrame()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tks_cppmd2viewer_Jni_setTouchAngle(JNIEnv *env, jclass clazz, jfloat a_touch_angle_x,
                                            jfloat a_touch_angle_y) {
    // TODO: implement setTouchAngle()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_tks_cppmd2viewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat m_scale) {
    // TODO: implement setScale()
}