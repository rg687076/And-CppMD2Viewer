#ifndef CPPMD2VIEWER_MD2OBJ_H
#define CPPMD2VIEWER_MD2OBJ_H

#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <array>
#include <GLES2/gl2.h>
#include "Md2Parts.h"
#include "TexObj.h"

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
    bool LoadModel();
    bool LoadTexture(); /* AssetsからTextureデータを読込む */
    bool InitTexture(); /* TextureデータをOpenGLで使えるようにする */
    bool InitShaders(); /* シェーダをOpenGLで使えるようにする */
    bool DrawModel(const std::array<float, 16> &mvpmat, const std::array<float, 16> &normalmat, float scale, float rotatex, float rotatey, float elapsedtimeMs);
    void SetPosition(float x, float y, float z);

public:
    std::string         mName = {0};
    std::vector<char>   mWkMd2BinData = {0};
    std::vector<char>   mWkTexBinData = {0};
    std::string         mWkVshStrData = {0};
    std::string         mWkFshStrData = {0};
    int                 mWkWidth = 0;
    int                 mWkHeight= 0;
    std::vector<char>   mWkRgbaData = {0};
    /* 描画に必要なデータ */
    MdlData                 mMdlData = {0};
    std::array<float, 3>    mPosition = {0, 0, 0};
    /* アニメ関連 */
    std::unordered_map<int, std::pair<int, int>> mFrameIndices = {};
    int mCurrentFrame = 0;
    /* テクスチャ関連 */
    GLuint mTexId = -1;
    /* シェーダー関連 */
    GLuint mVboId         = -1;
    GLuint mProgramId     = -1;
    GLuint mCurPosAttrib  = -1;
    GLuint mNextPosAttrib = -1;
    GLuint mTexCoordAttrib= -1;
};

class Md2Obj {
public:
    static bool LoadModel(std::map<std::string, Md2Model> &md2models);
    static bool InitModel(std::map<std::string, Md2Model> &md2models);
    using ArgType = std::tuple<const std::array<float, 16> &, const std::array<float, 16> &, float, float, float>;
    static bool DrawModel(std::map<std::string, Md2Model> &md2models, const ArgType &globalSpacePrm, float elapsedtimeMs);
};

#endif //CPPMD2VIEWER_MD2OBJ_H
