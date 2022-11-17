#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <android/log.h>
#include "Md2Model.h"
#include "TexObj.h"
#include "GlObj.h"
#include "MatVec.h"

std::map<std::string, TmpBinData2> gTmpRgbDatas;   /* 読込み用の一時置き場 */

Md2Model::~Md2Model() {
    std::unordered_map<int, std::pair<int, int>>().swap(mFrameIndices);
}

/* binデータからMd2データを読込む */
bool Md2Model::loadModel(const std::string &/*key*/, const std::vector<char> &md2bindata) {
    /* MD2ヘッダ */
    md2header *header = (md2header*)md2bindata.data();

    /* MD2形式チェック */
    if(header->magicnumber != MD2_IDENT) { /* "IDP2"じゃないとエラー */
        const union { int i; char b[4]; } ngno = {header->magicnumber};
        __android_log_print(ANDROID_LOG_INFO, "aaaaa", "MD2フォーマット不正(magicnumber=%s) %s %s(%d)", ngno.b, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__);
        return false;
    }

    if(header->version != MD2_VERSION) { /* 8じゃないとエラー */
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
        frame *lframe = (frame*)&(md2bindata[header->offset_frames + header->framesize * lpct]);
        vertex *pvertex = &mMdlData.vertexList[header->num_vertexs * lpct];
        for (size_t lpct2 = 0; lpct2 < header->num_vertexs; lpct2++) {
            pvertex[lpct2].v[0] = lframe->scale[0] * lframe->fp[lpct2].v[0] + lframe->translate[0];
            pvertex[lpct2].v[1] = lframe->scale[1] * lframe->fp[lpct2].v[1] + lframe->translate[1];
            pvertex[lpct2].v[2] = lframe->scale[2] * lframe->fp[lpct2].v[2] + lframe->translate[2];
        }
    }

    /* uvデータ読込み */
    mMdlData.st.resize(header->num_st);
    texindex *sts = (texindex*)&md2bindata[header->offset_st];
    for (size_t lpct = 0; lpct < header->num_st; lpct++) {
        mMdlData.st[lpct].s = static_cast<float>(sts[lpct].s) / static_cast<float>(header->skinwidth);
        mMdlData.st[lpct].t = static_cast<float>(sts[lpct].t) / static_cast<float>(header->skinheight);
    }

    /* mesh情報読込み */
    mMdlData.polyIndex.resize(header->num_polys);
    mesh *polyIndex = (mesh*)&md2bindata[header->offset_meshs];
    for (size_t lpct = 0; lpct < header->num_polys; lpct++) {
        mMdlData.polyIndex[lpct].meshIndex[0] = polyIndex[lpct].meshIndex[0];
        mMdlData.polyIndex[lpct].meshIndex[1] = polyIndex[lpct].meshIndex[1];
        mMdlData.polyIndex[lpct].meshIndex[2] = polyIndex[lpct].meshIndex[2];

        mMdlData.polyIndex[lpct].stIndex[0] = polyIndex[lpct].stIndex[0];
        mMdlData.polyIndex[lpct].stIndex[1] = polyIndex[lpct].stIndex[1];
        mMdlData.polyIndex[lpct].stIndex[2] = polyIndex[lpct].stIndex[2];
    }

//    /* アニメ関連情報初期化 */
//    mMdlData.currentFrame = 0;
//    mMdlData.nextFrame = 1;
//    mMdlData.interpol = 0.0;

    return true;
}

/* AssetsからTextureデータを読込む */
bool Md2Model::loadTexture(const std::string &key, const std::vector<char> &texbindata) {
    auto [retbool, w, h, rgbabindata] = TexObj::LoadTexture(texbindata);
    if(retbool) {
        gTmpRgbDatas.emplace(key, TmpBinData2{.mName=key,
                                              .mWkWidth=w,
                                              .mWkHeight=h,
                                              .mWkRgbaData=std::move(rgbabindata)});
    }

    return retbool;
}

/* TextureデータをOpenGLで使えるようにする */
bool Md2Model::initTexture(const std::string &key) {
    TmpBinData2 bindata = gTmpRgbDatas.at(key);
    /* OpenGLのTexture初期化 */
    auto[boolret, texid] = GlObj::InitTexture(bindata.mWkWidth, bindata.mWkHeight, bindata.mWkRgbaData.data());
    if(boolret)
        mTexId = texid;

    /* 解放処理 */
    bindata.mWkWidth = 0;
    bindata.mWkHeight= 0;
    std::vector<char>().swap(bindata.mWkRgbaData);
    gTmpRgbDatas.erase(key);
    if(gTmpRgbDatas.size() == 0)
        gTmpRgbDatas.clear();

    return boolret;
}

