#include <iostream>
#include <string>
#include <sstream>
#include <tuple>
#include <android/log.h>
#include "Md2Parts.h"
#include "Md2Obj.h"
#include "TexObj.h"
#include "GlObj.h"

/* Md2モデル読込み(model読込,tex読込) */
bool Md2Obj::LoadModel(std::map<std::string, Md2Model> &md2models) {
    __android_log_print(ANDROID_LOG_INFO, "aaaaa", "%s %s(%d)", __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);

    for(auto &[key, value] : md2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model load start (%s). %s %s(%d)", value.mName.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* MD2モデルLoad */
        bool ret = value.LoadModel();
        std::vector<char>().swap(value.mWkMd2BinData);
        if( !ret) return false;
        /* テクスチャLoad */
        bool ret1 = value.LoadTexture();
        std::vector<char>().swap(value.mWkTexBinData);
        if( !ret1) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model and Texture LOADED(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }

    return true;
}

/* Md2モデル初期化(特にOpenGL系は、onSurfaceCreated()ドリブンで動作しないとエラーになる) */
bool Md2Obj::InitModel(std::map<std::string, Md2Model> &md2models) {
    for(auto &[key, value] : md2models) {
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Md2Model Init start (%s). %s %s(%d)", value.mName.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        /* テクスチャInit */
        bool ret2 = value.InitTexture();
        std::vector<char>().swap(value.mWkTexBinData);
        if( !ret2) return false;
        /* シェーダ初期化 */
        bool ret3 = value.InitShaders();
        if( !ret3) return false;
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "Shader Init end(%s). %s %s(%d)", key.c_str(), __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
    }
    return true;
}

/* Md2モデル描画 */
bool Md2Obj::DrawModel(std::map<std::string, Md2Model> &md2models, const Md2Obj::ArgType &globalSpacePrm, float elapsedtimeMs) {
    const std::array<float, 16> &aMvpMat     = std::get<0>(globalSpacePrm);
    const std::array<float, 16> &amNormalMat = std::get<1>(globalSpacePrm);
    float Scale                              = std::get<2>(globalSpacePrm);
    float Rotatex                            = std::get<3>(globalSpacePrm);
    float Rotatey                            = std::get<4>(globalSpacePrm);

    /* glEnable(GL_DEPTH_TEST); */
    GlObj::enable(GL_DEPTH_TEST);

    for(auto &[key, value] : md2models) {
        value.DrawModel(aMvpMat, amNormalMat, Scale, Rotatex, Rotatey, elapsedtimeMs);
    }
    return true;
}

void Md2Model::SetPosition(float x, float y, float z) {
	mPosition = std::array<float, 3>{x, y, z};
}

Md2Model::~Md2Model() {
    std::vector<char>().swap(mWkMd2BinData);
    std::vector<char>().swap(mWkTexBinData);
    std::vector<char>().swap(mWkRgbaData);
    std::unordered_map<int, std::pair<int, int>>().swap(mFrameIndices);
}

bool Md2Model::LoadModel() {
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

/* AssetsからTextureデータを読込む */
bool Md2Model::LoadTexture() {
    auto [retbool, w, h, rgbabindata] = TexObj::LoadTexture(mWkTexBinData);
    if(retbool) {
        mWkWidth = w;
        mWkHeight= h;
        mWkRgbaData = std::move(rgbabindata);
    }
    return retbool;
}

/* TextureデータをOpenGLで使えるようにする */
bool Md2Model::InitTexture() {
    /* OpenGLのTexture初期化 */
    auto[boolret, texid] = GlObj::InitTexture(mWkWidth, mWkHeight, mWkRgbaData.data());
    if(boolret)
        mTexId = texid;

    /* 解放処理 */
    mWkWidth = 0;
    mWkHeight= 0;
    std::vector<char>().swap(mWkRgbaData);

    return boolret;
}

/* シェーダをOpenGLで使えるようにする */
bool Md2Model::InitShaders() {
    /* シェーダ読込み */
    auto[retboot, progid] = GlObj::LoadShaders(mWkVshStrData, mWkFshStrData);
    if( !retboot) {
        mProgramId = -1;
        return false;
    }
    mProgramId = progid;

    /* シェーダのAttributeにデータ一括設定 */
    auto[retbool, retAnimFrameS2e, retVboID, retCurPosAttrib, retNextPosAttrib, retTexCoordAttrib] = GlObj::setAttribute(mProgramId, mMdlData.numTotalFrames, mMdlData.vertexList, mMdlData.polyIndex, mMdlData.st);
    if( !retbool) {
        GlObj::DeleteShaders(mProgramId);
        mProgramId =-1;
        return false;
    }

    mFrameIndices  = std::move(retAnimFrameS2e);
    mVboId         = retVboID;
    mCurPosAttrib  = retCurPosAttrib;
    mNextPosAttrib = retNextPosAttrib;
    mTexCoordAttrib= retTexCoordAttrib;

    return true;
}

bool Md2Model::DrawModel(const std::array<float, 16> &mvpmat, const std::array<float, 16> &normalmat, float scale, float rotatex, float rotatey, float elapsedtimeMs) {
    static const int START_FRAME= 0;
    static const int END_FRAME  = (int)mFrameIndices.size() - 1;

    /* 補完係数の計算 */
    static std::map<std::string, float> interpolates = {{"female", -0.1f}, {"grunt", -0.1f}};   /* 本来は経過時間と絡めて算出すべきだけど、今回はズルした。単純インクリメントで、対応 */
    float interpolate = interpolates.at(mName);
    if(interpolate >= 1.0f) {
        interpolate = 0.0f;
        if(mCurrentFrame >= END_FRAME)
            mCurrentFrame = START_FRAME;
        else
            mCurrentFrame++;
    }
    interpolate += 0.1f;
    interpolates.at(mName) = interpolate;

    /* glActiveTexture() → glBindTexture() */
    GlObj::activeTexture(GL_TEXTURE0);
    GlObj::bindTexture(GL_TEXTURE_2D, mTexId);

    /* glUseProgram() */
    GlObj::useProgram(mProgramId);

    /* glUniformXxxxx() */
    GlObj::setUniform(mProgramId, "mvpmat", mvpmat);
    GlObj::setUniform(mProgramId, "interpolate", interpolate);

    /* glBindBuffer(有効化) */
    GlObj::bindBuffer(GL_ARRAY_BUFFER, mVboId);

    /* glVertexAttribPointer()×3(現在頂点s,次頂点s,UV座標) */
    GlObj::vertexAttribPointer(mCurPosAttrib  , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
    GlObj::vertexAttribPointer(mNextPosAttrib , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    GlObj::vertexAttribPointer(mTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    /* glDrawArrays() */
    int sidx = mFrameIndices[mCurrentFrame].first;
    int size = mFrameIndices[mCurrentFrame].second - mFrameIndices[mCurrentFrame].first - 1;
    GlObj::drawArrays(GL_TRIANGLES, sidx, size);

    /* glBindBuffer(無効化) */
    GlObj::bindBuffer(GL_ARRAY_BUFFER, NULL);

    return true;
}
