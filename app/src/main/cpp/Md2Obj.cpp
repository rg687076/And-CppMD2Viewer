#include <iostream>
#include <string>
#include <sstream>
#include <android/log.h>
#include "Md2Obj.h"
#include "TexObj.h"

/* Md2モデルデータ実体 */
std::map<std::string, Md2ModelInfo> gMd2models;

/* Md2モデル初期化(model読込,tex読込) */
bool Md2Obj::Init(std::map<std::string, Md2ModelInfo> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : gMd2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model losd start (%s). %s %s(%d)", value.name.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* MD2モデルLoad */
        bool ret = value.loadModel();
        std::vector<char>().swap(value.md2bindata);
        if( !ret) return false;
        /* テクスチャLoad */
        bool ret2 = value.LoadTexture();
        std::vector<char>().swap(value.texbindata);
        if( !ret2) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2 and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

Md2ModelInfo::~Md2ModelInfo() {
    std::vector<char>().swap(md2bindata);
    std::vector<char>().swap(texbindata);
}

bool Md2ModelInfo::loadModel() {
//    std::istringstream md2binstream(std::string(md2bindata.begin(), md2bindata.end()));
//    md2binstream.read((char*)&header, sizeof(md2header));
//    md2binstream.str("");
//    md2binstream.clear(std::stringstream::goodbit);

    /* MD2ヘッダ */
    md2header *header = (md2header*)md2bindata.data();

    /* MD2形式チェック */
    if(header->magicnumber != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        std::vector<char>().swap(md2bindata);
        const union { int i; char b[4]; } ngno = {header->magicnumber};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(magicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    if(header->version != MD2_VERSION) { /* 8じゃないとエラー */
        std::vector<char>().swap(md2bindata);
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(version=%d) %s %s(%d)", header->version, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* 初期化 */
    mdldata.numVertexsPerFrame= header->num_vertexs;
    mdldata.numTotalFrames    = header->num_totalframes;

    /* 頂点読込み */
    mdldata.vertexList.resize(header->num_vertexs * header->num_totalframes);
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "1frame当りの頂点数(%d)と総フレーム数(%d) %s %s(%d)", header->num_vertexs, header->num_totalframes, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    for(size_t lpct = 0; lpct < header->num_totalframes; lpct++) {
        frame *lframe = (frame*)&(md2bindata[header->offset_frames + header->framesize * lpct]);
        vertex *pvertex = &mdldata.vertexList[header->num_vertexs * lpct];
        for (size_t lpct2 = 0; lpct2 < header->num_vertexs; lpct2++) {
            pvertex[lpct2].v[0] = lframe->scale[0] * lframe->fp[lpct2].v[0] + lframe->translate[0];
            pvertex[lpct2].v[1] = lframe->scale[1] * lframe->fp[lpct2].v[1] + lframe->translate[1];
            pvertex[lpct2].v[2] = lframe->scale[2] * lframe->fp[lpct2].v[2] + lframe->translate[2];
        }
    }

    /* uvデータ読込み */
    mdldata.st.resize(header->num_st);
    texindex *sts = (texindex*)&md2bindata[header->offset_st];
    for (size_t lpct = 0; lpct < header->num_st; lpct++) {
        mdldata.st[lpct].s = static_cast<float>(sts[lpct].s) / static_cast<float>(header->skinwidth);
        mdldata.st[lpct].t = static_cast<float>(sts[lpct].t) / static_cast<float>(header->skinheight);
    }

    /* mesh情報読込み */
    mdldata.polyIndex.resize(header->num_polys);
    mdldata.numPolys = header->num_polys;
    mesh *polyIndex = (mesh*)&md2bindata[header->offset_meshs];
    for (size_t lpct = 0; lpct < header->num_polys; lpct++) {
        mdldata.polyIndex[lpct].meshIndex[0] = polyIndex[lpct].meshIndex[0];
        mdldata.polyIndex[lpct].meshIndex[1] = polyIndex[lpct].meshIndex[1];
        mdldata.polyIndex[lpct].meshIndex[2] = polyIndex[lpct].meshIndex[2];

        mdldata.polyIndex[lpct].stIndex[0] = polyIndex[lpct].stIndex[0];
        mdldata.polyIndex[lpct].stIndex[1] = polyIndex[lpct].stIndex[1];
        mdldata.polyIndex[lpct].stIndex[2] = polyIndex[lpct].stIndex[2];
    }

    /* アニメ関連情報初期化 */
    mdldata.currentFrame = 0;
    mdldata.nextFrame = 1;
    mdldata.interpol = 0.0;

    /* MD2バイナリデータ解放 */
    std::vector<char>().swap(md2bindata);

    return true;
}

bool Md2ModelInfo::LoadTexture() {
    /* Textureファイルのパース */
    std::istringstream texbinstream(std::string(texbindata.begin(), texbindata.end()));
    /* フォーマットチェック(BMP) */
    char bm[2] = {0};
    texbinstream.read(bm, sizeof(bm));
    if(bm[0]=='B' && bm[1]=='M') {
        /* BMP形式確定 */
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "BMP形式(%c %c) %s %s(%d)", bm[0], bm[1], __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        texdata = TexData::LoadTexture(FileFormat::BMP, texbindata);
        return true;
    }
    else {
        __android_log_print(ANDROID_LOG_ERROR, "aaaaa", "BMP形式ではない(%c %c) %s %s(%d)", bm[0], bm[1], __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
//        return false;   /* TGAの可能性がある */
    }

    /* フォーマットチェック(TGA) */
    texbinstream.seekg(-18, std::ios::end );
    char TRUEVISION_TARGA[18] = {0};
    texbinstream.read(TRUEVISION_TARGA, sizeof(TRUEVISION_TARGA));
    if(std::string(TRUEVISION_TARGA).find("TRUEVISION-") == 0) {
        /* TGA形式確定 */
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " TGA形式(%s) %s %s(%d)", TRUEVISION_TARGA, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        texdata = TexData::LoadTexture(FileFormat::TGA, texbindata);
        return true;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " TGA形式ではない(%s) %s %s(%d)", TRUEVISION_TARGA, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
//        return false;   /* 他の形式の可能性がある */
    }

    return false;
}
