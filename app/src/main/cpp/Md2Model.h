#ifndef CPPMD2VIEWER_MD2MODEL_H
#define CPPMD2VIEWER_MD2MODEL_H

#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <GLES2/gl2.h>
#include "Md2Parts.h"
#include "MatVec.h"

const int MD2_IDENT   = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I'); /* magic number "IDP2" or 844121161 */
const int MD2_VERSION = 8;                                        /* model version */

class TmpBinData1 {
public:
    std::string         mName = {};
    std::vector<char>   mWkMd2BinData = {};
    std::vector<char>   mWkTexBinData = {};
};

class TmpBinData2 {
public:
    std::string         mName = {};
    int                 mWkWidth = 0;
    int                 mWkHeight= 0;
    std::vector<char>   mWkRgbaData = {};
};

class TmpBinData3 {
public:
    std::string         mName = {};
    std::string         mWkVshStrData = {};
    std::string         mWkFshStrData = {};
};

struct MdlData {
    int numTotalFrames;
    int numVertexsPerFrame;
//    int numPolys;
//    int twidth;
//    int theight;
//    int currentFrame;
//    int nextFrame;
//    float interpol;
    std::vector<vertex>     vertexList;
    std::vector<texstcoord> st;
    std::vector<mesh>       polyIndex;
//    float x, y, z;
//    float nextX, nextY, nextZ;
//    float radius;
//    float dist_to_player;
//    int state;
//    float speed;
};

class Md2Model {
public:
    ~Md2Model();
    bool loadModel(const std::string &key, const std::vector<char> &md2bindata);   /* binデータからMd2データを読込む */
    bool loadTexture(const std::string &key, const std::vector<char> &texbindata); /* AssetsからTextureデータを読込む */
    bool initTexture(const std::string &key);                                      /* TextureデータをOpenGLで使えるようにする */
    bool initShaders(const std::string &key, const TmpBinData3 &tmpbindata3);      /* シェーダをOpenGLで使えるようにする */
    bool drawModel(const std::array<float, 16> &normalmat, float elapsedtimeMs);
    void setInitPosition(const std::array<float,3> &scale, const std::array<float,3> &rot, const std::array<float,3> &translate);
    void setPosition(float x, float y, float z);
    void setRotate(float rotatex, float rotatey);
    void setScale(float scale);
    void setVpMat(const std::array<float, 16> &vpmat);

public:
    /* モデルデータ */
    MdlData                 mMdlData = {0};
    /* アニメ関連 */
    int numTotalFrames = 0;
    std::unordered_map<int, std::pair<int, int>> mFrameIndices = {};
    int   mCurrentFrame = 0;
    float minterpolate = 0.0f;
    /* テクスチャ関連 */
    GLuint mTexId = -1;
    /* シェーダー関連 */
    GLuint mVboId         = -1;
    GLuint mProgramId     = -1;
    GLuint mCurPosAttrib  = -1;
    GLuint mNextPosAttrib = -1;
    GLuint mTexCoordAttrib= -1;
    /* 姿勢制御 */
    float                mRotatex  = 0;
    float                mRotatey  = 0;
    float                mScale    = 1.0f;
    std::array<float, 3> mPosition = {0, 0, 0};
    /* 行列 */
    std::array<float, 16> mModelMat      = MatVec::IDENTITY;
    std::array<float, 16> mExpiringVpMat = MatVec::IDENTITY;  /* 死にかけのView投影行列(随時、最新化されるのを期待する) */
    std::array<float, 16> mMvpMat        = MatVec::IDENTITY;  /* ModelView投影行列 */
};

#endif //CPPMD2VIEWER_MD2MODEL_H
