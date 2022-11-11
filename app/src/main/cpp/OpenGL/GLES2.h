#pragma warning(disable:4819)
//
// Created by jun on 2021/06/09.
//
#ifndef ANDCG3DVIEWER_GLES2_H
#define ANDCG3DVIEWER_GLES2_H
#include <vector>
#include <array>
#include "../format/MatVec.h"

/* glシェーダ関連オブジェクトの管理 構造体 */
typedef struct {
    int programId;
    int a_VertexId;
    int a_NormalId;
    int a_ColorId;
    int a_TexCoordId;
    int u_MvpMatrixId;
    int u_NormalMatrixId;
    int u_TexSamplerId;
//  int u_BmpSamplerId; /* 使い方不明 今回は未使用 */
} GlShaderObj;

/* gl頂点関連オブジェクトの管理 構造体 */
typedef struct {
    int VertexBufId;     /* 頂点座標用のバッファオブジェクト */
    int NormalBuffId;     /* 法線用のバッファオブジェクト */
    int ColorBuffId;      /* 色用のバッファオブジェクト */
    int TexCoordBuffId;   /* uv用のバッファオブジェクト */
} GlBufferObj;

class DrawInfo {
public:
    std::vector<m::Vector3f> mVirtexs;       /* 頂点情報 */
    std::vector<m::Vector3f> mNormals;       /* 法線情報 */
    std::vector<m::UV>         mUVs;           /* UV情報 */
    std::vector<m::Color4>     mColors;        /* カラー情報 */
    int    mTexWidth;      /* 画像データ幅 */
    int    mTexHeight;     /* 画像データ高 */
    int    mTexBinSize;    /* 画像データ幅 */
    char  *mTexBinData;    /* 画像データ */
    int    mVerArrySize;
    float *mVerArry;
    int    mNorArrySize;
    float *mNorArry;
    int    mUvsArrySize;
    float *mUvsArry;
    int    mClrArrySize;
    float *mClrArry;
    DrawInfo():mVirtexs(),mNormals(),mUVs(),mColors(),mTexWidth(0),mTexHeight(0),mTexBinSize(0),mTexBinData(NULL),mVerArrySize(0),mVerArry(NULL),mNorArrySize(0),mNorArry(NULL),mUvsArrySize(0),mUvsArry(NULL),mClrArrySize(0),mClrArry(NULL){}
    DrawInfo &operator=(const DrawInfo &rhs);
    ~DrawInfo();
};

class GlRenderData {
public:
    GlShaderObj     mGlShaderObjs[2];   /* glシェーダ関連情報 */
    GlBufferObj     mGlBufferObj;       /* glバーテックス関連情報 */
    unsigned int    mTexId;

    std::vector<DrawInfo>   mDrawInfos; /* 描画用データ */

    static GlRenderData &GetIns() {
        static GlRenderData instance;
        return instance;
    }

public:
    /* 座標変換行列 */
    std::array<float, 16> mModelMatrix; /* モデル行列 */
    std::array<float, 16> mVpMatrix;    /* ビュー投影行列 */
    std::array<float, 16> mMvpMatrix;   /* モデルビュー投影行列 */
    std::array<float, 16> mNormalMatrix;/* 法線の座標変換用行列 */
    float mTouchAngleX;
    float mTouchAngleY;
    float mScale;

private:
    GlRenderData():mModelMatrix(),mVpMatrix(),mMvpMatrix(),mNormalMatrix(),mTouchAngleX(0),mTouchAngleY(0),mScale(1),mTexId(0),mGlShaderObjs(),mGlBufferObj(){}
    GlRenderData(const GlRenderData &):mModelMatrix(),mVpMatrix(),mMvpMatrix(),mNormalMatrix(),mTouchAngleX(0),mTouchAngleY(0),mTexId(0),mGlShaderObjs(),mGlBufferObj() {}
    GlRenderData &operator=(const GlRenderData&){ return *this; }
};

class GLES2 {
public:
    static bool OpenGLInit();
    static bool ShaderInit(const std::vector<char> &colvsh, const std::vector<char> &colfsh,
                           const std::vector<char> &texvsh, const std::vector<char> &texfsh, GlShaderObj aShaders[2], unsigned int &TexId);
    static bool BufferInit(GlBufferObj &aVertex);
    static void draw();
private:
    static unsigned int loadShaders(const std::vector<char> &vsh, const std::vector<char> &fsh);
    static unsigned int compileShader(const std::vector<char> &aShaderSource, const int aType);
    static bool linkProgram(const unsigned int programId);
    static void calcCordinate(GlShaderObj &shaderobj, std::array<float, 16> &aModelMatrix, std::array<float, 16> &aViewProjMatrix, std::array<float, 16> &aMvpMatrix, std::array<float, 16> &aNormalMatrix);
};


#endif //ANDCG3DVIEWER_GLES2_H
