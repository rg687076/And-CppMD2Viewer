#include <vector>
#include <string>
#include <map>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "AssetsData.h"

#ifdef __cplusplus
extern "C" {
#endif

/* プロトタイプ宣言 */
static std::tuple<bool, std::map<std::string, std::vector<char>>> LoadAssets(JNIEnv *pEnv, jobject *pAssetManager, std::vector<std::string> &assetFiles);

JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onCreate(JNIEnv *env, jclass clazz, jobject assetManager, jobjectArray assetFileListobjary) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    std::vector<std::string> assetfilelist;
    jsize arraysize = env->GetArrayLength(assetFileListobjary);
    for(int lpct = 0; lpct < arraysize; lpct++) {
        jstring filenamejstr = (jstring)env->GetObjectArrayElement(assetFileListobjary, lpct);
        const char *filenamechar = env->GetStringUTFChars(filenamejstr, 0);
        assetfilelist.push_back(std::string(filenamechar));
        env->ReleaseStringUTFChars(filenamejstr, filenamechar);
        env->DeleteLocalRef(filenamejstr);
    }
    auto [ret0, AssetDatas] = LoadAssets(env, &assetManager, assetfilelist);
    if(!ret0) return false;
    AssetsData::GetIns().mAssets = std::move(AssetDatas);

    return true;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint w, jint h) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "w=%d h=%d %s %s(%d)", w, h,__PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onDrawFrame(JNIEnv *env, jclass clazz) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onDestroy(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    return;
}

static std::tuple<bool, std::map<std::string, std::vector<char>>> LoadAssets(JNIEnv *pEnv, jobject *pAssetManager, std::vector<std::string> &assetFiles) {
    /* AAssetManager取得 */
    AAssetManager *assetMgr = AAssetManager_fromJava(pEnv, *pAssetManager);
    if (assetMgr == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR loading asset!!");
        return {false, {} };
    }

    std::map<std::string, std::vector<char>> retMap;

    /* AAsset::open -> 読込 -> close */
    std::for_each(assetFiles.begin(), assetFiles.end(), [&assetMgr, &retMap](std::string f){
        /* AAsset::open */
        AAsset *assetFile = AAssetManager_open(assetMgr, f.c_str(), AASSET_MODE_RANDOM);
        if (assetFile == nullptr) {
            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR AAssetManager_open(%s)\n", f.c_str());
            return;
        }
        /* 読込 */
        int size = AAsset_getLength(assetFile);
        std::vector<char> buf(size);
        AAsset_read(assetFile, buf.data(), size);
        /* AAsset::close */
        AAsset_close(assetFile);
        retMap.insert({f, buf});
    });

    return {true, retMap};
}

#ifdef __cplusplus
};
#endif
