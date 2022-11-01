#include <iostream>
#include <string>
#include <sstream>
#include <android/log.h>
#include "Md2Parts.h"
#include "Md2Obj.h"
#include "TexObj.h"

/* Md2モデルデータ実体 */
std::map<std::string, Md2ModelInfo> gMd2models;

/* Md2モデル初期化(model読込,tex読込) */
bool Md2Obj::Init(std::map<std::string, Md2ModelInfo> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : gMd2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model losd start (%s). %s %s(%d)", value.mName.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* MD2モデルLoad */
        bool ret = value.LoadModel();
        std::vector<char>().swap(value.mWkMd2BinData);
        if( !ret) return false;
        /* テクスチャLoad */
        bool ret2 = value.LoadTexture();
        std::vector<char>().swap(value.mWkTexBinData);
        if( !ret2) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* シェーダ初期化 */
        bool ret3 = value.LoadShaders();
        if( !ret3) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Shader INITIALIZED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

Md2ModelInfo::~Md2ModelInfo() {
    std::vector<char>().swap(mWkMd2BinData);
    std::vector<char>().swap(mWkTexBinData);
}

bool Md2ModelInfo::LoadModel() {
//    std::istringstream md2binstream(std::string(mWkMd2BinData.begin(), mWkMd2BinData.end()));
//    md2binstream.read((char*)&header, sizeof(md2header));
//    md2binstream.str("");
//    md2binstream.clear(std::stringstream::goodbit);

    /* MD2ヘッダ */
    md2header *header = (md2header*)mWkMd2BinData.data();

    /* MD2形式チェック */
    if(header->magicnumber != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        std::vector<char>().swap(mWkMd2BinData);
        const union { int i; char b[4]; } ngno = {header->magicnumber};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(magicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    if(header->version != MD2_VERSION) { /* 8じゃないとエラー */
        std::vector<char>().swap(mWkMd2BinData);
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(version=%d) %s %s(%d)", header->version, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* 初期化 */
    mMdlData.numVertexsPerFrame= header->num_vertexs;
    mMdlData.numTotalFrames    = header->num_totalframes;

    /* 頂点読込み */
    mMdlData.vertexList.resize(header->num_vertexs * header->num_totalframes);
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "1frame当りの頂点数(%d)と総フレーム数(%d) %s %s(%d)", header->num_vertexs, header->num_totalframes, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    for(size_t lpct = 0; lpct < header->num_totalframes; lpct++) {
        frame *lframe = (frame*)&(mWkMd2BinData[header->offset_frames + header->framesize * lpct]);
        vertex *pvertex = &mMdlData.vertexList[header->num_vertexs * lpct];
        for (size_t lpct2 = 0; lpct2 < header->num_vertexs; lpct2++) {
            pvertex[lpct2].v[0] = lframe->scale[0] * lframe->fp[lpct2].v[0] + lframe->translate[0];
            pvertex[lpct2].v[1] = lframe->scale[1] * lframe->fp[lpct2].v[1] + lframe->translate[1];
            pvertex[lpct2].v[2] = lframe->scale[2] * lframe->fp[lpct2].v[2] + lframe->translate[2];
        }
    }

    /* uvデータ読込み */
    mMdlData.st.resize(header->num_st);
    texindex *sts = (texindex*)&mWkMd2BinData[header->offset_st];
    for (size_t lpct = 0; lpct < header->num_st; lpct++) {
        mMdlData.st[lpct].s = static_cast<float>(sts[lpct].s) / static_cast<float>(header->skinwidth);
        mMdlData.st[lpct].t = static_cast<float>(sts[lpct].t) / static_cast<float>(header->skinheight);
    }

    /* mesh情報読込み */
    mMdlData.polyIndex.resize(header->num_polys);
    mMdlData.numPolys = header->num_polys;
    mesh *polyIndex = (mesh*)&mWkMd2BinData[header->offset_meshs];
    for (size_t lpct = 0; lpct < header->num_polys; lpct++) {
        mMdlData.polyIndex[lpct].meshIndex[0] = polyIndex[lpct].meshIndex[0];
        mMdlData.polyIndex[lpct].meshIndex[1] = polyIndex[lpct].meshIndex[1];
        mMdlData.polyIndex[lpct].meshIndex[2] = polyIndex[lpct].meshIndex[2];

        mMdlData.polyIndex[lpct].stIndex[0] = polyIndex[lpct].stIndex[0];
        mMdlData.polyIndex[lpct].stIndex[1] = polyIndex[lpct].stIndex[1];
        mMdlData.polyIndex[lpct].stIndex[2] = polyIndex[lpct].stIndex[2];
    }

    /* アニメ関連情報初期化 */
    mMdlData.currentFrame = 0;
    mMdlData.nextFrame = 1;
    mMdlData.interpol = 0.0;

    /* MD2バイナリデータ解放 */
    std::vector<char>().swap(mWkMd2BinData);

    return true;
}

bool Md2ModelInfo::LoadTexture() {
    auto [retbool, texid] = TexObj::LoadTexture(mWkTexBinData);
    if(retbool)
        mTexId = texid;
    else
        mTexId = -1;

    return retbool;
}

bool Md2ModelInfo::LoadShaders() {
    /* シェーダ読込み */
    bool ret = mShaderObj.LoadShaders(mWkVshStrData, mWkFshStrData);
    if( !ret) return false;

    /* シェーダのAttributeにデータ一括設定 */
    return mShaderObj.setAttribute(mShaderObj.mProgramId, mMdlData.numTotalFrames,
                                   mMdlData.numPolys, mMdlData.numVertexsPerFrame,
                                   mMdlData.vertexList, mMdlData.polyIndex, mMdlData.st);
}
