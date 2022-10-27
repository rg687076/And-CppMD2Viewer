#include <iostream>
#include <string>
#include <sstream>
#include <android/log.h>
#include "Md2Obj.h"

/* Md2モデルデータ実体 */
std::map<std::string, Md2ModelInfo> gMd2models;

/* Md2モデルsetup */
bool Md2Init(std::map<std::string, Md2ModelInfo> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : gMd2models) {
        bool ret = value.loadModel(value.verbindata, value.texbindata, 180, 30);
        if(ret == false) return false;
    }

    return true;
}

bool Md2ModelInfo::loadModel(std::vector<char> &Md2file, std::vector<char> &Tgafile, float _size, float _fps) {
    md2_t header = {0};
    /* MD2ファイルのパース */
    std::istringstream streaaam(std::string(Md2file.begin(), Md2file.end()));
    streaaam.read((char*)&header, sizeof(md2_t));

    /* MD2形式チェック() */
    if(header.ident != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        streaaam.str("");
        streaaam.clear(std::stringstream::goodbit);
        const union { int i; char b[4]; } ngno = {header.ident};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(majicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    if(header.version != MD2_VERSION) { /* 8じゃないとエラー */
        streaaam.str("");
        streaaam.clear(std::stringstream::goodbit);
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(version=%s) %s %s(%d)", header.version, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    return true;
}
