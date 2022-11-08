#ifndef CPPMD2VIEWER_MD2OBJ_H
#define CPPMD2VIEWER_MD2OBJ_H

#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <GLES2/gl2.h>
#include "Md2Parts.h"
#include "MatObj.h"

#define MD2_IDENT   (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I')    /* magic number "IDP2" or 844121161 */
#define	MD2_VERSION 8                                           /* model version */

struct MdlData {
    int numTotalFrames;
    int numVertexsPerFrame;
    int numPolys;
    int twidth;
    int theight;
    int currentFrame;
    int nextFrame;
    float interpol;
    std::vector<vertex>     vertexList;
    std::vector<texstcoord> st;
    std::vector<mesh>       polyIndex;
    float x, y, z;
    float nextX, nextY, nextZ;
    float radius;
    float dist_to_player;
    int state;
    float speed;
};

class Md2Model {
public:
    ~Md2Model();
    bool loadModel();   /* AssetsからMd2データを読込む */
    bool loadTexture(); /* AssetsからTextureデータを読込む */
    bool initTexture(); /* TextureデータをOpenGLで使えるようにする */
    bool initShaders(); /* シェーダをOpenGLで使えるようにする */
    bool drawModel(const std::array<float, 16> &normalmat, float elapsedtimeMs);
    void setPosition(float x, float y, float z);
    void setRotate(float x, float y);
    void setScale(float scale);
    void setVpMat(const std::array<float, 16> &vpmat);

public:
    std::string         mName = {0};
    /* 一時データ領域 */
    std::vector<char>   mWkMd2BinData = {0};
    std::vector<char>   mWkTexBinData = {0};
    std::string         mWkVshStrData = {0};
    std::string         mWkFshStrData = {0};
    int                 mWkWidth = 0;
    int                 mWkHeight= 0;
    std::vector<char>   mWkRgbaData = {0};
    /* 描画に必要なデータ */
    MdlData                 mMdlData = {0};
    /* アニメ関連 */
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
    float                mScale    = 1.0f;
    float                mRotatex  = 180.0f;
    float                mRotatey  = 0;
    std::array<float, 3> mPosition = {0, 0, 0};
    /* 行列 */
    std::array<float, 16> mModelMat      = Mat44::IDENTITY;
    std::array<float, 16> mExpiringVpMat = Mat44::IDENTITY;  /* 死にかけのView投影行列(随時、最新化されるのを期待する) */
    std::array<float, 16> mMvpMat        = Mat44::IDENTITY;  /* ModelView投影行列 */
};

class Md2Obj {
public:
    static bool LoadModel(std::map<std::string, Md2Model> &md2models);
    static bool InitModel(std::map<std::string, Md2Model> &md2models);
    static bool DrawModel(std::map<std::string, Md2Model> &md2models, const std::array<float, 16> &amNormalMat, float elapsedtimeMs);
    static void SetScale(std::map<std::string, Md2Model> &md2models, float scale);
    static void SetRotate(std::map<std::string, Md2Model> &md2models, float x, float y);
};

#endif //CPPMD2VIEWER_MD2OBJ_H
