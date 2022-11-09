#include <string>
#include <android/log.h>
#include "CgViewer.h"
#include "GlObj.h"

/* Md2モデル読込み(model読込,tex読込) */
bool CgViewer::LoadModel(std::map<std::string, Md2Model> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : md2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model load start (%s). %s %s(%d)", value.mName.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* MD2モデルLoad */
        bool ret = value.loadModel();
        std::vector<char>().swap(value.mWkMd2BinData);
        if( !ret) return false;
        /* テクスチャLoad */
        bool ret1 = value.loadTexture();
        std::vector<char>().swap(value.mWkTexBinData);
        if( !ret1) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

/* Md2モデル初期化(特にOpenGL系は、onSurfaceCreated()ドリブンで動作しないとエラーになる) */
bool CgViewer::InitModel(std::map<std::string, Md2Model> &md2models) {
    for(auto &[key, value] : md2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model Init start (%s). %s %s(%d)", value.mName.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* テクスチャInit */
        bool ret2 = value.initTexture();
        std::vector<char>().swap(value.mWkTexBinData);
        if( !ret2) return false;
        /* シェーダ初期化 */
        bool ret3 = value.initShaders();
        if( !ret3) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Shader Init end(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }
    return true;
}

/* Md2モデル描画 */
bool CgViewer::DrawModel(std::map<std::string, Md2Model> &md2models, const std::array<float, 16> &amNormalMat, float elapsedtimeMs) {
//  const std::array<float, 16> &amNormalMat = std::get<1>(globalSpacePrm);

	/* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
    GlObj::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /* glEnable(GL_DEPTH_TEST); */
    GlObj::enable(GL_DEPTH_TEST);

    for(auto &[key, value] : md2models) {
        value.drawModel(amNormalMat, elapsedtimeMs);
    }
    return true;
}

void CgViewer::SetRotate(std::map<std::string, Md2Model> &md2models, float x, float y) {
	for(auto &[key, value] : md2models) {
		value.setRotate(x, y);
	}
	return;
}

void CgViewer::SetScale(std::map<std::string, Md2Model> &md2models, float scale) {
	for(auto &[key, value] : md2models) {
		value.setScale(scale);
	}
}

