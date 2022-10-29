#include <iostream>
#include <string>
#include <sstream>
#include <android/log.h>
#include "Md2Obj.h"
#include "TexObj.h"

/* Md2モデルデータ実体 */
std::map<std::string, Md2ModelInfo> gMd2models;

/* Md2モデルsetup */
bool Md2Obj::Init(std::map<std::string, Md2ModelInfo> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : gMd2models) {
        bool ret = value.loadModel(180, 30);
        if(ret == false) return false;
        bool ret2 = value.loadSkin();
        if(ret2 == false) return false;
    }

    return true;
}

Md2ModelInfo::~Md2ModelInfo() {
    std::vector<char>().swap(md2bindata);
    std::vector<char>().swap(texbindata);
    delete [] m_vertices;
    delete [] m_glcmds;
    delete [] m_lightnormals;
    delete [] m_wkbuff;
}

bool Md2ModelInfo::loadModel(float scale, float fps) {
    md2_t header = {0};
    /* MD2ファイルのパース */
    std::istringstream md2binstream(std::string(md2bindata.begin(), md2bindata.end()));
    md2binstream.read((char*)&header, sizeof(md2_t));

    /* MD2形式チェック */
    if(header.ident != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        md2binstream.str("");
        md2binstream.clear(std::stringstream::goodbit);
        const union { int i; char b[4]; } ngno = {header.ident};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(majicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    if(header.version != MD2_VERSION) { /* 8じゃないとエラー */
        md2binstream.str("");
        md2binstream.clear(std::stringstream::goodbit);
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(version=%d) %s %s(%d)", header.version, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    /* 領域確保 */
    m_glcmds        = new int   [ header.num_glcmds ];
    m_wkbuff        = new char  [header.num_frames * header.framesize ];
    m_vertices      = new vec3_t[ header.num_xyz * header.num_frames ];
    m_lightnormals  = new int   [ header.num_xyz * header.num_frames ];

    /* OpenGLコマンド読込み */
    md2binstream.seekg( header.ofs_glcmds, std::ios::beg );
    md2binstream.read( (char *)m_glcmds, header.num_glcmds * sizeof( int ) );

    /* (頂点/法線ベクトルindex)データ読込み */
    md2binstream.seekg(header.ofs_frames, std::ios::beg);
    md2binstream.read((char*)m_wkbuff, header.num_frames * header.framesize );

    /* (頂点/法線ベクトルindex)データを頂点データと法線ベクトルindexデータに詰替え */
    for(int lpj = 0; lpj < header.num_frames; lpj++ ){
        frame_t	*frame     = (frame_t*)&m_wkbuff[header.framesize * lpj ];
        vec3_t	*ptrverts  = &m_vertices[header.num_xyz * lpj ];
        int     *ptrnormals= &m_lightnormals[header.num_xyz * lpj ];

        for(int lpi = 0; lpi < header.num_xyz; lpi++ ){
            /* 頂点データ詰替え(scaleと移動量を計算しとく) */
            ptrverts[lpi][0] = (frame->verts[lpi].v[0] * frame->scale[0]) + frame->translate[0];
            ptrverts[lpi][1] = (frame->verts[lpi].v[1] * frame->scale[1]) + frame->translate[1];
            ptrverts[lpi][2] = (frame->verts[lpi].v[2] * frame->scale[2]) + frame->translate[2];
            /* 法線ベクトルindexを詰替え */
            ptrnormals[lpi] = frame->verts[lpi].lightnormalindex;
        }
    }

    /* 後片付け */
    delete[] m_wkbuff;
    m_wkbuff = nullptr;
    md2binstream.str("");                           /* バッファクリア */
    md2binstream.clear(std::stringstream::goodbit); /* 状態クリア */

    return true;
}

bool Md2ModelInfo::loadSkin() {
    /* Textureファイルのパース */
    std::istringstream texbinstream(std::string(texbindata.begin(), texbindata.end()));
    /* フォーマットチェック(BMP) */
    char bm[2] = {0};
    texbinstream.read(bm, sizeof(bm));
    if(bm[0]=='B' && bm[1]=='M') {
        /* BMP形式確定 */
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " BMP形式(%c %c) %s %s(%d)", bm[0], bm[1], __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        texinfo = LoadTexture(FileFormat::BMP, texbinstream);
        return true;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " BMP形式ではない(%c %c) %s %s(%d)", bm[0], bm[1], __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    /* フォーマットチェック(TGA) */
    texbinstream.seekg(-18, std::ios::end );
    char TRUEVISION_TARGA[18] = {0};
    texbinstream.read(TRUEVISION_TARGA, sizeof(TRUEVISION_TARGA));
    if(std::string(TRUEVISION_TARGA).find("TRUEVISION-") == 0) {
        /* TGA形式確定 */
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " TGA形式(%s) %s %s(%d)", TRUEVISION_TARGA, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        texinfo = LoadTexture(FileFormat::TGA, texbinstream);
        return true;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", " TGA形式ではない(%s) %s %s(%d)", TRUEVISION_TARGA, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return false;
}
