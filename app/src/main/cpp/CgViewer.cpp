#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <android/log.h>
#include "Md2Model.h"
#include "CgViewer.h"
#include "GlObj.h"
#include "GlobalSpacePrm.h"

static std::map<std::string, Md2Model> gMd2Models; /* Md2モデルデータ実体 */
static GlobalSpacePrm                  gGsPrm;     /* グローバル空間パラメータ */

/* Md2モデル読込み(model読込,tex読込) */
bool CgViewer::LoadModel(std::map<std::string, TmpBinData1> &tmpbindata1, const std::map<std::string, std::tuple<float, float, float, float, float, float, float, float, float>> &initPosition) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    gMd2Models.clear();

    for(auto &[key, bindata] : tmpbindata1) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model load start (%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

        Md2Model md2Model;
        /* MD2モデルLoad */
        bool ret = md2Model.loadModel(key, bindata.mWkMd2BinData);
        std::vector<char>().swap(bindata.mWkMd2BinData);
        if( !ret) return false;
        /* テクスチャLoad */
        bool ret2 = md2Model.loadTexture(key, bindata.mWkTexBinData);
        std::vector<char>().swap(bindata.mWkTexBinData);
        if( !ret2) return false;

        auto ip = initPosition.at(key);
        md2Model.setInitPosition({std::get<0>(ip),std::get<1>(ip),std::get<2>(ip)}, {std::get<3>(ip),std::get<4>(ip),std::get<5>(ip)}, {std::get<6>(ip),std::get<7>(ip),std::get<8>(ip)});

        /* Md2モデルデータ追加 */
        gMd2Models.emplace(key, md2Model);

        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

/* Md2モデル初期化(特にOpenGL系は、onSurfaceCreated()ドリブンで動作しないとエラーになる) */
bool CgViewer::InitModel(const std::map<std::string, TmpBinData3> &tmpbindata3s) {
    for(auto &[key, value] : gMd2Models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model Init start (%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* テクスチャInit */
        bool ret2 = value.initTexture(key);
        if( !ret2) return false;
        /* シェーダ初期化 */
        bool ret3 = value.initShaders(key, tmpbindata3s.at(key));
        if( !ret3) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Shader Init end(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }
    return true;
}

/* 描画エリア設定 */
void CgViewer::SetViewerArea(int width, int height) {
    gGsPrm.mProjectionMat = MatVec::getPerspectivef(30.0, ((float)width)/((float)height), 1.0, 5000.0);
    gGsPrm.mViewMat       = MatVec::getLookAtf(0.0f, 250.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    std::array<float, 16> vmmat = MatVec::multMatrixf(gGsPrm.mProjectionMat, gGsPrm.mViewMat);
    /* View投影行列の変更を通知 */
    CgViewer::SetVpMat(vmmat);
    return;
}

/* Md2モデル描画 */
bool CgViewer::DrawModel(float elapsedtimeMs) {
    for(auto &[key, value] : gMd2Models) {
        value.drawModel(gGsPrm.mNormalMat, elapsedtimeMs);
    }
    return true;
}

void CgViewer::SetVpMat(const std::array<float, 16> &vpmat) {
    for(auto &[key, value] : gMd2Models) {
        value.setVpMat(vpmat);
    }
    return;
}

void CgViewer::SetRotate(float rotatex, float rotatey) {
    for(auto &[key, value] : gMd2Models) {
        value.setRotate(rotatex, rotatey);
    }
    return;
}

void CgViewer::SetScale(float scale) {
    for(auto &[key, value] : gMd2Models) {
        value.setScale(scale);
    }
    return;
}

void CgViewer::SetPosition(const std::string &key, const std::array<float, 3> &pos) {
    gMd2Models.at(key).setPosition(pos[0], pos[1], pos[2]);
    return;
}
