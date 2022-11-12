#include <string>
#include <vector>
#include <map>
#include <android/log.h>
#include "Md2Model.h"
#include "CgViewer.h"

static std::map<std::string, Md2Model> gMd2Models;       /* Md2モデルデータ実体 */

/* Md2モデル読込み(model読込,tex読込) */
bool CgViewer::LoadModel(std::map<std::string, TmpBinData1> &tmpbindata1) {
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

        /* Md2モデルデータ追加 */
        gMd2Models.emplace(key, md2Model);

        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

