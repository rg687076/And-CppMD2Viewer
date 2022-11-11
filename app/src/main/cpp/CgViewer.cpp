#include <string>
#include <vector>
#include <map>
#include <android/log.h>
#include "Md2Model.h"
#include "CgViewer.h"

//static std::map<std::string, Md2Model> gMd2Models;       /* Md2モデルデータ実体 */

/* Md2モデル読込み(model読込,tex読込) */
bool CgViewer::LoadModel(std::map<std::string, TmpBinData> &tmpbindatas) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
//    gMd2Models.clear();

    for(auto &[key, bindata] : tmpbindatas) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model load start (%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
//        auto[itr, retbool] = gMd2Models.emplace(key, Md2Model());
//        if( !retbool) {
//            __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "ありえない!! 空のMd2Model追加に失敗!! (%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
//            return false;
//        }
//
//        /* MD2モデルLoad */
//        bool ret = (*itr).second.loadModel(bindata.mWkMd2BinData);
//        std::vector<char>().swap(bindata.mWkMd2BinData);
//        if( !ret) return false;
        /* テクスチャLoad */
//        bool ret1 = value.loadTexture();
//        std::vector<char>().swap(value.mWkTexBinData);
//        if( !ret1) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

