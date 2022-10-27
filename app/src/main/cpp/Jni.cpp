#include <vector>
#include <string>
#include <map>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "Md2Model.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                                                 jobjectArray modelnames, jobjectArray vertexnames, jobjectArray texnames) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* 引数チェック */
    jsize size0 = env->GetArrayLength(modelnames), size1 = env->GetArrayLength(vertexnames), size2 = env->GetArrayLength(texnames);
    if(size0 != size1 || size0 != size2) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "引数不正 名称リスト数が合わない!!! modelname.size=%d vertexnames.size=%d texnames.size=%d %s %s(%d)", size0, size1, size2, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* AAssetManager取得 */
    AAssetManager *assetMgr = AAssetManager_fromJava(env, assets);
    if (assetMgr == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR loading asset!!");
        return false;
    }
    /* モデル名,頂点ファイル名,Texファイル名,頂点ファイル中身,Texファイル中身 取得 */
    for(int lpct = 0; lpct < size0; lpct++) {
        /* jobjectArray -> jstring */
        jstring modelfilenamejstr= (jstring)env->GetObjectArrayElement(modelnames, lpct);
        jstring verfilenamejstr  = (jstring)env->GetObjectArrayElement(vertexnames, lpct);
        jstring texfilenamejstr  = (jstring)env->GetObjectArrayElement(texnames, lpct);

        /* jstring -> char */
        const char *modelfilenamechar= env->GetStringUTFChars(modelfilenamejstr, nullptr);
        const char *verfilenamechar  = env->GetStringUTFChars(verfilenamejstr, nullptr);
        const char *texfilenamechar  = env->GetStringUTFChars(texfilenamejstr, nullptr);

        /* md2データ一括読込み */
        /* AAsset::open */
        AAsset *verAssetFile = AAssetManager_open(assetMgr, verfilenamechar, AASSET_MODE_RANDOM);
        if (verAssetFile == nullptr) {
            __android_log_print(ANDROID_LOG_INFO, "aaaaa", "ERROR AAssetManager_open(%s) %s %s(%d)", verfilenamechar, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
            return false;
        }
        /* 読込 */
        int versize = AAsset_getLength(verAssetFile);
        std::vector<char> verbuf(versize);
        AAsset_read(verAssetFile, verbuf.data(), versize);
        /* AAsset::close */
        AAsset_close(verAssetFile);

        /* Texデータ一括読込み */
        /* AAsset::open */
        AAsset *texAssetFile = AAssetManager_open(assetMgr, texfilenamechar, AASSET_MODE_RANDOM);
        if (texAssetFile == nullptr) {
            __android_log_print(ANDROID_LOG_INFO, "aaaaa", "ERROR AAssetManager_open(%s) %s %s(%d)", texfilenamechar, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
            return false;
        }
        /* 読込 */
        int texsize = AAsset_getLength(texAssetFile);
        std::vector<char> texbuf(texsize);
        AAsset_read(texAssetFile, texbuf.data(), texsize);
        /* AAsset::close */
        AAsset_close(texAssetFile);

        /* Md2model追加 */
        gMd2models.emplace(modelfilenamechar, Md2Model{ .name=modelfilenamechar, .verfilename=verfilenamechar, .texfilename=texfilenamechar,
                                                       .verbindata=std::move(verbuf), .texbindata=std::move(texbuf)});
        /* char解放 */
        env->ReleaseStringUTFChars(modelfilenamejstr, modelfilenamechar);
        env->ReleaseStringUTFChars(verfilenamejstr, verfilenamechar);
        env->ReleaseStringUTFChars(texfilenamejstr, texfilenamechar);

        /* jstring解放 */
        env->DeleteLocalRef(modelfilenamejstr);
        env->DeleteLocalRef(verfilenamejstr);
        env->DeleteLocalRef(texfilenamejstr);
    }

    /* Md2modelロード */
    Md2Setup(std::move(gMd2models));

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

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onStop(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    return;
}

#ifdef __cplusplus
};
#endif
