#include <jni.h>
#include <string>

#ifdef __ANDROID__
#include <android/log.h>
#include <android/asset_manager_jni.h>
#endif  // __ANDROID__

#ifdef __cplusplus
extern "C" {
#endif


/*******/
/* Jni */
/*******/
/* onStart */
JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                      jobjectArray modelnames,
                                      jobjectArray md2filenames, jobjectArray texfilenames,
                                      jobjectArray vshfilenames, jobjectArray fshfilenames) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

}

/* onSurfaceCreated */
JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    // TODO: implement onSurfaceCreated()
}

/* onSurfaceChanged */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    // TODO: implement onSurfaceChanged()
}

/* onDrawFrame */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onDrawFrame(JNIEnv *env, jclass clazz) {
    // TODO: implement onDrawFrame()
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setTouchAngle(JNIEnv *env, jclass clazz, jfloat aTouchAngleX, jfloat aTouchAngleY) {
    // TODO: implement setTouchAngle()
}

/* モデルデータ拡縮設定 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
    // TODO: implement setScale()
}

#ifdef __cplusplus
};
#endif
