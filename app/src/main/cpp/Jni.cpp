#include <string>
#include <map>
#include <mutex>
#include <chrono>
#include <tuple>
#include <jni.h>
#ifdef __ANDROID__
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "AppData.h"
#include "CG3DViewer.h"
#endif  // __ANDROID__
#include "Md2Model.h"
#include "CgViewer.h"
#include "GlObj.h"

#ifdef __cplusplus
extern "C" {
#endif

static std::mutex                            gMutex;        /* onStart()完了待ちmutex */
static std::chrono::system_clock::time_point gPreStartTime; /* 前回開始時刻 */
static std::map<std::string, TmpBinData3>    gTmpBinData3s; /* Gl初期化用のTexデータ */

/**************/
/* CG3DViewer */
/**************/
/* プロトタイプ宣言 */
static std::tuple<bool, std::map<std::string, std::vector<char>>> LoadAssets(JNIEnv *pEnv, jobject *pAssetManager, std::vector<std::string> &assetFiles);

/* MQO初期化 */
JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_init(JNIEnv *env, jclass clazz, jobject assetmanager, jobjectArray objectArray) {
    std::vector<std::string> assetfiles;
    jsize arraysize = env->GetArrayLength(objectArray);
    for(int lpct = 0; lpct < arraysize; lpct++) {
        jstring filenamejstr = (jstring)env->GetObjectArrayElement(objectArray, lpct);
        const char *filenamechar = env->GetStringUTFChars(filenamejstr, 0);
        assetfiles.push_back(std::string(filenamechar));
        env->ReleaseStringUTFChars(filenamejstr, filenamechar);
        env->DeleteLocalRef(filenamejstr);
    }
    auto [ret0, AssetDatas] = LoadAssets(env, &assetmanager, assetfiles);
    if(!ret0) return false;
    AppData::GetIns().mAssets = std::move(AssetDatas);

    return true;
}

/* Assets読込み */
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

/*******/
/* Jni */
/*******/
/* onStart */
JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onStart(JNIEnv *env, jclass clazz, jobject assets,
                                                                 jobjectArray modelnames,
                                                                 jobjectArray md2filenames, jobjectArray texfilenames,
                                                                 jobjectArray vshfilenames, jobjectArray fshfilenames,
                                                                 jfloatArray scalexs, jfloatArray scaleys, jfloatArray scalezs,
                                                                 jfloatArray rotxs  , jfloatArray rotys  , jfloatArray rotzs,
                                                                 jfloatArray transxs, jfloatArray transys, jfloatArray transzs) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMutex.lock();

    /* 引数チェック(md2model) */
    jsize size0 = env->GetArrayLength(modelnames), size1 = env->GetArrayLength(md2filenames), size2 = env->GetArrayLength(texfilenames), size3 = env->GetArrayLength(vshfilenames), size4 = env->GetArrayLength(fshfilenames);
    jsize size5 = env->GetArrayLength(scalexs), size6 = env->GetArrayLength(scaleys), size7 = env->GetArrayLength(scalezs), size8 = env->GetArrayLength(rotxs), size9 = env->GetArrayLength(rotys), size10 = env->GetArrayLength(rotzs), size11 = env->GetArrayLength(transxs), size12 = env->GetArrayLength(transys), size13 = env->GetArrayLength(transzs);
    if(size0 != size1 || size0 != size2 || size0 != size3 || size0 != size4 || size0 != size5 || size0 != size6 || size0 != size7 || size0 != size8 || size0 != size9 || size0 != size10 || size0 != size11 || size0 != size12 || size0 != size13) {
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
    jfloat *pscalex = env->GetFloatArrayElements(scalexs, nullptr);
    jfloat *pscaley = env->GetFloatArrayElements(scaleys, nullptr);
    jfloat *pscalez = env->GetFloatArrayElements(scalezs, nullptr);
    jfloat *protx   = env->GetFloatArrayElements(rotxs, nullptr);
    jfloat *proty   = env->GetFloatArrayElements(rotys, nullptr);
    jfloat *protz   = env->GetFloatArrayElements(rotzs, nullptr);
    jfloat *ptransx = env->GetFloatArrayElements(transxs, nullptr);
    jfloat *ptransy = env->GetFloatArrayElements(transys, nullptr);
    jfloat *ptransz = env->GetFloatArrayElements(transzs, nullptr);
    std::map<std::string, std::tuple<float,float,float, float,float,float, float,float,float>> initposison = {};
    std::map<std::string, TmpBinData1> tmpbindata1s = {};
    for(int lpct = 0; lpct < size0; lpct++) {
        /* jobjectArray -> jstring */
        jstring modelnamejstr   = (jstring)env->GetObjectArrayElement(modelnames  , lpct);
        jstring md2filenamejstr = (jstring)env->GetObjectArrayElement(md2filenames, lpct);
        jstring texfilenamejstr = (jstring)env->GetObjectArrayElement(texfilenames, lpct);
        jstring vshfilenamejstr = (jstring)env->GetObjectArrayElement(vshfilenames, lpct);
        jstring fshfilenamejstr = (jstring)env->GetObjectArrayElement(fshfilenames, lpct);
        float scalex    = pscalex[lpct];
        float scaley    = pscaley[lpct];
        float scalez    = pscalez[lpct];
        float rotx      = protx[lpct];
        float roty      = proty[lpct];
        float rotz      = protz[lpct];
        float translatex= ptransx[lpct];
        float translatey= ptransy[lpct];
        float translatez= ptransz[lpct];
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "初期位置[%d] scale(%f,%f,%f) rot(%f,%f,%f) translate(%f,%f,%f) %s %s(%d)", lpct,
                            scalex,scaley,scalez, rotx,roty,rotz, translatex,translatey,translatez, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

        /* jstring -> char */
        const char *modelnamechar   = env->GetStringUTFChars(modelnamejstr  , nullptr);
        const char *md2filenamechar = env->GetStringUTFChars(md2filenamejstr, nullptr);
        const char *texfilenamechar = env->GetStringUTFChars(texfilenamejstr, nullptr);
        const char *vshfilenamechar = env->GetStringUTFChars(vshfilenamejstr, nullptr);
        const char *fshfilenamechar = env->GetStringUTFChars(fshfilenamejstr, nullptr);

        /* md2初期位置データ設定 */
        initposison.emplace(modelnamechar, std::tuple{scalex,scaley,scalez, rotx,roty,rotz, translatex,translatey,translatez});

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

        /* ファイル一括読込み(tmpbindata2は、tmpbindata1の戻りで生成されるのでここでは作らない) */
        tmpbindata1s.emplace(modelnamechar, TmpBinData1{.mName=modelnamechar,
                                                       .mWkMd2BinData=std::move(wk[0].second),
                                                       .mWkTexBinData=std::move(wk[1].second)});
        gTmpBinData3s.emplace(modelnamechar, TmpBinData3{.mName=modelnamechar,
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
    env->ReleaseFloatArrayElements(scalexs, pscalex, 0);
    env->ReleaseFloatArrayElements(scaleys, pscaley, 0);
    env->ReleaseFloatArrayElements(scalezs, pscalez, 0);
    env->ReleaseFloatArrayElements(rotxs  , protx  , 0);
    env->ReleaseFloatArrayElements(rotys  , proty  , 0);
    env->ReleaseFloatArrayElements(rotzs  , protz  , 0);
    env->ReleaseFloatArrayElements(transxs, ptransx, 0);
    env->ReleaseFloatArrayElements(transys, ptransy, 0);
    env->ReleaseFloatArrayElements(transzs, ptransz, 0);

    /* 初期化 */
//    bool ret = CgViewer::LoadModel(tmpbindata1s);
    bool ret = CgViewer::LoadModel(tmpbindata1s, initposison);
    tmpbindata1s.clear();
    initposison.clear();
    if(!ret) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::loadModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    gMutex.unlock();
    return true;
}

/* onSurfaceCreated */
JNIEXPORT jboolean JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceCreated(JNIEnv *env, jclass clazz) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMutex.lock();  /* onStart()の実行終了を待つ */

    /* OpenGL初期化(GL系は、このタイミングでないとエラーになる) */
    GlObj::GlInit();

    /* MQO */
    GlObj::enable(GL_DEPTH_TEST);
    bool ret1 = CG3DViewer::init();
    if( !ret1) return false;
    std::map<std::string, std::vector<char>> &AssetDatas = AppData::GetIns().mAssets;
    GlRenderData &RenderData = GlRenderData::GetIns();
    auto [ret0, MqoInfo] = MQO::init(AssetDatas.at("vignette_ppp.mqo"));
    if(!ret0) return false;
    bool ret3 = MQO::remakeDrawInfo(MqoInfo, RenderData.mDrawInfos);
    if(!ret3) return false;
    bool ret6 = MQO::TextureInit(AssetDatas, MqoInfo.mMqoMaterials, RenderData.mDrawInfos);
    if(!ret6) return false;
    AppData::GetIns().mAssets.clear();

    /* MD2 */
    /* GL系モデル初期化(GL系は、このタイミングでないとエラーになる) */
    bool ret = CgViewer::InitModel(gTmpBinData3s);
    gTmpBinData3s.clear();
    if(!ret)
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::InitModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* 初期位置設定 */
    CgViewer::SetPosition("grunt", {100.0f, 0.0f, 0.0f});

    gMutex.unlock();
    return true;
}

/* onSurfaceChanged */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onSurfaceChanged(JNIEnv *env, jclass clazz, jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "w=%d h=%d %s %s(%d)", width, height, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* setViewport() */
    GlObj::setViewport(0, 0, width, height);

    CG3DViewer::setDrawArea(width, height);

    CgViewer::SetViewerArea(width, height);

    return;
}

/* onDrawFrame */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_onDrawFrame(JNIEnv *env, jclass clazz) {
//    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    /* 前回描画からの経過時間を算出 */
    std::chrono::system_clock::time_point stime = std::chrono::system_clock::now();
    float elapsedtimeMs = (float)std::chrono::duration_cast<std::chrono::microseconds>(stime-gPreStartTime).count() / 1000.0f;
    gPreStartTime = stime;

    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
    GlObj::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* glEnable(GL_DEPTH_TEST); */
    GlObj::enable(GL_DEPTH_TEST);

    /* MQOモデル描画 */
	CG3DViewer::draw();

    /* Md2モデル描画 */
    bool ret = CgViewer::DrawModel(elapsedtimeMs);
    if(!ret) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Obj::drawModel()で失敗!! %s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return;
    }

    return;
}

JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setTouchAngle(JNIEnv *env, jclass clazz, jfloat aTouchAngleX, jfloat aTouchAngleY) {
	CG3DViewer::setTouchAngle(aTouchAngleX, aTouchAngleY);
    CgViewer::SetRotate(aTouchAngleX, aTouchAngleY);
	return;
}

/* モデルデータ拡縮設定 */
JNIEXPORT void JNICALL Java_com_tks_cppmd2viewer_Jni_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
	CG3DViewer::setScale(scale);
    CgViewer::SetScale(scale);
	return;
}

#ifdef __cplusplus
};
#endif
