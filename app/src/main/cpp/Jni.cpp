#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "Md2Obj.h"
#include "GlObj.h"
#include "GlobalSpaceObj.h"
#include "MatObj.h"

#ifdef __cplusplus
extern "C" {
#endif

std::map<std::string, Md2ModelInfo> gMd2models;     /* Md2モデルデータ実体 */
std::mutex                          gMutex;         /* onStart()完了待ちmutex */
GlobalSpaceObj                      gGlobalSpaceObj;   /* onStart()完了待ちmutex */

JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                                                 jobjectArray modelnames, jobjectArray md2filenames, jobjectArray texfilenames,
                                                                 jobjectArray vshfilenames, jobjectArray fshfilenames) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMutex.lock();

    /* 引数チェック(md2model) */
    jsize size0 = env->GetArrayLength(modelnames), size1 = env->GetArrayLength(md2filenames), size2 = env->GetArrayLength(texfilenames), size3 = env->GetArrayLength(vshfilenames), size4 = env->GetArrayLength(fshfilenames);
    if(size0 != size1 || size0 != size2 || size0 != size3 || size0 != size4) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "引数不正 名称リストの数が合わない!!! modelname.size=%d md2filenames.size=%d texfilenames.size=%d vshfilenames.size=%d fshfilenames.size=%d %s %s(%d)", size0, size1, size2, size3, size4, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* AAssetManager取得 */
    AAssetManager *assetMgr = AAssetManager_fromJava(env, assets);
    if (assetMgr == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ERROR loading asset!!");
        return false;
    }

    /* モデル名,頂点ファイル名,Texファイル名,頂点ファイル中身,Texファイル中身,vshファイルの中身,fshのファイルの中身 取得 */
    for(int lpct = 0; lpct < size0; lpct++) {
        /* jobjectArray -> jstring */
        jstring modelnamejstr   = (jstring)env->GetObjectArrayElement(modelnames  , lpct);
        jstring md2filenamejstr = (jstring)env->GetObjectArrayElement(md2filenames, lpct);
        jstring texfilenamejstr = (jstring)env->GetObjectArrayElement(texfilenames, lpct);
        jstring vshfilenamejstr = (jstring)env->GetObjectArrayElement(vshfilenames, lpct);
        jstring fshfilenamejstr = (jstring)env->GetObjectArrayElement(fshfilenames, lpct);

        /* jstring -> char */
        const char *modelnamechar   = env->GetStringUTFChars(modelnamejstr  , nullptr);
        const char *md2filenamechar = env->GetStringUTFChars(md2filenamejstr, nullptr);
        const char *texfilenamechar = env->GetStringUTFChars(texfilenamejstr, nullptr);
        const char *vshfilenamechar = env->GetStringUTFChars(vshfilenamejstr, nullptr);
        const char *fshfilenamechar = env->GetStringUTFChars(fshfilenamejstr, nullptr);

        /* md2関連データ一括読込み */
        std::vector<std::pair<std::string, std::vector<char>>> wk = {{md2filenamechar, std::vector<char>()},
                                                                     {texfilenamechar, std::vector<char>()},
                                                                     {vshfilenamechar, std::vector<char>()},
                                                                     {fshfilenamechar, std::vector<char>()}};
        for(std::pair<std::string, std::vector<char>> &item : wk) {
            const std::string &filename = item.first;
            std::vector<char> &binbuf = item.second;

            /* AAsset::open */
            AAsset *assetFile = AAssetManager_open(assetMgr, filename.c_str(), AASSET_MODE_RANDOM);
            if (assetFile == nullptr) {
                __android_log_print(ANDROID_LOG_INFO, "aaaaa", "ERROR AAssetManager_open(%s) %s %s(%d)", filename.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
                return false;
            }
            /* 読込 */
            int size = AAsset_getLength(assetFile);
            binbuf.resize(size);
            AAsset_read(assetFile, binbuf.data(), size);
            /* AAsset::close */
            AAsset_close(assetFile);
        }

        /* Md2model追加 */
        gMd2models.emplace(modelnamechar, Md2ModelInfo{ .mName=modelnamechar,
                                                        .mWkMd2BinData=std::move(wk[0].second),
                                                        .mWkTexBinData=std::move(wk[1].second),
                                                        /* shaderはデータを文字列に変換して格納 */
                                                        .mWkVshStrData=std::string(wk[2].second.begin(), wk[2].second.end()),
                                                        .mWkFshStrData=std::string(wk[3].second.begin(), wk[3].second.end())});

        /* char解放 */
        env->ReleaseStringUTFChars(modelnamejstr  , modelnamechar);
        env->ReleaseStringUTFChars(md2filenamejstr, md2filenamechar);
        env->ReleaseStringUTFChars(texfilenamejstr, texfilenamechar);
        env->ReleaseStringUTFChars(vshfilenamejstr, vshfilenamechar);
        env->ReleaseStringUTFChars(fshfilenamejstr, fshfilenamechar);

        /* jstring解放 */
        env->DeleteLocalRef(modelnamejstr);
        env->DeleteLocalRef(md2filenamejstr);
        env->DeleteLocalRef(texfilenamejstr);
        env->DeleteLocalRef(vshfilenamejstr);
        env->DeleteLocalRef(fshfilenamejstr);
    }

    /* 初期化 */
    bool ret = Md2Obj::LoadModel(gMd2models);
    if(!ret) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::LoadModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    gMutex.unlock();
    return true;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMutex.lock();  /* onStart()の実行終了を待つ */

    /* OpenGL初期化(GL系は、このタイミングでないとエラーになる) */
    GlObj::GlInit();

    /* GL系モデル初期化(GL系は、このタイミングでないとエラーになる) */
    bool ret = Md2Obj::InitModel(gMd2models);
    if(!ret)
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::InitModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* View行列を設定 */
    std::array<float,  3> camerapos = {0.0f, 250.0f, 1000.0f};
    std::array<float,  3> targetpos = {0.0f, 0.0f, 0.0f};
    std::array<float,  3> uppos     = {0.0f, 1.0f, 0.0f};
    gGlobalSpaceObj.mCameraPos = camerapos;
    gGlobalSpaceObj.mTargetPos = targetpos;
    gGlobalSpaceObj.mUpPos     = uppos;
    gGlobalSpaceObj.mViewMat   = Mat44::getLookAtf(camerapos, targetpos, uppos);
    /* ビュー行列を変更したので再計算 */
    gGlobalSpaceObj.mVpMat = Mat44::multMatrixf(gGlobalSpaceObj.mProjectionMat, gGlobalSpaceObj.mViewMat);

    gMutex.unlock();
    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "w=%d h=%d %s %s(%d)", width, height, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    gGlobalSpaceObj.mProjectionMat = Mat44::getPerspectivef(30.0, ((float)width)/((float)height), 1.0, 5000.0);
    /* 投影行列を変更したので再計算 */
    gGlobalSpaceObj.mVpMat = Mat44::multMatrixf(gGlobalSpaceObj.mProjectionMat, gGlobalSpaceObj.mViewMat);

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

/* モデルデータ移動 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setModelPosition(JNIEnv *env, jclass clazz, jstring modelnamejstr, jfloat x, jfloat y, jfloat z) {
    const char *modelnamechar = env->GetStringUTFChars(modelnamejstr, nullptr);

    std::map<std::string, Md2ModelInfo>::iterator itr = gMd2models.find(modelnamechar);
    if(itr == gMd2models.end()) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "warning!! 指定キャラクタ(=%s)は存在しない。 %s %s(%d)", modelnamechar, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return;
    }

    /* Global座標を設定 */
    (*itr).second.mPosition = {x, y, z};

    env->ReleaseStringUTFChars(modelnamejstr  , modelnamechar);
    env->DeleteLocalRef(modelnamejstr);

    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
    gGlobalSpaceObj.mScale = scale;

    /* モデル行列を算出 */
    std::array<float, 16> rotetematx = Mat44::getRotatef(-gGlobalSpaceObj.mRotatex, 1.0f, 0.0f, 0.0f);
    std::array<float, 16> rotetematy = Mat44::getRotatef( gGlobalSpaceObj.mRotatey, 0.0f, 1.0f, 0.0f);
    std::array<float, 16> rotetemat = Mat44::multMatrixf(rotetematx, rotetematy);
    std::array<float, 16> translatemat = Mat44::translatef(rotetemat, {0.0f, -150.0f, 0.0f});
    gGlobalSpaceObj.mModelMat = Mat44::scalef(translatemat, {gGlobalSpaceObj.mScale, gGlobalSpaceObj.mScale, gGlobalSpaceObj.mScale});

    /* 正規行列を算出 */
    std::array<float, 16> invertMat = Mat44::invertf(gGlobalSpaceObj.mModelMat);
    gGlobalSpaceObj.mNormalMatrix = Mat44::transposef(invertMat);

    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setRotate(JNIEnv *env, jclass clazz, jfloat rotatex, jfloat rotatey) {
    gGlobalSpaceObj.mRotatex = rotatex;
    gGlobalSpaceObj.mRotatey = rotatey;

    /* モデル行列を算出 */
    std::array<float, 16> rotetematx = Mat44::getRotatef(-gGlobalSpaceObj.mRotatex, 1.0f, 0.0f, 0.0f);
    std::array<float, 16> rotetematy = Mat44::getRotatef( gGlobalSpaceObj.mRotatey, 0.0f, 1.0f, 0.0f);
    std::array<float, 16> rotetemat = Mat44::multMatrixf(rotetematx, rotetematy);
    std::array<float, 16> translatemat = Mat44::translatef(rotetemat, {0.0f, -150.0f, 0.0f});
    gGlobalSpaceObj.mModelMat = Mat44::scalef(translatemat, {gGlobalSpaceObj.mScale, gGlobalSpaceObj.mScale, gGlobalSpaceObj.mScale});

    /* 正規行列を算出 */
    std::array<float, 16> invertMat = Mat44::invertf(gGlobalSpaceObj.mModelMat);
    gGlobalSpaceObj.mNormalMatrix = Mat44::transposef(invertMat);

    return;
}

#ifdef __cplusplus
};
#endif
