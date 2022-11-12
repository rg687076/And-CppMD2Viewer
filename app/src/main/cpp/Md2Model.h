#ifndef CPPMD2VIEWER_MD2MODEL_H
#define CPPMD2VIEWER_MD2MODEL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <GLES2/gl2.h>
#include "Md2Parts.h"

const int MD2_IDENT = (('2'<<24) + ('P'<<16) + ('D'<<8) + 'I'); /* magic number "IDP2" or 844121161 */
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

public:
    /* モデルデータ */
    MdlData                 mMdlData = {0};
    /* アニメ関連 */
    int numTotalFrames = 0;
    std::unordered_map<int, std::pair<int, int>> mFrameIndices = {};
    /* テクスチャ関連 */
    GLuint mTexId = -1;
    /* シェーダー関連 */
    GLuint mVboId         = -1;
    GLuint mProgramId     = -1;
    GLuint mCurPosAttrib  = -1;
    GLuint mNextPosAttrib = -1;
    GLuint mTexCoordAttrib= -1;
};

#endif //CPPMD2VIEWER_MD2MODEL_H