/* シェーダをOpenGLで使えるようにする */
bool Md2Model::initShaders(const std::string &key, const TmpBinData3 &tmpbindata3) {
    /* シェーダ読込み */
    auto[retboot, progid] = GlObj::LoadShaders(tmpbindata3.mWkVshStrData, tmpbindata3.mWkFshStrData);
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

bool Md2Model::drawModel(const std::array<float, 16> &normalmat, float elapsedtimeMs) {
    /* glActiveTexture() → glBindTexture() */
    GlObj::activeTexture(GL_TEXTURE0);
    GlObj::bindTexture(GL_TEXTURE_2D, mTexId);

    /* glUseProgram() */
    GlObj::useProgram(mProgramId);

    /* glUniformXxxxx() */
    GlObj::setUniform(mProgramId, "mvpmat",      mMvpMat);
    GlObj::setUniform(mProgramId, "interpolate", minterpolate);

    /* glBindBuffer(有効化) */
    GlObj::bindBuffer(GL_ARRAY_BUFFER, mVboId);

    /* glVertexAttribPointer()×3(現在頂点s,次頂点s,UV座標) */
    GlObj::vertexAttribPointer(mCurPosAttrib  , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
    GlObj::vertexAttribPointer(mNextPosAttrib , 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    GlObj::vertexAttribPointer(mTexCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));

    /* glDrawArrays() */
    int sidx = mFrameIndices[mCurrentFrame].first;
    int size = mFrameIndices[mCurrentFrame].second - mFrameIndices[mCurrentFrame].first + 1;
    GlObj::drawArrays(GL_TRIANGLES, sidx, size);

    /* glBindBuffer(無効化) */
    GlObj::bindBuffer(GL_ARRAY_BUFFER, NULL);

    /* 補完係数の計算 */
    if(minterpolate >= 1.0f) {
        minterpolate = 0.0f;
        if(mCurrentFrame >= (mFrameIndices.size()-1))
            mCurrentFrame = 0;
        else
            mCurrentFrame++;
    }
    minterpolate += 0.1f;

    return true;
}

void Md2Model::setInitPosition(const std::array<float,3> &scale, const std::array<float,3> &rot, const std::array<float,3> &translate) {
    mInitScaleVec  = scale;
    mInitRotateVec = rot;
    mInitTransVec  = translate;

    /* モデル行列更新 */
    mModelMat = calcModelMat();
    /* モデル/ビュー/投影行列更新 */
    mMvpMat = MatVec::multMatrixf(mExpiringVpMat, mModelMat);

    return;
}

void Md2Model::setPosition(float x, float y, float z) {
    mPosition = {x, y, z};

    /* モデル行列更新 */
    mModelMat = calcModelMat();

    /* モデル/ビュー/投影行列更新 */
    mMvpMat = MatVec::multMatrixf(mExpiringVpMat, mModelMat);

    return;
}

/* 回転行列を設定 */
void Md2Model::setRotate(float rotatex, float rotatey) {
    /* 回転設定 */
    mRotatex = rotatex;
    mRotatey = rotatey;

    /* モデル行列更新 */
    mModelMat = calcModelMat();

    /* モデルビュー投影行列更新 */
    mMvpMat = MatVec::multMatrixf(mExpiringVpMat, mModelMat);
    return;
}

/* 拡縮行列を設定 */
void Md2Model::setScale(float scale) {
    /* 拡縮設定 */
    mScale = scale;

    /* モデル行列更新 */
    mModelMat = calcModelMat();

    /* モデルビュー投影行列更新 */
    mMvpMat = MatVec::multMatrixf(mExpiringVpMat, mModelMat);
    return;
}

/* View投影行列を設定 */
void Md2Model::setVpMat(const std::array<float, 16> &vpmat) {
    /* ビュー投影行列更新 */
    mExpiringVpMat = vpmat;
    /* モデルビュー投影行列更新 */
    mMvpMat = MatVec::multMatrixf(vpmat, mModelMat);
    return;
}

/* View行列を計算 */
std::array<float,16> Md2Model::calcModelMat() {
    std::array<float, 16> modelmat = MatVec::getRotatef(-mRotatex-mInitRotateVec[0], 1.0f, 0.0f, 0.0f);
//    modelmat = MatVec::rotatef(modelmat, mRotatey+mInitRotateVec[1], 0.0f, 1.0f, 0.0f);
    modelmat = MatVec::rotatef(modelmat, mRotatey+mInitRotateVec[1], 0.0f, 0.0f, 1.0f); /* xz平面だからz軸に対して回転を掛ける */
    modelmat = MatVec::rotatef(modelmat, mInitRotateVec[2], 0.0f, 0.0f, 1.0f);
    modelmat = MatVec::translatef(modelmat, mPosition[0]+mInitTransVec[0], mPosition[1]+mInitTransVec[1], mPosition[2]+mInitTransVec[2]);
    modelmat = MatVec::scalef(modelmat, mScale*mInitScaleVec[0], mScale*mInitScaleVec[1], mScale*mInitScaleVec[2]);

    return modelmat;
}
