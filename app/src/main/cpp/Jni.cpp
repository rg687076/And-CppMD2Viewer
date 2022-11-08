#include <vector>
#include <string>
#include <map>
#include <mutex>
#include <chrono>
#include <tuple>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "Md2Obj.h"
#include "GlObj.h"
#include "GlobalSpaceObj.h"

#ifdef __cplusplus
extern "C" {
#endif

std::map<std::string, Md2Model>       gMd2Models;       /* Md2モデルデータ実体 */
std::mutex                            gMutex;           /* onStart()完了待ちmutex */
GlobalSpaceObj                        gGlobalSpacePrm;  /* グローバル空間パラメータ */
std::chrono::system_clock::time_point gPreStartTime;    /* 前回開始時刻 */

/* onStart */
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
        gMd2Models.emplace(modelnamechar, Md2Model{ .mName=modelnamechar,
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
    bool ret = Md2Obj::LoadModel(gMd2Models);
    if(!ret) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::loadModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    gMutex.unlock();
    return true;
}

/* onSurfaceCreated */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMutex.lock();  /* onStart()の実行終了を待つ */

    /* OpenGL初期化(GL系は、このタイミングでないとエラーになる) */
    GlObj::GlInit();

    /* GL系モデル初期化(GL系は、このタイミングでないとエラーになる) */
    bool ret = Md2Obj::InitModel(gMd2Models);
    if(!ret)
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::InitModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    std::array<float,  3> camerapos = {0.0f, 250.0f, 1000.0f};
    std::array<float,  3> targetpos = {0.0f, 0.0f, 0.0f};
    std::array<float,  3> uppos     = {0.0f, 1.0f, 0.0f};
    /* View行列を更新 */
    gGlobalSpacePrm.mCameraPos = camerapos;
    gGlobalSpacePrm.mTargetPos = targetpos;
    gGlobalSpacePrm.mUpPos     = uppos;
    gGlobalSpacePrm.mViewMat   = Mat44::getLookAtf(camerapos, targetpos, uppos);
    /* ViewProjection行列を更新 */
    gGlobalSpacePrm.mVpMat = Mat44::multMatrixf(gGlobalSpacePrm.mProjectionMat, gGlobalSpacePrm.mViewMat);
    /* View投影行列の変更を通知 */
    for(auto &[key, value] : gMd2Models)
        value.setVpMat(gGlobalSpacePrm.mVpMat);

    gMd2Models.at("grunt").setPosition(0.0f, 6.5f, -25.0f);

    gMutex.unlock();
    return;
}

/* onSurfaceChanged */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "w=%d h=%d %s %s(%d)", width, height, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* setViewport() */
    GlObj::setViewport(0, 0, width, height);

    /* 投影行列を更新 */
    gGlobalSpacePrm.mProjectionMat  = Mat44::getPerspectivef(30.0, ((float)width)/((float)height), 1.0, 5000.0);
    /* View投影行列を更新 */
    gGlobalSpacePrm.mVpMat  = Mat44::multMatrixf(gGlobalSpacePrm.mProjectionMat, gGlobalSpacePrm.mViewMat);
    /* View投影行列の変更を通知 */
    for(auto &[key, value] : gMd2Models)
        value.setVpMat(gGlobalSpacePrm.mVpMat);

    return;
}

/* onDrawFrame */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onDrawFrame(JNIEnv *env, jclass clazz) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* 前回描画からの経過時間を算出 */
    std::chrono::system_clock::time_point stime = std::chrono::system_clock::now();
    float elapsedtimeMs = (float)std::chrono::duration_cast<std::chrono::microseconds>(stime-gPreStartTime).count() / 1000.0f;
    gPreStartTime = stime;

    /* Md2モデル描画 */
    bool ret = Md2Obj::DrawModel(gMd2Models, gGlobalSpacePrm.mNormalMatrix, elapsedtimeMs);
    if(!ret) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::drawModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return;
    }

    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onStop(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

/* 注 : この処理は、OpenGLのスレッドでないので、OpenGL系の終了処理はできない。
 *      OpenGL系の終了処理は、GlSurfaceViewのdestory系処理で実行すべきだが、AndroidのGlSurfaceView::renderには
 *      その関数がない。GL系の終了処理は不要なんだと。
 *      それは、気持ち悪いので、今度実装する時は、GlSurfaceViewでなく、SurfaceViewで実装した方がいいかも...
 */
//    /* glDeleteTextures() */
//    const std::map<std::string, Md2Model> &md2models = gMd2Models;
//    const std::tuple<std::vector<unsigned int>, std::vector<unsigned int>> &ids = []() {
//                                                        std::vector<unsigned int> rettexids = {};
//                                                        std::vector<unsigned int> retprogids = {};
//                                                        rettexids.reserve(md2models.size());
//                                                        retprogids.reserve(md2models.size());
//                                                        for(auto &[key, value] : md2models) {
//                                                            rettexids.push_back(value.mTexId);
//                                                            retprogids.push_back(value.mProgramId);
//                                                        }
//                                                        return std::make_tuple(rettexids, retprogids);
//                                                    }();
//    /* テクスチャ解放 */
//    const std::vector<unsigned int> &texids = std::get<0>(ids);
//    GlObj::deleteTextures((GLsizei)texids.size(), texids.data());
//
//    /* シェーダ解放 */
//    const std::vector<unsigned int> &progids= std::get<1>(ids);
//    for(unsigned int progid : progids) {
//        GlObj::deleteProgram(progid);
//    }

    return;
}

/* モデルデータ位置設定 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setModelPosition(JNIEnv *env, jclass clazz, jstring modelnamejstr, jfloat x, jfloat y, jfloat z) {
    const char *modelnamechar = env->GetStringUTFChars(modelnamejstr, nullptr);

    std::map<std::string, Md2Model>::iterator itr = gMd2Models.find(modelnamechar);
    if(itr == gMd2Models.end()) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "warning!! 指定キャラクタ(=%s)は存在しない。 %s %s(%d)", modelnamechar, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return;
    }

    /* Global座標を設定 */
    (*itr).second.mPosition = {x, y, z};

    env->ReleaseStringUTFChars(modelnamejstr  , modelnamechar);
    env->DeleteLocalRef(modelnamejstr);

    return;
}

/* モデルデータ拡縮設定 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
    Md2Obj::SetScale(gMd2Models, scale);
    return;
}

/* モデルデータ回転設定 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setRotate(JNIEnv *env, jclass clazz, jfloat rotatex, jfloat rotatey) {
    Md2Obj::SetRotate(gMd2Models, rotatex, rotatey);
    return;
}

#ifdef __cplusplus
};
#endif
