#include <jni.h>
#include <map>
#include <vector>
#include <string>
#ifdef __ANDROID__
#include <android/log.h>
#include <android/asset_manager_jni.h>
#include "AppData.h"
#include "CG3DViewer.h"

#endif

#ifdef __cplusplus
extern "C" {
#endif

static std::tuple<bool, std::map<std::string, std::vector<char>>> LoadAssets(JNIEnv *pEnv, jobject *pAssetManager, std::vector<std::string> &assetFiles);
/**************/
/* CG3DViewer */
/**************/
JNIEXPORT jboolean JNICALL Java_com_tks_cppcgviewer_MainActivity_init(JNIEnv *env, jclass clazz, jobject assetmanager, jobjectArray objectArray) {
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

    return CG3DViewer::init();
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

/*******/
/* MQO */
/*******/
JNIEXPORT jboolean JNICALL Java_com_tks_cppcgviewer_modelformat_MQO_init(JNIEnv *env, jclass clazz) {
    std::map<std::string, std::vector<char>> &AssetDatas = AppData::GetIns().mAssets;
    GlRenderData &RenderData = GlRenderData::GetIns();

    auto [ret0, MqoInfo] = MQO::init(AssetDatas.at("vignette_ppp.mqo"));
    if(!ret0) return false;
    bool ret3 = MQO::remakeDrawInfo(MqoInfo, RenderData.mDrawInfos);
    if(!ret3) return false;
    bool ret6 = MQO::TextureInit(AssetDatas, MqoInfo.mMqoMaterials, RenderData.mDrawInfos);
    if(!ret6) return false;

    AppData::GetIns().mAssets.clear();
    return true;
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_modelformat_MQO_setDrawArea(JNIEnv *env, jclass clazz, jint width, jint height) {
	CG3DViewer::setDrawArea(width, height);
	return;
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_modelformat_MQO_draw(JNIEnv *env, jclass clazz) {
	CG3DViewer::draw();
	return;
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_modelformat_MQO_setTouchAngle(JNIEnv *env, jclass clazz, jfloat aTouchAngleX, jfloat aTouchAngleY) {
	CG3DViewer::setTouchAngle(aTouchAngleX, aTouchAngleY);
	return;
}

JNIEXPORT void JNICALL Java_com_tks_cppcgviewer_modelformat_MQO_setScale(JNIEnv *env, jclass clazz, jfloat scale) {
	CG3DViewer::setScale(scale);
	return;
}

#ifdef __cplusplus
};
#endif
